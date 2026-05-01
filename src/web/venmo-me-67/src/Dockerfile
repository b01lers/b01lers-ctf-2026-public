FROM python:3.12-slim

ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1
ENV PORT=5000
ENV SPLIT_MODEL=gemini-2.5-flash-lite
ENV GOOGLE_APPLICATION_CREDENTIALS=/app/adc-cred-real.json
ENV HOME=/home/appuser

WORKDIR /app

RUN apt-get update \
    && apt-get install --no-install-recommends -y sox libsox-fmt-all \
    && rm -rf /var/lib/apt/lists/*

RUN addgroup --system appgroup \
    && adduser --system --ingroup appgroup --home /home/appuser appuser \
    && mkdir -p /home/appuser \
    && chown -R appuser:appgroup /home/appuser

COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY app.py utils.py ./
COPY adc-cred-real.json adc-cred-real.json ./
COPY flag.txt ./
COPY templates ./templates
COPY static ./static

RUN chown -R appuser:appgroup /app

USER appuser

EXPOSE 5000

CMD ["gunicorn", "--bind", "0.0.0.0:5000", "--workers", "1", "--threads", "8", "--timeout", "120", "--access-logfile", "-", "--error-logfile", "-", "app:app"]
