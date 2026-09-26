void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println("======================================");
  Serial.println("  Néstor Alejandro Ayala Abarca");
  Serial.println("               00133723 ");
  Serial.println("=====================================");
  Serial.println();

}

void loop() {
  
  Serial.print("ESP32 funcionando correctamente :0 - Timepo activa: ");
  Serial.print(millis()/1000.0);
  Serial.println(" segundos");

  delay(2000);

}
