
// For W5100 based ethernet modules
#include <SPI.h>     
#include <Ethernet.h>

// For ENC28J60 based ethernet modules
//#include <UIPEthernet.h>
//#include <UIPServer.h>
//#include <UIPClient.h>

#include <EasyWebServer.h> // Must be included AFTER the ethernet libraries. See comment in EasyWebServer.h.

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) { ; } // wait for serial port to connect. Needed for native USB port only

  // Start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());

  // Set pin 2-8 as digital input pins.
  for (int digitalPin = 2; digitalPin < 8; digitalPin++)
    pinMode(digitalPin,INPUT);
}

void loop() {
  // Listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("New client!");
    EasyWebServer w(client);                    // Read and parse the HTTP Request
    w.serveUrl("/",rootPage);                   // Root page
    w.serveUrl("/analog",analogSensorPage);     // Analog sensor page
    w.serveUrl("/digital",digitalSensorPage);   // Digital sensor page
  }  
}

void rootPage(EasyWebServer &w){
  w.client.println(F("<!DOCTYPE HTML>"));
  w.client.println(F("<html><head><title>EasyWebServer</title></head><body>"));
  w.client.println(F("<p>Welcome to my little web server.</p>"));
  w.client.println(F("<p><a href='/analog'>Click here to see the analog sensors</a></p>"));
  w.client.println(F("<p><a href='/digital'>Click here to see the digital sensors</a></p>"));
  w.client.println(F("</body></html>"));
}

void analogSensorPage(EasyWebServer &w){
  w.client.println(F("<!DOCTYPE HTML>"));
  w.client.println(F("<html><head><title>Analog Sensors</title></head><body>"));
  for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
    int sensorReading = analogRead(analogChannel); // Note that analogReaad uses CHANNELS insead of pin. 
    w.client.print(F("analog input "));
    w.client.print(analogChannel);
    w.client.print(F(" is "));
    w.client.print(sensorReading);
    w.client.println(F("<br />"));
  }
  w.client.println(F("<p><a href='/'>Home</a></body></html>"));
}

void digitalSensorPage(EasyWebServer &w){
  w.client.println(F("<!DOCTYPE HTML>"));
  w.client.println(F("<html><head><title>Digital Sensors</title></head><body>"));
  for (int digitalPin = 2; digitalPin < 8; digitalPin++) {
    int sensorReading = digitalRead(digitalPin);
    w.client.print(F("digital pin "));
    w.client.print(digitalPin);
    w.client.print(F(" is "));
    w.client.print(sensorReading);
    w.client.println(F("<br />"));
  }
  w.client.println(F("<p><a href='/'>Home</a></body></html>"));
}






