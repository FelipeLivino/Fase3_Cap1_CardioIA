RISK_RULES = {
    "tachycardia_bpm": 120,
    "fever_celsius": 38.0,
}


def check_risks(vital_record):
    alerts = []

    heart_rate = vital_record.get("heart_rate")
    if heart_rate is not None and heart_rate > RISK_RULES["tachycardia_bpm"]:
        alerts.append(
            {
                "type": "TAQUICARDIA",
                "message": f"Frequencia cardiaca elevada: {heart_rate} bpm",
            }
        )

    temperature = vital_record.get("temperature")
    if temperature is not None and temperature >= RISK_RULES["fever_celsius"]:
        alerts.append(
            {
                "type": "FEBRE",
                "message": f"Temperatura elevada: {temperature:.1f} C",
            }
        )

    if vital_record.get("movement_detected") is False:
        alerts.append(
            {
                "type": "AUSENCIA_DE_MOVIMENTO",
                "message": "Paciente sem movimento detectado",
            }
        )

    return alerts


def has_risk(vital_record):
    return len(check_risks(vital_record)) > 0
