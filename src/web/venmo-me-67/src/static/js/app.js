(() => {
  const root = document.body;
  if (!root) {
    return;
  }

  const page = root.dataset.page || "";
  const activeTokenStorageKey = root.dataset.activeTokenStorageKey || "split-the-bill-active-token";
  const pollIntervalMs = Number(root.dataset.pollIntervalMs || 3000);

  function resolveQrImageSrc(value) {
    if (typeof value !== "string") {
      return null;
    }

    const cleaned = value.trim();
    if (!cleaned) {
      return null;
    }

    if (cleaned.startsWith("data:image/")) {
      return cleaned;
    }

    const base64Payload = cleaned.includes("base64,")
      ? cleaned.split("base64,").pop()
      : cleaned.replace(/\s+/g, "");

    return base64Payload ? `data:image/png;base64,${base64Payload}` : null;
  }

  function renderRows(node, rows, emptyText) {
    node.innerHTML = "";

    if (!rows.length) {
      node.innerHTML = `<p>${emptyText}</p>`;
      return;
    }

    rows.forEach((row) => {
      const item = document.createElement("div");
      item.className = "row";

      const label = document.createElement("strong");
      label.textContent = row.label;

      const value = document.createElement("span");
      value.textContent = row.value;

      item.appendChild(label);
      item.appendChild(value);
      node.appendChild(item);
    });
  }

  function initIndexPage() {
    const form = document.getElementById("upload-form");
    const submitButton = document.getElementById("submit-button");
    const submitStatus = document.getElementById("submit-status");

    if (!form || !submitButton || !submitStatus) {
      return;
    }

    form.addEventListener("submit", async (event) => {
      event.preventDefault();
      submitStatus.textContent = "Uploading your files...";
      submitStatus.style.color = "";
      submitButton.disabled = true;

      try {
        const response = await fetch("/process", {
          method: "POST",
          body: new FormData(form),
        });
        const payload = await response.json();

        if (!response.ok) {
          throw new Error(payload.error || "Unable to queue the bill split.");
        }

        localStorage.setItem(activeTokenStorageKey, payload.token);
        submitStatus.textContent = "Redirecting to your live status...";
        window.location.assign("/status");
      } catch (error) {
        submitStatus.textContent = error.message || "Something went wrong while submitting.";
        submitStatus.style.color = "var(--danger)";
        submitButton.disabled = false;
      }
    });
  }

  function initStatusPage() {
    const tokenValue = document.getElementById("token-value");
    const statusValue = document.getElementById("status-value");
    const updatedValue = document.getElementById("updated-value");
    const statusBadge = document.getElementById("status-badge");
    const statusMessage = document.getElementById("status-message");
    const errorBanner = document.getElementById("error-banner");
    const splitResults = document.getElementById("split-results");
    const warningsList = document.getElementById("warnings-list");
    const qrResults = document.getElementById("qr-results");
    const clearTokenButton = document.getElementById("clear-token-button");

    if (
      !tokenValue ||
      !statusValue ||
      !updatedValue ||
      !statusBadge ||
      !statusMessage ||
      !errorBanner ||
      !splitResults ||
      !warningsList ||
      !qrResults ||
      !clearTokenButton
    ) {
      return;
    }

    let pollTimerId = null;

    function showError(text) {
      errorBanner.textContent = text || "";
      errorBanner.classList.toggle("show", Boolean(text));
    }

    function stamp() {
      return new Intl.DateTimeFormat(undefined, {
        dateStyle: "medium",
        timeStyle: "medium",
      }).format(new Date());
    }

    function setStatus(status, message) {
      const badgeClass =
        status === "error"
          ? "badge error"
          : status === "queued" || status === "running"
            ? "badge warning"
            : "badge";

      statusValue.textContent = status;
      updatedValue.textContent = stamp();
      statusBadge.textContent = status;
      statusBadge.className = badgeClass;
      statusMessage.textContent = message;
    }

    function stopPolling() {
      if (pollTimerId !== null) {
        window.clearInterval(pollTimerId);
        pollTimerId = null;
      }
    }

    function startPolling() {
      if (pollTimerId === null) {
        pollTimerId = window.setInterval(pollStatus, pollIntervalMs);
      }
    }

    function renderQr(venmo) {
      qrResults.innerHTML = "";

      const entries = Object.entries(venmo || {}).filter(([, entry]) => Boolean(entry));
      if (!entries.length) {
        qrResults.innerHTML = "<p>No QR codes are available for this job.</p>";
        return 0;
      }

      const debtorStrip = document.createElement("div");
      debtorStrip.className = "debtor-strip";

      const preview = document.createElement("article");
      preview.className = "qr-focus";

      const chips = [];
      let selectedIndex = 0;

      function renderPreview() {
        const [person, entry] = entries[selectedIndex];
        const amount = Number(entry.amount ?? 0);
        const qrSrc = resolveQrImageSrc(entry.qr_png_base64 || "");

        chips.forEach((chip, index) => {
          chip.classList.toggle("active", index === selectedIndex);
        });

        preview.innerHTML = "";

        const title = document.createElement("strong");
        title.textContent = `${person} owes $${amount.toFixed(2)}`;
        preview.appendChild(title);

        if (qrSrc) {
          const image = document.createElement("img");
          image.alt = `Venmo QR code for ${person}`;
          image.src = qrSrc;
          preview.appendChild(image);
          return;
        }

        const message = document.createElement("p");
        message.textContent = entry.error || "No QR code is available for this entry.";
        preview.appendChild(message);
      }

      entries.forEach(([person], index) => {
        const chip = document.createElement("button");
        chip.type = "button";
        chip.className = "debtor-chip";
        chip.textContent = person;
        chip.addEventListener("click", () => {
          selectedIndex = index;
          renderPreview();
        });
        debtorStrip.appendChild(chip);
        chips.push(chip);
      });

      qrResults.appendChild(debtorStrip);
      qrResults.appendChild(preview);
      renderPreview();

      return entries.filter(([, entry]) => Boolean(resolveQrImageSrc(entry.qr_png_base64 || ""))).length;
    }

    function renderCompleted(payload) {
      const result = payload.result || {};
      const split = result.split || {};
      const payer = split.payer || "unknown-payer";

      renderRows(
        splitResults,
        Object.entries(split.owes || {}).map(([person, amount]) => ({
          label: person,
          value: `$${Number(amount).toFixed(2)}`,
        })),
        "The split completed, but no balances were due."
      );

      renderRows(
        warningsList,
        (split.allocation_warnings || []).map((warning, index) => ({
          label: `Warning ${index + 1}`,
          value: warning,
        })),
        "No warnings were returned."
      );

      const qrCount = renderQr(result.venmo || {});
      setStatus(
        "completed",
        qrCount > 0
          ? `Split complete for ${payer}. ${qrCount} QR code(s) are ready to share.`
          : `Split complete for ${payer}. No QR code was generated for this job.`
      );
    }

    async function pollStatus() {
      const activeToken = localStorage.getItem(activeTokenStorageKey);
      tokenValue.textContent = activeToken || "No token saved";

      if (!activeToken) {
        setStatus("idle", "No active split yet. Start a new upload from the home page.");
        renderRows(splitResults, [], "No active split is available.");
        renderRows(warningsList, [], "No warnings yet.");
        qrResults.innerHTML = "<p>Payment QR codes will appear here when your split is ready.</p>";
        showError("");
        return;
      }

      try {
        const response = await fetch(`/check-result?token=${encodeURIComponent(activeToken)}`);
        const payload = await response.json();

        if (!response.ok) {
          throw new Error(payload.error || "Unable to load job status.");
        }

        showError("");

        if (payload.status === "queued") {
          setStatus("queued", "Upload received. Your split is waiting to start.");
          return;
        }

        if (payload.status === "running") {
          setStatus("running", "Working on your split now.");
          return;
        }

        if (payload.status === "completed") {
          renderCompleted(payload);
          stopPolling();
          return;
        }

        if (payload.status === "error") {
          setStatus("error", payload.error || "The job reported an error.");
          showError(payload.error || "The backend reported an unknown error.");
          return;
        }

        setStatus(payload.status || "unknown", "Received an unrecognized status from the backend.");
      } catch (error) {
        setStatus("error", "Could not refresh your split status. Retrying...");
        showError(error.message || "Unable to load the latest status.");
      }
    }

    clearTokenButton.addEventListener("click", () => {
      localStorage.removeItem(activeTokenStorageKey);
      startPolling();
      pollStatus();
    });

    pollStatus();
    startPolling();
  }

  if (page === "index") {
    initIndexPage();
    return;
  }

  if (page === "status") {
    initStatusPage();
  }
})();
