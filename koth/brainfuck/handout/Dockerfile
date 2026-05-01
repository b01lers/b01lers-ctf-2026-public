FROM alpine:edge AS builder
RUN apk add --no-cache build-base git
RUN git clone https://github.com/primo-ppcg/bfci.git /bfci
WORKDIR /bfci
RUN cc -Wall -Wextra -O3 -s -static -o bfci bfci.c src/*.c

FROM python:3.14.3-alpine
COPY --from=builder /bfci/bfci /usr/local/bin/bfci
COPY runner.py /usr/local/bin/runner
RUN chmod +x /usr/local/bin/runner
WORKDIR /work
ENTRYPOINT ["runner"]
