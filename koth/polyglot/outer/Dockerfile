FROM python:3.14-alpine3.23

RUN pip install --no-cache-dir websockets

COPY run.py /app/run.py

WORKDIR /app
EXPOSE 5000

RUN adduser -D -h /app runner
USER runner

CMD ["python3", "-u", "/app/run.py"]