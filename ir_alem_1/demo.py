import time

from rest_client import RestClientError
from rest_client import get_vitals, health_check, send_vitals


SAMPLE_VITALS = [
    {
        "patient_id": "PAC001",
        "patient_name": "Ana Souza",
        "heart_rate": 82,
        "temperature": 36.7,
        "movement_detected": True,
    },
    {
        "patient_id": "PAC002",
        "patient_name": "Bruno Lima",
        "heart_rate": 132,
        "temperature": 37.2,
        "movement_detected": True,
    },
    {
        "patient_id": "PAC003",
        "patient_name": "Carla Mendes",
        "heart_rate": 118,
        "temperature": 38.6,
        "movement_detected": False,
    },
]


def wait_for_api(max_attempts=10, delay_seconds=1):
    for attempt in range(1, max_attempts + 1):
        try:
            return health_check()
        except RestClientError:
            if attempt == max_attempts:
                raise
            print("API ainda nao respondeu. Tentando novamente...")
            time.sleep(delay_seconds)


def process_vitals(vitals):
    created_record = send_vitals(vitals)
    alerts = created_record.get("alerts", [])

    if alerts:
        print(f"Paciente {created_record['patient_name']}: alerta enviado pela API.")
    else:
        print(f"Paciente {created_record['patient_name']}: leitura sem alerta.")

    return created_record, alerts


def main():
    print("Validando conexao com API...")
    print(wait_for_api())

    print("\nEnviando leituras de sinais vitais...")
    for vitals in SAMPLE_VITALS:
        record, alerts = process_vitals(vitals)
        print(f"Registro #{record['id']} processado com {len(alerts)} alerta(s).")

    print("\nConsultando dados recebidos pela API...")
    response = get_vitals()
    print(f"Total de leituras armazenadas: {response['count']}")


if __name__ == "__main__":
    main()
