#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Bridge.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

//Hlavni promenne
const char* jmenoWifi = "test";
const char* hesloWifi = "test";
long kalibracePaliva = 0;
String c = "";
int tlakVNadrzi = 0;
int delkaTestu = 0;
int kalibraceIgnition = 0;
int casOtevreni = 0;
long dalsiPricetniCasu = 0;
bool start = false;
bool stopS = false;
bool poprve = true;
long aktualniCas;
long konecnyCas;

//Hlavni piny
int Va = 16;
int Vb = 27;
int Vc = 14;
int Vd = 12;
int Vf = 13;
int Start = 6;
int Stop = 7;

void setup() {
  Serial.begin(115200);
  Serial.println("Vitejte, pane.");
  delay(4000);
  //Inicializace pripojeni
  WiFi.begin(jmenoWifi, hesloWifi);

  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
    HTTPClient http;
    WiFiClient client;
     //dopsat URL
    Serial.println("Access URL");
    http.begin("https://jsonplaceholder.typicode.com/todos/1" );  
    int httpResponseCode = http.GET();
    String res = http.getString();
    Serial.println(res);
    Serial.println(httpResponseCode);
    if(httpResponseCode>0){
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, res);
      kalibracePaliva = doc["userId"];
      tlakVNadrzi = doc["userId"];
      delkaTestu = doc["userId"];
      kalibraceIgnition = doc["userId"];
      casOtevreni = doc["userId"];
      delay(2000);
  }
  else{
    Serial.println("Selhani requestu.");
  }
  Serial.println("Going to loop");

    //Inicializace pinu
    pinMode(Va, OUTPUT);
    pinMode(Vb, OUTPUT);
    pinMode(Vc, OUTPUT);
    pinMode(Vd, OUTPUT);
    pinMode(Vf, OUTPUT);
    //pinMode(Start, INPUT_PULLUP);
    //pinMode(Stop, INPUT_PULLUP);
}

void loop() {
  Serial.println("Loop");
  Serial.println(kalibracePaliva);
  digitalWrite(Va, HIGH);
  digitalWrite(Vb, HIGH);
  digitalWrite(Vc, HIGH);
  digitalWrite(Vd, HIGH);
  digitalWrite(Vf, HIGH);
  delay(4000);
  digitalWrite(Va, LOW);
  digitalWrite(Vb, LOW);
  digitalWrite(Vc, LOW);
  digitalWrite(Vd, LOW);
  digitalWrite(Vf, LOW);
  delay(2000);
  //Podminky nize resi celou logiku s tlacitkem start a stop
  if(stopS){}
  else if(digitalRead(Stop) == true){
    stopS = true;
  }
  else if(start) {
      if(poprve) {
      poprve = false;
      digitalWrite(Vd, HIGH);
      if(kalibracePaliva < 10000) {
        delay(kalibracePaliva);
        digitalWrite(Vf, HIGH);
        delay(10000 - kalibracePaliva);
        digitalWrite(Va, HIGH);
        dalsiPricetniCasu = 10000;
      }
      else if(kalibracePaliva > 10000) {
        delay(10000);
        digitalWrite(Va, HIGH);
        delay(kalibracePaliva - 10000);
        digitalWrite(Vf, HIGH);
        dalsiPricetniCasu = 10000 + kalibracePaliva;
      }
      else {
        delay(10000);
        digitalWrite(Va, HIGH);
        digitalWrite(Vf, HIGH);
        dalsiPricetniCasu = 10000;
      }
      delay(kalibraceIgnition);
      digitalWrite(Va, LOW);
      digitalWrite(Vd, LOW);
      digitalWrite(Vb, HIGH);
      konecnyCas = millis() + delkaTestu + dalsiPricetniCasu;
    }
    else {
      aktualniCas = millis();
      reliefeCyklus();
      if(aktualniCas >= konecnyCas){
        stopS = true;
      }
      else{
        //dopsat, kontrola senzoru apod.
      }
    }
  }
  else if(digitalRead(Start) == true){
    start = true;
  }
  else{
    //Kdyby neco, tak dopsat
  }
}

void reliefeCyklus(){
  //Řeší problematiku reliefu
  if(tlakVNadrzi > namerenyTlak){
    digitalWrite(Reliefe, LOW);
  }
  else if(tlakVNadrzi+1 <= namerenyTlak){
    digitalWrite(Reliefe, HIGH);
  }
}
