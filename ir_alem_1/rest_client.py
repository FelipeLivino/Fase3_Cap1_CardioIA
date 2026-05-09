from urllib import request, error
import json
import os


BASE_URL = os.getenv("VITALS_API_BASE_URL", "http://127.0.0.1:8000").rstrip("/")


class RestClientError(Exception):
    pass


def _request_json(method, path, payload=None):
    url = f"{BASE_URL}{path}"
    data = None
    headers = {"Accept": "application/json"}

    if payload is not None:
        data = json.dumps(payload).encode("utf-8")
        headers["Content-Type"] = "application/json"

    req = request.Request(url, data=data, headers=headers, method=method)

    try:
        with request.urlopen(req, timeout=5) as response:
            return json.loads(response.read().decode("utf-8"))
    except error.HTTPError as exc:
        details = exc.read().decode("utf-8")
        raise RestClientError(f"Erro HTTP {exc.code}: {details}") from exc
    except error.URLError as exc:
        raise RestClientError(
            f"Nao foi possivel conectar na API em {BASE_URL}. "
            "Verifique se api_server.py esta em execucao."
        ) from exc


def send_vitals(vitals):
    return _request_json("POST", "/vitals", vitals)


def get_vitals():
    return _request_json("GET", "/vitals")


def health_check():
    return _request_json("GET", "/health")


if __name__ == "__main__":
    sample = {
        "patient_id": "PAC-DEMO",
        "patient_name": "Paciente Demo",
        "heart_rate": 124,
        "temperature": 38.2,
        "movement_detected": True,
    }

    print("Health check:", health_check())
    print("Enviando leitura:", send_vitals(sample))
    print("Leituras cadastradas:", get_vitals())
