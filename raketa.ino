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
int dalsiPricetniCasu = 0;
bool start = false;
bool stopS = false;
bool poprve = true;
long aktualniCas;
long konecnyCas;

//Hlavni piny
int Va = 1;
int Vb = 2;
int Vc = 3;
int Vd = 4;
int Vd = 5;
int Start = 6;
int Stop = 7;

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
  pinMode(Vf, OUTPUT);
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
    if(poprve){
      poprve = false;
      digitalWrite(Vd, HIGH);
      if(kalibracePaliva < 10000){
        delay(kalibrace);
        digitalWrite(Vf, HIGH);
        delay(10000 - kalibrace);
        digitalWrite(Va, HIGH);
        dalsiPricetniCasu = 10000;
      }
      else if(kalibracePaliva > 10000){
        delay(10000);
        digitalWrite(Va, HIGH);
        delay(kalibrace - 10000);
        digitalWrite(Vf, HIGH);
        dalsiPricetniCasu = 10000 + kalibrace;
      }
      else{
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
