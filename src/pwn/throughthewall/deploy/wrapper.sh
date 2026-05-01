#!/bin/sh
# Not used in production (Dockerfile CMD runs socat directly).
# For local testing outside Docker:
#   socat TCP-LISTEN:7352,reuseaddr,fork EXEC:./start_prod.sh,pty,setsid,sigint,sane
exec /app/start.sh
