
int checkTimeMinutes = 15;
int pumpRunningTimeSeconds = 40;

int moistureSensorValue = 0;
int moisturePercent = 0;

String lastPumpStartTime = "N/A";
String lastPumpStopTime = "N/A";

void setup()
{
  Particle.variable("moisture", moistureSensorValue);
  Particle.variable("moisturePercent", moisturePercent);
  Particle.variable("lastPumpStartTime", lastPumpStartTime);
  Particle.variable("lastPumpStopTime", lastPumpStopTime);
  bool success = Particle.function("readRawMoistureData", readMoistureData);
  success = Particle.function("getMoisturePercent", getMoisturePercent);
  success = Particle.function("startPump", startPump);
  success = Particle.function("stopPump", stopPump);
  success = Particle.function("waterPlant", waterPlant);
  pinMode(D0, OUTPUT);
  
}

void loop() {
    readMoistureData("");
    if( moisturePercent < 20) {
        if(8 < Time.hour() < 22) {
            //waterPlant("");
        }
    }
    
    delay(checkTimeMinutes * 60 * 1000);
}

int readMoistureData(String data) {
    moistureSensorValue = analogRead(A1);
    moisturePercent = (int) (((float)moistureSensorValue / 4096.0) * 100.0);
    Particle.publish("moistureRaw", String(moistureSensorValue));
    Particle.publish("moisturePercent", String(moisturePercent));
    return moistureSensorValue;
}

int getMoisturePercent(String data) {
    return moisturePercent;
}

int startPump(String data) {
    lastPumpStartTime = Time.timeStr();
    Particle.publish("pumpStarted", lastPumpStartTime);
    digitalWrite(D0, HIGH);
    return 0;
}

int stopPump(String data) {
    digitalWrite(D0, LOW);
    lastPumpStopTime = Time.timeStr();
    Particle.publish("pumpStopped", lastPumpStopTime);
    return 0;
}

int waterPlant(String data) {
    // Should pump around 1l of water to the plant
    // Depends on pump used
    startPump("");
    delay (pumpRunningTimeSeconds * 1000);
    stopPump("");
    return 0;
}