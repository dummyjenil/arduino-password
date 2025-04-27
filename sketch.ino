const int buzzer_pin = 4;
const int trig_pin = A6;
const int echo_pin = A4;
const int light_pin = 13;

int tempo = 150;
float delaytime = 1000 * (60.0 / tempo);
unsigned long lastTime = 0;  // Last time valid distance was detected
bool isStoppedPrinted = false;

void setup() {
  pinMode(echo_pin, INPUT);
  pinMode(trig_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(light_pin, OUTPUT);
  digitalWrite(trig_pin, LOW);
  digitalWrite(buzzer_pin, LOW);
  Serial.begin(9600);
}

void loop() {
  // Send pulse to trigger sensor
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  // Measure the pulse width to get distance
  float timing = pulseIn(echo_pin, HIGH, 30000);
  int distance = (timing * 0.034) / 2;

  if (distance > 0 && distance <= 15) {
    Serial.println("tin:" + String(distance));
    lastTime = millis();  // Reset the timer
    isStoppedPrinted = false;  // Reset the flag
  } 
  else if (distance > 15 && distance <= 30) {
    Serial.println("dhin:" + String(distance));
    lastTime = millis();
    isStoppedPrinted = false;
  } 
  else if (distance > 30 && distance <= 45) {
    Serial.println("tan:" + String(distance));
    lastTime = millis();
    isStoppedPrinted = false;
  } 
  else {
    // If no valid detection for 10 seconds, print "stopped"
    if (millis() - lastTime > 10000 && !isStoppedPrinted) {
      Serial.println("stopped");
      isStoppedPrinted = true;  // Ensure it prints only once
    }
    return;
  }
  digitalWrite(light_pin, HIGH);  // Turn on light to indicate detection
  delay(delaytime);
  digitalWrite(light_pin, LOW);   // Turn off light after the delay
}
