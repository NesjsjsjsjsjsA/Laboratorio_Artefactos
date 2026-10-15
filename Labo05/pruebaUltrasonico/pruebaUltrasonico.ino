const int pinTrig = 4;
const int pinEcho = 5;

void setup() {
  Serial.begin(115200);
  delay(3000); 
  
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
  
  digitalWrite(pinTrig, LOW);
  Serial.println("\n--- Iniciando prueba HC-SR04 con Pulso Extendido ---");
}

void loop() {
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  
  // Hack: Pulso de 50 microsegundos (en lugar de 10) para asegurar 
  // que el sensor de 5V detecte la señal de 3.3V del ESP32.
  digitalWrite(pinTrig, HIGH);
  delay(10);
  digitalWrite(pinTrig, LOW);
  
  // Timeout de 30ms para evitar cuelgues
  long duracion = pulseIn(pinEcho, HIGH, 30000);
  
  if (duracion == 0) {
    Serial.println("Error: Timeout / 0.00 cm");
  } else {
    float distancia = (duracion * 0.0343) / 2;
    Serial.print("Distancia: ");
    Serial.print(distancia);
    Serial.println(" cm");
  }
  
  delay(500); 
}