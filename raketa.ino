#include <HttpClient.h>
#include <ArduinoJson.h>
#include <Bridge.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

//Hlavni promenne
const char* jmenoWifi = "test";
const char* hesloWifi = "test";
int kalibracePaliva = 0;
int tlakVNadrzi = 0;
int delkaTestu = 0;
int kalibraceIgnition = 0;
int casOtevreni = 0;
bool start = false;
bool stopS = false;
long aktualniCas = millis();

//Hlavni piny
int Va = 1;
int Vb = 2;
int Vc = 3;
int Vd = 4;
int Start = 5;
int Stop = 6;

void setup() {
  Serial.begin(115200);
  delay(4000);
  //Inicializace pripojeni
  WiFi.begin(jmenoWifi, hesloWifi);

  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  if(WiFi.status() == WL_CONNECTED){
    HttpClient http;
     //dopsat URL
    int httpResponseCode = http.get("https://jsonplaceholder.typicode.com/todos/1");
    if(httpResponseCode>0){
  
    String response = http.readString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, response);
    kalibracePaliva = doc["userId"];
    tlakVNadrzi = doc["userId"];
    delkaTestu = doc["userId"];
    kalibraceIgnition = doc["userId"];
    casOtevreni = doc["userId"];
  
    }else{
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
    }
  }

  //Inicializace pinu
  pinMode(Va, OUTPUT);
  pinMode(Vb, OUTPUT);
  pinMode(Vc, OUTPUT);
  pinMode(Vd, OUTPUT);
  pinMode(Start, INPUT_PULLUP);
  pinMode(Stop, INPUT_PULLUP);
}

void loop() {
  //Podminky nize resi celou logiku s tlacitkem start a stop
  if(stopS){}
  else if(digitalRead(Stop) == true){
    stopS = true;
  }
  else if(start){
  //kontrola casu, az na tests
  }
  else if(digitalRead(Start) == true){
    start = true;
  }
  else{
    //Kdyby neco, tak dopsat
  }
}
