FROM python:3.12-slim-bookworm AS app

RUN pip install --break-system-packages --no-cache-dir "numpy<3" "qiskit>=1,<2"

FROM pwn.red/jail

COPY --from=app / /srv

COPY ./src/challenge.py /srv/app/run

RUN chmod 775 /srv/app/run

ENV JAIL_TIME=600 \
	JAIL_MEM=1024M \
	JAIL_PORT=4447 \
	JAIL_PIDS=64 \
	JAIL_TMP_SIZE=64M \
	JAIL_ENV_PYTHONUNBUFFERED=1 \
	JAIL_ENV_TMPDIR=/tmp \
	JAIL_ENV_TMP=/tmp \
	JAIL_ENV_TEMP=/tmp \
	JAIL_ENV_OPENBLAS_NUM_THREADS=1 \
	JAIL_ENV_OMP_NUM_THREADS=1 \
	JAIL_ENV_MKL_NUM_THREADS=1 \
	JAIL_ENV_NUMEXPR_NUM_THREADS=1 \
	JAIL_ENV_RAYON_NUM_THREADS=1 \
	JAIL_ENV_QISKIT_NUM_PROCS=1 \
	JAIL_CPU=1000