/*
 * Example to post data throw Wifi Shield 
 * Abdullah Alhazmy
 * @Alhazmy13 - http://Alhazmy13.net
 */
#include <SPI.h>
#include <WiFi.h>

//-------- WIFI SETTINGS ------- //
// EDIT: Change the 'ssid' and 'password' to match your network
char ssid[] = "SSD";  // wireless network name
char password[] = "PASSWORD"; // wireless password
int status = WL_IDLE_STATUS;
WiFiClient client;


// ------ SERVER SETTINGS -------- //
// EDIT: 'Server' address to match your domain
byte server[] = { 192, 168, 8, 101 }; // IP of server
int port = 8888;

//-------- DATA SETTINGS ------- //
// This is the data that will be passed into your POST and matches your mysql column
int yourarduinodata = 999;
String yourdatacolumn = "data=";
String yourdata;


//-------- RESPONCE SETTINGS ------- //
char inString[500]; // string for incoming serial data
int stringPos = 0; // string index counter
unsigned long time;
char c;
unsigned long lastConnectionTime = 0; // last time you connected to the server, in milliseconds
boolean lastConnected = false; // state of the connection last time through the main loop
const unsigned long postingInterval = 10 * 1000; // delay between updates, in milliseconds



void setup() {
  Serial.begin(9600);
  
  connectWifi();

  // You're connected now, so print out the status
  printWifiStatus();
  
  postData();
 
}

void loop() {

   // Print the respnce back from server
  while (client.available()) {

    c = client.read();

    Serial.write(c);

    CheckingStatus();

  }

  
  if (!client.connected() && lastConnected) {

    Serial.println();

    Serial.println("disconnecting.");

    client.stop();

  }

  // Post data agian 
    unsigned long currentMillis = millis();

  if (!client.connected() && (currentMillis - lastConnectionTime  == postingInterval)) {

     postData();

  }

  lastConnected = client.connected();

}





void connectWifi() {
  // Attempt to connect to wifi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, password);
    // Wait 10 seconds for connection
    delay(10000);
  }
}




void printWifiStatus() {
  // Print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}




// This method makes a HTTP connection to the server and POSTs data
void postData() {
  // Combine yourdatacolumn header (yourdata=) with the data recorded from your arduino
  // (yourarduinodata) and package them into the String yourdata which is what will be
  // sent in your POST request
  yourdata = yourdatacolumn + yourarduinodata;

  // If there's a successful connection, send the HTTP POST request
  if (client.connect(server, port)) {
    Serial.println("connecting...");

    // EDIT: The POST 'URL' to the location of your insert_mysql.php on your web-host
    client.println("POST /api/data HTTP/1.1");

    // EDIT: 'Host' to match your domain
    client.println("Host: 192.168.8.101:8888");
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(yourdata.length());
    client.println();
    client.println(yourdata); 
  
  } 
  else {
    // If you couldn't make a connection:
    Serial.println("Connection failed");
    Serial.println("Disconnecting.");
    client.stop();
  }
}




void CheckingStatus() {
  inString[stringPos] = c;
  if (c == '*'){
    stringPos = 0;
    delay(500);
  }
  stringPos ++;

}
