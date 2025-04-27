const int buzzer_pin = 4;
const int trig_pin = A6;
const int echo_pin = A4;
const int light_pin = 13;

int tempo = 80;
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
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  
  float timing = pulseIn(echo_pin, HIGH, 30000);
  int distance = (timing * 0.034) / 2;
  
  if (distance > 0 && distance < 15) {
    Serial.println("tin:" + String(distance));
    lastTime = millis();  // Reset the timer
    isStoppedPrinted = false;  // Reset the flag
    tone(buzzer_pin, 1000, 200);  // High pitch tone for close distance (15cm or less)
  } 
  else if (distance > 15 && distance < 30) {
    Serial.println("dhin:" + String(distance));
    lastTime = millis();
    isStoppedPrinted = false;
    tone(buzzer_pin, 800, 200);  // Medium pitch tone for mid-range (15-30cm)
  } 
  else if (distance > 30 && distance < 45) {
    Serial.println("tan:" + String(distance));
    lastTime = millis();
    isStoppedPrinted = false;
    tone(buzzer_pin, 500, 200);  // Low pitch tone for far distance (30-45cm)
  } 
  else {
    // Check if 10 seconds have passed since last valid detection
    if (millis() - lastTime > 10000 && !isStoppedPrinted) {
      Serial.println("stopped");
      isStoppedPrinted = true;  // Ensure it prints only once
      noTone(buzzer_pin);  // Stop the buzzer if no detection for 10 seconds
    }
    return;
  }

  delay(delaytime);
}
