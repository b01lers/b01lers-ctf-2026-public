FROM node:22-alpine

RUN apk add --no-cache \
    udev \
    ttf-freefont \
    chromium

ENV PUPPETEER_EXECUTABLE_PATH=/usr/bin/chromium-browser

ENV USER gas-giant
WORKDIR /home/$USER

COPY . /home/$USER/

EXPOSE 3000

RUN cd /home/$USER && npm ci && npm run build

CMD ["npm", "start"]
