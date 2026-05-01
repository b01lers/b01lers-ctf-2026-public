FROM --platform=linux/amd64 debian@sha256:f807f4b16002c623115b0247dca6a55711c6b1ae821dc64fb8a2339e4ce2115d AS build

RUN apt-get update && \
apt-get install -y gcc

ENV USER priority-queue
WORKDIR /home/$USER
RUN useradd $USER

COPY ./chall.c .
RUN gcc -no-pie -o chall chall.c

RUN mkdir /out
RUN cp chall /out/chall

FROM --platform=linux/amd64 debian@sha256:f807f4b16002c623115b0247dca6a55711c6b1ae821dc64fb8a2339e4ce2115d AS inner

RUN apt-get update && \
apt-get install -y socat

COPY --from=build /out/chall /app/run
RUN chmod +x /app/run

COPY ./flag.txt /app/flag.txt

FROM pwn.red/jail

COPY --from=inner / /srv

ENV JAIL_MEM=10M
ENV JAIL_TIME=120
ENV JAIL_PORT=1337
