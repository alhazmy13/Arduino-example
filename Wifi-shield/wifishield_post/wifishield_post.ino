/*
 * Example to post data throw Wifi Shield 
 * Abdullah Alhazmy
 * @Alhazmy13 - http://Alhazmy13.net
 */
#include <SPI.h>
#include <WiFi.h>
#include <Time.h>  
#include <ArduinoJson.h>

//-------- WIFI SETTINGS ------- //
// EDIT: Change the 'ssid' and 'password' to match your network
char ssid[] = "AH";  // wireless network name
char password[] = "1135face"; // wireless password
int status = WL_IDLE_STATUS;
WiFiClient client;


// ------ SERVER SETTINGS -------- //
// EDIT: 'Server' address to match your domain
byte server[] = { 192, 168, 8, 100 }; // IP of server
int port = 8888;

//-------- DATA SETTINGS ------- //
// This is the data that will be passed into your POST and matches your mysql column
StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
JsonArray& data = root.createNestedArray("data");
String yourdatacolumn = "data=";
String yourdata;

//-------- RESPONCE SETTINGS ------- //
char inString[500]; // string for incoming serial data
int stringPos = 0; // string index counter
unsigned long time;
char c;
unsigned long lastConnectionTime = 0;            // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 30L * 1000L; // delay between updates, in milliseconds


//-------- RIP SETTINGS ------- //
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 10;        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 2000;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 3;    //the digital pin connected to the PIR sensor's output

//


void setup() {
  Serial.begin(9600);
  
  connectWifi();

  // You're connected now, so print out the status
  printWifiStatus();
  
  //Set time 
  setTime(12,00,00,1,1,15); // Another way to set
  lastConnectionTime = millis();
 //Rip
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);

  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
}

void loop() {
  
   // Print the respnce back from server
  while (client.available()) {

     char c = client.read();
    Serial.write(c);

  }
 // Serial.println();
 

  // Post data agian 
if (millis() - lastConnectionTime > postingInterval) {
     postData();
  }


  updaeRipValue();


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

   // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();
  // If there's a successful connection, send the HTTP POST request
  if (client.connect(server, port)) {
    char body[256];
    root.prettyPrintTo(body, sizeof(body));
    yourdata = yourdatacolumn + body;
    Serial.println("connecting...");
    Serial.println(yourdata);
    // EDIT: The POST 'URL' to the location of your insert_mysql.php on your web-host
    client.println("POST /api/data HTTP/1.1");
    client.println("Host: 192.168.8.100:8888");
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(yourdata.length());
    client.println();
    client.print(yourdata);
    client.println(); 
    clearJson();
    lastConnectionTime = millis();
  } 
  else {
    // If you couldn't make a connection:
    Serial.println("Connection failed");
    Serial.println("Disconnecting.");
    client.stop();
  }
}


void updaeRipValue(){
  if(digitalRead(pirPin) == HIGH){
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         addCurrentTimeToJson("1");
         lockLow = false;            
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec"); 
         delay(50);
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       

       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           addCurrentTimeToJson("0");
           lockLow = true;                        
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           delay(50);
           }
       }
}

void clearJson(){
  jsonBuffer = StaticJsonBuffer<200>();
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& data = root.createNestedArray("data");

}

void addCurrentTimeToJson(String value){
  // digital clock display of the time
 
  String currentTime = ((String)year()+"-"+month()+"-"+day()+" "+hour())+printDigits(minute())+printDigits(second());
  String upldValue = value+","+currentTime;
  data.add(upldValue);  // 6 is the number of decimals to print
}

String printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  String digit = ":";
  if(digits < 10)
    digit+= "0";
  digit+=digits;
  return digit;
}


void CheckingStatus() {
  inString[stringPos] = c;
  if (c == '*'){
    stringPos = 0;
    delay(500);
  }
  stringPos ++;

}