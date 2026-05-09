from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
import json
import os
from pathlib import Path
from datetime import datetime, timezone
from urllib.parse import urlparse

from email_service import send_alert_email
from risk_checker import check_risks


HOST = os.getenv("API_HOST", "127.0.0.1")
PORT = int(os.getenv("API_PORT", "8000"))
ALERT_RECIPIENT = os.getenv(
    "ALERT_RECIPIENT", "equipe.monitoramento@hospital.example"
)
STATIC_DIR = Path(__file__).parent / "static"
VITALS = []


def utc_now_iso():
    return datetime.now(timezone.utc).isoformat()


def validate_vitals(payload):
    required_fields = {
        "patient_id": str,
        "patient_name": str,
        "heart_rate": (int, float),
        "temperature": (int, float),
        "movement_detected": bool,
    }

    errors = []
    for field, expected_type in required_fields.items():
        if field not in payload:
            errors.append(f"Campo obrigatorio ausente: {field}")
        elif not isinstance(payload[field], expected_type):
            errors.append(f"Campo {field} possui tipo invalido")

    if "heart_rate" in payload and isinstance(payload.get("heart_rate"), (int, float)):
        if payload["heart_rate"] <= 0:
            errors.append("heart_rate deve ser maior que zero")

    if "temperature" in payload and isinstance(payload.get("temperature"), (int, float)):
        if payload["temperature"] < 30 or payload["temperature"] > 45:
            errors.append("temperature deve estar entre 30 e 45 graus Celsius")

    return errors


class VitalsHandler(BaseHTTPRequestHandler):
    def _send_text(self, status_code, content, content_type):
        response = content.encode("utf-8")
        self.send_response(status_code)
        self.send_header("Content-Type", f"{content_type}; charset=utf-8")
        self.send_header("Content-Length", str(len(response)))
        self.end_headers()
        self.wfile.write(response)

    def _send_json(self, status_code, body):
        response = json.dumps(body, ensure_ascii=False).encode("utf-8")
        self.send_response(status_code)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(response)))
        self.end_headers()
        self.wfile.write(response)

    def _send_static_file(self, file_name, content_type):
        file_path = STATIC_DIR / file_name
        if not file_path.exists():
            self._send_json(404, {"error": "Arquivo nao encontrado"})
            return

        self._send_text(200, file_path.read_text(encoding="utf-8"), content_type)

    def do_GET(self):
        path = urlparse(self.path).path

        if path in ("/", "/index.html"):
            self._send_static_file("index.html", "text/html")
            return

        if path == "/styles.css":
            self._send_static_file("styles.css", "text/css")
            return

        if path == "/app.js":
            self._send_static_file("app.js", "application/javascript")
            return

        if path == "/health":
            self._send_json(200, {"status": "ok", "service": "vitals-api"})
            return

        if path == "/vitals":
            self._send_json(200, {"count": len(VITALS), "items": VITALS})
            return

        self._send_json(404, {"error": "Endpoint nao encontrado"})

    def do_POST(self):
        path = urlparse(self.path).path

        if path != "/vitals":
            self._send_json(404, {"error": "Endpoint nao encontrado"})
            return

        try:
            content_length = int(self.headers.get("Content-Length", 0))
            raw_body = self.rfile.read(content_length).decode("utf-8")
            payload = json.loads(raw_body)
        except json.JSONDecodeError:
            self._send_json(400, {"error": "JSON invalido"})
            return

        errors = validate_vitals(payload)
        if errors:
            self._send_json(422, {"error": "Dados invalidos", "details": errors})
            return

        vital_record = {
            "id": len(VITALS) + 1,
            "received_at": utc_now_iso(),
            **payload,
        }

        alerts = check_risks(vital_record)
        vital_record["alerts"] = alerts
        VITALS.append(vital_record)

        if alerts:
            subject = f"Alerta de risco - {vital_record['patient_name']}"
            send_alert_email(ALERT_RECIPIENT, subject, vital_record, alerts)

        self._send_json(201, vital_record)

    def log_message(self, format, *args):
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        print(f"[{timestamp}] {self.address_string()} - {format % args}")


def run_server():
    server = ThreadingHTTPServer((HOST, PORT), VitalsHandler)
    print(f"API REST iniciada em http://{HOST}:{PORT}")
    print("Pressione Ctrl+C para encerrar.")
    server.serve_forever()


if __name__ == "__main__":
    run_server()
