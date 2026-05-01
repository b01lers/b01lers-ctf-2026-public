FROM --platform=linux/amd64 debian:trixie AS base

RUN apt-get update && apt-get install -y wget xz-utils socat
RUN wget https://downloadmirror.intel.com/915934/sde-external-10.8.0-2026-03-15-lin.tar.xz -O /root/sde.tar.xz
RUN tar -xf /root/sde.tar.xz -C /root/
RUN mkdir /app && mv /root/sde-external-10.8.0-2026-03-15-lin /app/sde

COPY ./flag.txt /app/flag.txt
COPY ./dist/chall /app/chall
# RUN mkdir /etc/sysctl.d && echo kernel.yama.ptrace_scope = 0 > /etc/sysctl.d/10-ptrace.conf


WORKDIR /app
CMD socat -T60 TCP-LISTEN:5000,reuseaddr,fork EXEC:'/app/sde/sde64 -spr -- /app/chall'
