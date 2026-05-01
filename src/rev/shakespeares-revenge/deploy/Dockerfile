FROM ubuntu:22.04 AS base

RUN apt-get update && \
apt-get install -y socat python3

# wrapper.sh wraps the executable by
# performing any necessary setup
# this has to be called /app/run for redpwn jail to work
COPY ./dist/server.py /app/run
RUN chmod +x /app/run

COPY ./dist/shakespeare /app/shakespeare
RUN chmod +x /app/shakespeare

COPY ./flag.txt /app/flag.txt

COPY ./dist/challenge.spl /app/challenge.spl

COPY ./deploy/insults.txt /app/insults.txt

# this container runs the redpwn jail
FROM pwn.red/jail

# copy challenge container files to /srv
# /srv will be all files in the jail
COPY --from=base / /srv

# configures allowed memory in redpwn jail
ENV JAIL_MEM=100M
# configures time before chall is killed
ENV JAIL_TIME=1800
# use specified port to run challenge
ENV JAIL_PORT=3001