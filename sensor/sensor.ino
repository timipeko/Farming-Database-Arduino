#include <SPI.h>
#include <WiFi.h>
#include "DHT.h"

// Settings for this sensor.

// The (hopefully) unique sensor ID.
const int sensor_id = 5; 

// WiFi network name and password.
char ssid[] = "EntoCube"; 
char pass[] = "MobileProtein!";

// IP address and port number of the web server where we send data.
unsigned char ip[4] = { 192, 168, 1, 187 };
const int port = 8000;

// The amount of time to wait before sending in a sensor reading again, in milliseconds.
const unsigned long postingInterval = 30L * 1000L; 

// Practical details.

int status = WL_IDLE_STATUS;
WiFiClient client;
IPAddress server(ip[0], ip[1], ip[2], ip[3]);
unsigned long lastConnectionTime = 0; // in milliseconds

#define DHTPIN 2  // what pin we're connected to
#define DHTTYPE DHT22 // DHT 22  (AM2302)

// The temperature and humidity sensor interface.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  // you're connected now, so print out the status:
  printWifiStatus();

  // Also, remember to start the sensor.
  dht.begin();
}

void loop() {
  // If there's incoming data from the net connection send it out the serial port.  
  // This is for debugging purposes only.
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C ");
    
    sendData(sensor_id, t, h, 0);
  }
}

// This method sends the sensor data to the server.
void sendData(int id, float temp, float humidity, int status_code) {
  // Close any connection before we send a new request. This will free the socket on the WiFi shield.
  client.stop();

  // If there is a successful connection:
  if (client.connect(server, port)) {
    // We just send the POST request directly since we know the form it should have. Ugly but works.
    // First we form the content string.
    String content = "sensor_id=";
    content.concat(id);
    content.concat("&temp=");
    content.concat(temp);
    content.concat("&humidity=");
    content.concat(humidity);
    content.concat("&status_code=");
    content.concat(status_code);
    
    Serial.println("Sending data...\n");
    // Then we send the content.
    client.println("POST / HTTP/1.1");
    client.println("Host: " + String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]) + ":" + String(port));
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(content.length());
    client.println();
    client.println(content);

    // Note the time that the connection was made.
    lastConnectionTime = millis();
  } else {
    Serial.println("Failed to send data...");
  }
}

void printWifiStatus() {
  // Print the SSID of the network you're attached to.
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print your WiFi shield's IP address.
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Print the received signal strength.
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

