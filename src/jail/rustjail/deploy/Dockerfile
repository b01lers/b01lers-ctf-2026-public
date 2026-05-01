# Standard debian instance, pin specific version to avoid breaking libc changes and such
FROM --platform=linux/amd64 rust:1.90 AS safe_rustjail_inner_container

# Install any software to run the challenge
# Build tools should be installed in Dockerfile_build

RUN apt-get update && \
apt-get install -y python3

RUN mkdir -p /rustup_home
ENV RUSTUP_HOME=/rustup_home
RUN rustup toolchain install nightly-2025-09-25

# wrapper.sh wraps the executable by
# performing any necessary setup
# this has to be called /app/run for redpwn jail to work
COPY ./src/ /app/src/
COPY ./src/wrapper.py /app/run
RUN chmod +x /app/run

# We don't want to forget the flag!
COPY ./flag.txt /app/src/flag.txt
# Make flags at random location
RUN mv /app/src/flag.txt /app/src/flag_$(cat /dev/urandom | tr -cd 'a-f0-9' | head -c 32).txt

# this container runs the redpwn jail
FROM pwn.red/jail

# copy challenge container files to /srv
# /srv will be all files in the jail
COPY --from=safe_rustjail_inner_container / /srv

# redpwn jail options can be found at https://github.com/redpwn/jail?tab=readme-ov-file#configuration-reference
# some example options are set below

# configures allowed memory in redpwn jail
ENV JAIL_MEM=100M
# configures time before chall is killed
ENV JAIL_TIME=120
# use specified port to run challenge
ENV JAIL_PORT=6969
ENV JAIL_PIDS=32
# empty main.rs about 8.6 megabyte target folder
ENV JAIL_TMP_SIZE=20M
