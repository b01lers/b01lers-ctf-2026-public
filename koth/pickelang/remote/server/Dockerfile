FROM python:3.12-alpine
WORKDIR /app
RUN apk add --no-cache netcat-openbsd
RUN pip install websockets backports.zstd
COPY server.py /app/server.py
EXPOSE 5000
CMD ["python3", "-u", "server.py"]
