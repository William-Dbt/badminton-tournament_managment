FROM node:18

RUN apt-get update && apt-get install -y postgresql-client

WORKDIR /app

COPY package*.json package-lock.json ./

RUN npm install

COPY . .

RUN chmod +x script_init.sh

CMD [ "./script_init.sh" ]
