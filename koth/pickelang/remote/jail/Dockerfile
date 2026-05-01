FROM python:3.13.5@sha256:4ea77121eab13d9e71f2783d7505f5655b25bb7b2c263e8020aae3b555dbc0b2 AS app

FROM pwn.red/jail
COPY --from=app / /srv
COPY --chmod=755 pickelang.py /srv/app/run
ENV JAIL_MEM=100M JAIL_TIME=241 JAIL_PIDS=10 JAIL_CPU=250 JAIL_PORT=6969
