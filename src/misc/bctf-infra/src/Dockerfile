FROM python:3.13-slim-trixie@sha256:eefe082c4b73082d83b8e7705ed999bc8a1dae57fe1ea723f907a0fc4b90f088  AS app

RUN apt-get update && \
	apt-get install -y netcat-openbsd && \
	rm -rf /var/lib/apt/lists/*

FROM debian:trixie-slim AS jail

RUN apt-get -y update && apt-get install -y \
	autoconf \
	bison \
	flex \
	gcc \
	g++ \
	git \
	libprotobuf-dev \
	libnl-route-3-dev \
	libtool \
	make \
	pkg-config \
	protobuf-compiler \
	socat \
	&& rm -rf /var/lib/apt/lists/*


RUN mkdir /nsjail && cd /nsjail && \
	git init && \
	git remote add origin https://github.com/google/nsjail && \
	git fetch --depth 1 origin bcf9eca73843fccbf48d063709adde3554a1e197 && \
	git reset --hard FETCH_HEAD

RUN cd /nsjail && make && mv /nsjail/nsjail /bin && rm -rf -- /nsjail

RUN apt-get update && \
	apt-get install -y socat && \
	rm -rf /var/lib/apt/lists/*

COPY --from=app / /srv

COPY app /srv/app


COPY nsjail.cfg /nsjail.cfg
COPY entrypoint.sh /entrypoint.sh

RUN echo "ctf:x:1000:1000::/:/bin/sh" >> /srv/etc/passwd && echo "ctf:x:1000:ctf" >> /srv/etc/group

RUN <<EOF
uid=65001
for folder in $(find /srv/app/chals -mindepth 1 -maxdepth 1); do
	user="$(basename "$folder")"
	groupadd -g "$uid" "$user"
	useradd -g "$uid" -u "$uid" "$user"
	echo "$user:x:$uid:$uid::/:/bin/sh" >> /srv/etc/passwd
	echo "$user:x:$uid:$user" >> /srv/etc/group
	chown -R "$user:$user" "$folder"
	chmod 700 "$folder"
	uid=$((uid+1))
done
EOF

ENTRYPOINT ["/entrypoint.sh"]
CMD ["socat", "TCP-LISTEN:1337,reuseaddr,fork", "EXEC:nsjail --config /nsjail.cfg -- /app/run.sh"]