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
