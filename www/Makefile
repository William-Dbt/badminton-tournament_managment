start:
	docker compose up --build -d

stop:
	docker compose down

restart: stop start

clean:
	docker system prune -af --volumes
