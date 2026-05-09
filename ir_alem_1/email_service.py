from datetime import datetime
from pathlib import Path


OUTBOX_DIR = Path("outbox")
OUTBOX_FILE = OUTBOX_DIR / "alertas.log"


def send_alert_email(to_address, subject, patient, alerts):
    OUTBOX_DIR.mkdir(exist_ok=True)

    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    alert_lines = "\n".join(f"- {alert['type']}: {alert['message']}" for alert in alerts)
    body = (
        f"Data/hora: {timestamp}\n"
        f"Para: {to_address}\n"
        f"Assunto: {subject}\n"
        f"Paciente: {patient.get('patient_name')} ({patient.get('patient_id')})\n"
        f"Leitura registrada: FC={patient.get('heart_rate')} bpm, "
        f"Temp={patient.get('temperature')} C, "
        f"Movimento={patient.get('movement_detected')}\n"
        "Alertas:\n"
        f"{alert_lines}\n"
        f"{'-' * 60}\n"
    )

    with OUTBOX_FILE.open("a", encoding="utf-8") as output:
        output.write(body)

    print("E-mail de alerta simulado.")
    print(f"Destino: {to_address}")
    print(f"Assunto: {subject}")
    print(f"Registro salvo em: {OUTBOX_FILE}")
