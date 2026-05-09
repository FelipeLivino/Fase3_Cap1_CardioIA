const form = document.querySelector("#vitalsForm");
const formStatus = document.querySelector("#formStatus");
const recordsList = document.querySelector("#recordsList");
const refreshButton = document.querySelector("#refreshButton");
const totalRecords = document.querySelector("#totalRecords");
const totalAlerts = document.querySelector("#totalAlerts");

function setStatus(message, type = "") {
  formStatus.textContent = message;
  formStatus.className = `form-status ${type}`.trim();
}

function formatDate(value) {
  return new Intl.DateTimeFormat("pt-BR", {
    dateStyle: "short",
    timeStyle: "medium",
  }).format(new Date(value));
}

function renderRecords(records) {
  totalRecords.textContent = records.length;
  totalAlerts.textContent = records.filter((record) => record.alerts?.length).length;

  if (!records.length) {
    recordsList.innerHTML = '<p class="empty-state">Nenhuma leitura recebida ainda.</p>';
    return;
  }

  recordsList.innerHTML = records
    .slice()
    .reverse()
    .map((record) => {
      const alerts = record.alerts || [];
      const badge = alerts.length
        ? `<span class="badge alert">${alerts.length} alerta(s)</span>`
        : '<span class="badge ok">Sem alerta</span>';
      const alertItems = alerts.length
        ? `<ul class="alerts-list">${alerts
            .map((alert) => `<li>${alert.message}</li>`)
            .join("")}</ul>`
        : "";

      return `
        <article class="record-card ${alerts.length ? "has-alert" : ""}">
          <div class="record-topline">
            <div>
              <div class="patient-name">${record.patient_name}</div>
              <div class="record-id">${record.patient_id} · #${record.id} · ${formatDate(record.received_at)}</div>
            </div>
            ${badge}
          </div>
          <div class="vitals-grid">
            <div class="vital-item">
              <span>Frequencia</span>
              <strong>${record.heart_rate} bpm</strong>
            </div>
            <div class="vital-item">
              <span>Temperatura</span>
              <strong>${Number(record.temperature).toFixed(1)} C</strong>
            </div>
            <div class="vital-item">
              <span>Movimento</span>
              <strong>${record.movement_detected ? "Sim" : "Nao"}</strong>
            </div>
          </div>
          ${alertItems}
        </article>
      `;
    })
    .join("");
}

async function loadRecords() {
  const response = await fetch("/vitals");
  const data = await response.json();
  renderRecords(data.items || []);
}

form.addEventListener("submit", async (event) => {
  event.preventDefault();
  setStatus("Enviando leitura...");
  form.querySelector("button").disabled = true;

  const data = new FormData(form);
  const payload = {
    patient_id: data.get("patient_id").trim(),
    patient_name: data.get("patient_name").trim(),
    heart_rate: Number(data.get("heart_rate")),
    temperature: Number(data.get("temperature")),
    movement_detected: data.get("movement_detected") === "on",
  };

  try {
    const response = await fetch("/vitals", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload),
    });
    const result = await response.json();

    if (!response.ok) {
      throw new Error(result.details?.join(", ") || result.error || "Erro ao enviar");
    }

    const alertCount = result.alerts?.length || 0;
    setStatus(
      alertCount ? "Leitura enviada e alerta disparado." : "Leitura enviada sem alerta.",
      alertCount ? "error" : "success"
    );
    await loadRecords();
  } catch (error) {
    setStatus(error.message, "error");
  } finally {
    form.querySelector("button").disabled = false;
  }
});

refreshButton.addEventListener("click", loadRecords);
loadRecords().catch(() => setStatus("Nao foi possivel carregar os registros.", "error"));
