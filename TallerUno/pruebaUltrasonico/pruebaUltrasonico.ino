#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       "TU_WIFI"
#define WLAN_PASS       "TU_CONTRASEÑA"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "TU_USUARIO_ADAFRUIT"
#define AIO_KEY         "TU_LLAVE_AIO"

#define PIN_LM35 34
#define PIN_LED 2

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish publicador_temp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperatura");
Adafruit_MQTT_Subscribe suscriptor_led = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led");

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  
  Serial.print("Conectando a WiFi...");
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" ¡Conectado!");

  mqtt.subscribe(&suscriptor_led);
}

void loop() {
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &suscriptor_led) {
      Serial.print("Comando recibido en el LED: ");
      Serial.println((char *)suscriptor_led.lastread);
      
      int estado_led = atoi((char *)suscriptor_led.lastread);
      digitalWrite(PIN_LED, estado_led);
    }
  }
  
  int lectura_adc = analogRead(PIN_LM35);
  
  float voltaje = (lectura_adc / 4095.0) * 3300.0;
  float temperatura_c = voltaje / 10.0;

  Serial.print("Enviando temperatura: ");
  Serial.print(temperatura_c);
  Serial.println(" C");

  if (!publicador_temp.publish(temperatura_c)) {
    Serial.println("Error al publicar.");
  } else {
    Serial.println("¡Publicación exitosa!");
  }

  delay(10000);
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Conectando a MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Reintentando conexión MQTT en 5 segundos...");
       mqtt.disconnect();
       delay(5000);
       retries--;
       if (retries == 0) {
         while (1);
       }
  }
  Serial.println("¡MQTT Conectado!");
}