# Farming-Database-Arduino
An Arduino/AMP/Django-based stack for remote monitoring in insect farming

Problems:
 1. WiFi name and password need to be set manually 
   -> Probably no way around this
 2. Web server IP needs to be set manually 
   -> DNS is the easy answer, but also has downsides
 3. The POST request has to have a specific form for it to work 
   -> No real way around this I think
 4. Server is not production grade as they say, and if it dies we need to restart it
 5. Sensor ID needs to be set manually
   -> Perhaps there is some constant which is different in each arduino which could be used in its place
 6. Posting interval needs to be set manually
   -> This could be read online
 7. Time zones are handled poorly

http://www.instructables.com/id/How-to-Update-Your-Arduino-Wi-Fi-Shield-not-101/
https://www.digitalocean.com/community/tutorials/how-to-use-mysql-or-mariadb-with-your-django-application-on-ubuntu-14-04
http://cactus.io/hookups/sensors/temperature-humidity/am2302/hookup-arduino-to-am2302-temp-humidity-sensor

python manage.py runserver 192.168.1.47:8000

Failure mode notes:
 1. Sensor broken in some way -> blink to show error
 2. No WiFi connection -> retry until succeeds, blink light to show the problem
 3. No connection to server -> retry until succeeds, blink light to show the problem
 4. Server somehow broken -> restart until it works :^)

MySql:

 mysql -u root
 CREATE DATABASE sd CHARACTER SET UTF8;
 CREATE USER t@localhost IDENTIFIED BY 'temporary';
 GRANT ALL PRIVILEGES ON sensordata.* TO t@localhost;
 FLUSH PRIVILEGES;
 exit

Then makemigrations and migrate, and create a superuser (admin - admin@example.com - temporary)
