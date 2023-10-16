#!/bin/sh

set -e

until pg_isready -h postgres -p 5432 -U postgres; do
  echo "$(date) - waiting for database to start"
  sleep 1
done

echo "$(date) - Database is up - executing command"
exec "$@"

npx prisma db push
npx prisma generate
(npx prisma studio&)
npm run start:dev
