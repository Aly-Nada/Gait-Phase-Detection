#include <WiFi.h>
#include <WiFiAP.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <ESPmDNS.h>


// #define LED D0

// Set these to your desired credentials.
const char *ssid = "Talento 2";
const char *password = "1234567890george";

WebServer server(80);

// JSON data buffer
StaticJsonDocument<250> jsonDocument;
char buffer[250];
 
// env variable
int data1 = 1;
int data2 = 2;
int data3 = 3;
int data4 = 4;
int data5 = 5;


void create_json(char *tag, float value) {  
  jsonDocument.clear();  
  jsonDocument[tag] = value;
  serializeJson(jsonDocument, buffer);
}


void getData1Handler() {
  Serial.println("GET /data1");
  data1 = analogRead(32);
  create_json("data1", data1);
  server.send(200, "application/json", buffer);
}

 
void getData2Handler() {
  Serial.println("GET /data2");
  data2 = analogRead(35);
  create_json("data2", data2);
  server.send(200, "application/json", buffer);
}

 
void getData3Handler() {
  Serial.println("GET /data3");
  data3 = analogRead(34);
  create_json("data3", data3);
  server.send(200, "application/json", buffer);
}


void getData4Handler() {
  Serial.println("GET /data4");
  data4 = analogRead(36);
  create_json("data4", data4);
  server.send(200, "application/json", buffer);
}


void getData5Handler() {
  Serial.println("GET /data5");
  data5 = analogRead(39);
  create_json("data5", data5);
  server.send(200, "application/json", buffer);
}

 
void getAllHandler() {
  Serial.println("GET /all");
  jsonDocument.clear(); // Clear json buffer
  
  data1 = analogRead(32);
  jsonDocument["data1"] = data1;
  data2 = analogRead(35);
  jsonDocument["data2"] = data2;
  data3 = analogRead(34);
  jsonDocument["data3"] = data3;
  data4 = analogRead(36);
  jsonDocument["data4"] = data4;;
  data5 = analogRead(39);
  jsonDocument["data5"] = data5;
  serializeJson(jsonDocument, buffer);
  server.send(200, "application/json", buffer);
}


void postLED() {
  Serial.println("POST /led");
  if (server.hasArg("plain") == false) {
    //handle error here
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  
  // Get state
  int state = jsonDocument["state"];

  Serial.println("State: "+ String(state));

  // digitalWrite(LED, state);
   
  // Respond to the client
  server.send(200, "application/json", "{}");
}


void rootHandler(){
  Serial.println("GET /");
  char rootPage[10000] = "";
  server.send(200, "text/html", rootPage);
}


void setup_routing() {
  // ROOT
  server.on("/", rootHandler);
  
  // GET
  server.on("/data1", getData1Handler);
  server.on("/data2", getData2Handler);
  server.on("/data3", getData3Handler);
  server.on("/data4", getData4Handler);
  server.on("/data5", getData5Handler);
  server.on("/all", getAllHandler);
  
  //POST
  server.on("/led", HTTP_POST, postLED);
 
  // start server
  server.begin();
  Serial.println("Server started");
}


void setup() {
  // pinMode(LED, OUTPUT);

  pinMode(32, INPUT);
  pinMode(35, INPUT);
  pinMode(34, INPUT);
  pinMode(39, INPUT);
  pinMode(36, INPUT);

  Serial.begin(115200);
  Serial.println();

// ---------------------- OPEN WIFI AP
// ---------------------------------------------------------------------
  //Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  //WiFi.softAP(ssid, password);
  //IPAddress myIP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(myIP);

  //WiFi.hostname("prosthetic");

// ---------------------- CONNECT TO WIFI AP
// ---------------------------------------------------------------------

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  // ---------------------------------------------------------------------

  
  setup_routing();
}


void loop() {
  server.handleClient();  
}
