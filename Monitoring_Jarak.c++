#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "DHT.h"

// #define DHTPIN 4
// #define DHTTYPE DHT11
#define trigPin 5
#define echoPin 18
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Redmi Note 13 5G";
const char* password = "ayambakar";

#define BOTtoken "8594135545:AAGkeJn3XKS0oMG7RZnegF8t9ScXNNxEAyc"
#define CHAT_ID "8195755389"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

unsigned long lastTimeBotRan;
const unsigned long botRequestDelay = 2000;

void pesanBaru(int jumlahPesan){
for(int i = 0; i < jumlahPesan; i++){
  String chat_id = bot.messages[i].chat_id;
  String text = bot.messages[i].text;
  String user = "Alexx";
  if(text=="/start"){
    bot.sendMessage(chat_id, "Halo saya dari ESP32, apa kabar "+user+" mau cek apa?\n\n"
    "/cek - jarak\n"
    "/status");
  }

  if(text=="/cek"){
    digitalWrite(trigPin,LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);

    long waktu = pulseIn(echoPin,HIGH);
    float distance = waktu * 0.034 / 2;
    String pesanJarak = "Jarak : "+String(distance)+" cm";
    bot.sendMessage(chat_id, pesanJarak);
  }

  if(text=="/status"){
    bot.sendMessage(chat_id, "Devices Online");
  }

}

}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  WiFi.begin(ssid, password);
  client.setInsecure();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi Connected");
}

void loop() {

  if (millis() - lastTimeBotRan > botRequestDelay) {
    int jumlahPesan= bot.getUpdates(bot.last_message_received + 1);

    while (jumlahPesan) {
      pesanBaru(jumlahPesan);
      jumlahPesan = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
  }
}
