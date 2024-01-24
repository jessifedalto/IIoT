#include <Arduino.h>
#include <WiFi.h>
#include "FirebaseESP32.h"

const char* WIFI_SSID = "Redmi 9";
const char* WIFI_PW = "12345678";


FirebaseData fbdo;
FirebaseAuth fbauth;
FirebaseConfig fbconfig;

const char* FB_HOST = "https://francesco-5cdcb-default-rtdb.firebaseio.com";
const char* FB_KEY = "4YrankdtBZ05WEpTNr3C9SsKahI3Pf3K8v54P9lB";

const int trigPin = 13;
const int echoPin = 12;
const int buzzer = 4;

int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;

int motor2Pin1 = 33;
int motor2Pin2 = 32;
int enable2Pin = 25;

int motor3Pin1 = 21;
int motor3Pin2 = 19;
int enable3Pin = 18;

int motor4Pin1 = 23;
int motor4Pin2 = 22;
int enable4Pin = 15;

const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 170;

bool stoped = false;
float counter = 10;

#define VELOCIDADE_DO_SOM 0.034
#define CM_PARA_POLEGADA 0.393701

long duracao;
float distancCm;
float distance;

int choice;

const uint8_t PIN_BOTAO = 5; 

bool botao_anterior = LOW;
bool pressionado = LOW;
long tempo = 0;
int buzzerBool = 0;
long tempobuzzer = 0;


bool connectWifi(const char* ssid, const char* pw)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, pw);
  Serial.print("\nConectando!");
  int tentativa = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    tentativa++;
    if (tentativa > 300)
    {
      return false;
    }
    Serial.print(".");
    delay(200);
  }
  Serial.print("\nConectado com o IP:");
  Serial.println(WiFi.localIP());
  return true;
}


void setup()
{
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  pinMode(motor3Pin1, OUTPUT);
  pinMode(motor3Pin2, OUTPUT);
  pinMode(enable3Pin, OUTPUT);

  pinMode(motor4Pin1, OUTPUT);
  pinMode(motor4Pin2, OUTPUT);
  pinMode(enable4Pin, OUTPUT);

  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(enable2Pin, pwmChannel);
  ledcAttachPin(enable3Pin, pwmChannel);
  ledcAttachPin(enable4Pin, pwmChannel);
  ledcWrite(pwmChannel, dutyCycle);

  Serial.begin(115200); // Inicia a comunicação serial
  pinMode(buzzer, OUTPUT);
  pinMode(PIN_BOTAO, INPUT);
  pinMode(trigPin, OUTPUT); // Define o trigPin como uma saída
  pinMode(echoPin, INPUT); // Define o echoPin como uma entrada
  connectWifi(WIFI_SSID, WIFI_PW);
  fbconfig.database_url = FB_HOST;
  fbconfig.signer.tokens.legacy_token = FB_KEY;
  fbdo.setBSSLBufferSize(4096, 1024);
  Firebase.reconnectWiFi(true);
  Firebase.begin(&fbconfig, &fbauth);
}

void stop(){
  
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  digitalWrite(motor3Pin1, LOW);
  digitalWrite(motor3Pin2, LOW);
  digitalWrite(motor4Pin1, LOW);
  digitalWrite(motor4Pin2, LOW);
  if (stoped == false)
  {
    FirebaseJson json;
    json.set("/DEF", 0);
    Firebase.updateNode(fbdo,"carro2", json);
  }  
  stoped = true;
}

void foward(){
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  digitalWrite(motor3Pin1, HIGH);
  digitalWrite(motor3Pin2, LOW);
  digitalWrite(motor4Pin1, HIGH);
  digitalWrite(motor4Pin2, LOW);
  stoped = false;
}

void backward(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  digitalWrite(motor3Pin1, LOW);
  digitalWrite(motor3Pin2, HIGH);
  digitalWrite(motor4Pin1, LOW);
  digitalWrite(motor4Pin2, HIGH);
  stoped = false;
}

void turnLeft(){
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  digitalWrite(motor3Pin1, LOW);
  digitalWrite(motor3Pin2, HIGH);
  digitalWrite(motor4Pin1, LOW);
  digitalWrite(motor4Pin2, HIGH);
  stoped = false;
}

void turnRight(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  digitalWrite(motor3Pin1, HIGH);
  digitalWrite(motor3Pin2, LOW);
  digitalWrite(motor4Pin1, HIGH);
  digitalWrite(motor4Pin2, LOW);
  stoped = false;
}

float readSensor()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // Define o trigPin no estado ALTO por 10 microssegundos
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW); // Lê o echoPin, retorna o tempo de viagem da onda sonora em microssegundos

  duracao = pulseIn(echoPin, HIGH); // Calcule a distância

  distancCm = duracao * VELOCIDADE_DO_SOM / 2;

  return distancCm;
}

void loop()
{
  
  botao_anterior = pressionado;
  pressionado = digitalRead(PIN_BOTAO);

  if(pressionado && !botao_anterior && millis() - tempo > 50)
  {
    buzzerBool = !buzzerBool;
    digitalWrite(buzzer, buzzerBool);
    tempobuzzer = millis();
    tempo = millis();
  }

  if (millis() - tempobuzzer > 5000)
  {
    digitalWrite(buzzer, 0);
    tempobuzzer = 0;
  }
  

  readSensor();

  Serial.print("Distancia (cm): "); // Imprime a distância no Serial Monitor
  Serial.println(readSensor());

  FirebaseJson json;

  json.set("/distance", distancCm);

  Firebase.updateNode(fbdo,"carro2", json);

  if (Firebase.getInt(fbdo, "carro2/DEF")) 
  {
    choice = fbdo.to<int>();
    Serial.println(choice);
  }

 else {
    Serial.println(fbdo.errorReason());
  }

  switch (choice)
  {
  case 0:
    stop();
    break;
  case 1:
    if (readSensor() < 60){
      stop();
    } 
    else 
    {
      Serial.println("Walking");
      foward();
    }
    break;
  case 2:
    if (readSensor() < 60){
      stop();
    } 
    else 
    {
      turnLeft();
    }
    break;
  case 3:
    if (readSensor() < 60){
      stop();
    } 
    else 
    {
      turnRight();
    }
    break;
  case 4:
    backward();
    break;
  default:
    stop();
  }
  //delay(300);

}
