#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
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
    http.begin("url"); //dopsat URL
    http.addHeader("Content-Type", "text/plain");
    int httpResponseCode = http.POST("POSTING from ESP32");
    if(httpResponseCode>0){
  
    String response = http.getString();
    StaticJsonDocument<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(response);
    kalibracePaliva = parsed['palivo'];
    tlakVNadrzi = parsed['nadrz'];
    delkaTestu = parsed['delka'];
    kalibraceIgnition = parsed['kalign'];
    casOtevreni = parsed['otevreni'];
  
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
  }
  else if(digitalRead(Start) == true){
    start = true;
  }
  else{
    //Kdyby neco, tak dopsat
  }
}
