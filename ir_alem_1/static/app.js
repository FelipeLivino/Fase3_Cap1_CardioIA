const { useEffect, useState } = React;

const INITIAL_FORM = {
  patient_id: "PAC004",
  patient_name: "Daniel Rocha",
  heart_rate: "86",
  temperature: "36.8",
  movement_detected: true,
};

function formatDate(value) {
  return new Intl.DateTimeFormat("pt-BR", {
    dateStyle: "short",
    timeStyle: "medium",
  }).format(new Date(value));
}

function StatusMessage({ status }) {
  return React.createElement(
    "p",
    {
      className: `form-status ${status.type}`.trim(),
      role: "status",
    },
    status.message
  );
}

function SummaryRow({ records }) {
  const totalAlerts = records.filter((record) => record.alerts?.length).length;

  return React.createElement(
    "div",
    { className: "summary-row" },
    React.createElement(
      "div",
      null,
      React.createElement("span", { className: "summary-label" }, "Registros"),
      React.createElement("strong", null, records.length)
    ),
    React.createElement(
      "div",
      null,
      React.createElement("span", { className: "summary-label" }, "Alertas"),
      React.createElement("strong", null, totalAlerts)
    )
  );
}

function RecordCard({ record }) {
  const alerts = record.alerts || [];
  const hasAlerts = alerts.length > 0;

  return React.createElement(
    "article",
    { className: `record-card ${hasAlerts ? "has-alert" : ""}`.trim() },
    React.createElement(
      "div",
      { className: "record-topline" },
      React.createElement(
        "div",
        null,
        React.createElement("div", { className: "patient-name" }, record.patient_name),
        React.createElement(
          "div",
          { className: "record-id" },
          `${record.patient_id} · #${record.id} · ${formatDate(record.received_at)}`
        )
      ),
      React.createElement(
        "span",
        { className: `badge ${hasAlerts ? "alert" : "ok"}` },
        hasAlerts ? `${alerts.length} alerta(s)` : "Sem alerta"
      )
    ),
    React.createElement(
      "div",
      { className: "vitals-grid" },
      React.createElement(
        "div",
        { className: "vital-item" },
        React.createElement("span", null, "Frequencia"),
        React.createElement("strong", null, `${record.heart_rate} bpm`)
      ),
      React.createElement(
        "div",
        { className: "vital-item" },
        React.createElement("span", null, "Temperatura"),
        React.createElement("strong", null, `${Number(record.temperature).toFixed(1)} C`)
      ),
      React.createElement(
        "div",
        { className: "vital-item" },
        React.createElement("span", null, "Movimento"),
        React.createElement("strong", null, record.movement_detected ? "Sim" : "Nao")
      )
    ),
    hasAlerts
      ? React.createElement(
          "ul",
          { className: "alerts-list" },
          alerts.map((alert) => React.createElement("li", { key: alert.type }, alert.message))
        )
      : null
  );
}

function RecordsPanel({ records, onRefresh }) {
  const orderedRecords = [...records].reverse();

  return React.createElement(
    "section",
    { className: "panel received-panel" },
    React.createElement(
      "div",
      { className: "panel-header" },
      React.createElement(
        "div",
        null,
        React.createElement("p", { className: "eyebrow" }, "API REST"),
        React.createElement("h1", null, "Sinais vitais recebidos")
      ),
      React.createElement(
        "button",
        {
          className: "icon-button",
          type: "button",
          title: "Atualizar lista",
          onClick: onRefresh,
        },
        "↻"
      )
    ),
    React.createElement(SummaryRow, { records }),
    React.createElement(
      "div",
      { className: "records-list" },
      orderedRecords.length
        ? orderedRecords.map((record) =>
            React.createElement(RecordCard, { key: record.id, record })
          )
        : React.createElement("p", { className: "empty-state" }, "Nenhuma leitura recebida ainda.")
    )
  );
}

function VitalsForm({ onSubmitRecord }) {
  const [formData, setFormData] = useState(INITIAL_FORM);
  const [isSubmitting, setIsSubmitting] = useState(false);
  const [status, setStatus] = useState({ message: "", type: "" });

  function handleChange(event) {
    const { checked, name, type, value } = event.target;
    setFormData((current) => ({
      ...current,
      [name]: type === "checkbox" ? checked : value,
    }));
  }

  async function handleSubmit(event) {
    event.preventDefault();
    setStatus({ message: "Enviando leitura...", type: "" });
    setIsSubmitting(true);

    const payload = {
      patient_id: formData.patient_id.trim(),
      patient_name: formData.patient_name.trim(),
      heart_rate: Number(formData.heart_rate),
      temperature: Number(formData.temperature),
      movement_detected: formData.movement_detected,
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
      setStatus({
        message: alertCount
          ? "Leitura enviada e alerta disparado."
          : "Leitura enviada sem alerta.",
        type: alertCount ? "error" : "success",
      });
      await onSubmitRecord();
    } catch (error) {
      setStatus({ message: error.message, type: "error" });
    } finally {
      setIsSubmitting(false);
    }
  }

  return React.createElement(
    "section",
    { className: "panel form-panel" },
    React.createElement(
      "div",
      { className: "panel-header" },
      React.createElement(
        "div",
        null,
        React.createElement("p", { className: "eyebrow" }, "Nova leitura"),
        React.createElement("h2", null, "Enviar sinais vitais")
      )
    ),
    React.createElement(
      "form",
      { onSubmit: handleSubmit },
      React.createElement(
        "label",
        null,
        "ID do paciente",
        React.createElement("input", {
          name: "patient_id",
          type: "text",
          value: formData.patient_id,
          onChange: handleChange,
          required: true,
        })
      ),
      React.createElement(
        "label",
        null,
        "Nome do paciente",
        React.createElement("input", {
          name: "patient_name",
          type: "text",
          value: formData.patient_name,
          onChange: handleChange,
          required: true,
        })
      ),
      React.createElement(
        "div",
        { className: "field-grid" },
        React.createElement(
          "label",
          null,
          "Frequencia cardiaca",
          React.createElement("input", {
            name: "heart_rate",
            type: "number",
            min: "1",
            step: "1",
            value: formData.heart_rate,
            onChange: handleChange,
            required: true,
          })
        ),
        React.createElement(
          "label",
          null,
          "Temperatura",
          React.createElement("input", {
            name: "temperature",
            type: "number",
            min: "30",
            max: "45",
            step: "0.1",
            value: formData.temperature,
            onChange: handleChange,
            required: true,
          })
        )
      ),
      React.createElement(
        "label",
        { className: "toggle-row" },
        React.createElement("input", {
          name: "movement_detected",
          type: "checkbox",
          checked: formData.movement_detected,
          onChange: handleChange,
        }),
        React.createElement("span", null, "Movimento detectado")
      ),
      React.createElement(
        "button",
        { className: "primary-button", type: "submit", disabled: isSubmitting },
        "Enviar leitura"
      ),
      React.createElement(StatusMessage, { status })
    )
  );
}

function App() {
  const [records, setRecords] = useState([]);
  const [loadError, setLoadError] = useState("");

  async function loadRecords() {
    setLoadError("");
    try {
      const response = await fetch("/vitals");
      const data = await response.json();
      setRecords(data.items || []);
    } catch (error) {
      setLoadError("Nao foi possivel carregar os registros.");
    }
  }

  useEffect(() => {
    loadRecords();
  }, []);

  return React.createElement(
    "main",
    { className: "app-shell" },
    React.createElement(RecordsPanel, { records, onRefresh: loadRecords }),
    React.createElement(
      React.Fragment,
      null,
      React.createElement(VitalsForm, { onSubmitRecord: loadRecords }),
      loadError ? React.createElement("p", { className: "screen-reader-status" }, loadError) : null
    )
  );
}

ReactDOM.createRoot(document.querySelector("#root")).render(React.createElement(App));
