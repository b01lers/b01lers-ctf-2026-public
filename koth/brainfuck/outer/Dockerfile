FROM alpine:edge
RUN apk add --no-cache python3 py3-pip
RUN pip3 install --break-system-packages websockets zstd
RUN adduser -D runner
COPY run.py /app/run.py
USER runner
EXPOSE 5001
CMD ["python3", "/app/run.py"]
