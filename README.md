Simple odds calculator for football events.

Building on Ubuntu 25.10

Prerequisites:
sudo apt update
sudo apt install build-essential
sudo apt install libpq-dev
sudo apt install libpqxx-dev

Compile and link:
g++ -std=c++20 -Iinclude src/*.cpp -o odds -lpqxx -lpq

PostgreSQL:
sudo apt install postgresql postgresql-contrib
sudo systemctl enable postgresql
sudo systemctl start postgresql
sudo systemctl status postgresql
sudo chown postgres:postgres /home/yourname/odds/odds.sql
sudo -i -u postgres
createdb odds
psql -d odds -f /home/yourname/odds/odds.sql

within the psql odds db:
ALTER USER postgres PASSWORD 'mypassword';


Example use:
./odds --conv --league "premier_league" --season "2023-2024"
./odds --semi --league "premier_league" --sd "2011-01-01" --ed "2012-12-31"
./odds --line --league "ligue_1" --sd "2011-01-01" --ed "2012-12-31" --sc 2.0 1.0 1.5 3.0
