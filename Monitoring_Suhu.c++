#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

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
    "/cek - Suhu dan Kelembapan\n"
    "/status");
  }

  if(text=="/cek"){

    float suhu = dht.readTemperature();
    float lembab = dht.readHumidity();
    if(isnan(suhu)||isnan(lembab)){
      bot.sendMessage(chat_id, "Sensor gagal mendeteksi/gagal");
    }
    String pesanSuhuLembab = "Suhu saat ini :"+ String(suhu) +" °C\n"
    "kelembapan saat ini :"+ String(lembab)+" %"; 
    bot.sendMessage(chat_id, pesanSuhuLembab);
  }

  if(text=="/status"){
    bot.sendMessage(chat_id, "Devices Online");
  }

}

}

void setup() {
  Serial.begin(115200);
  dht.begin();

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
