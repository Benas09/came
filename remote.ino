#define PULSE_SHORT 340
#define PULSE_LONG 612
#define PIN 3

void setup() {
  Serial.begin(9600);

  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW);
}

void loop() {
    digitalWrite(PIN, LOW); 
    delayMicroseconds(24000);

    sendCode();

    digitalWrite(PIN, LOW); 
    delayMicroseconds(24000);

    sendCode();

    digitalWrite(PIN, LOW); 
    delayMicroseconds(24000);

    sendCode();

    Serial.println("Done");
    delay(3000);  
}

void sendCode() {
    //1011101101011011010100011

    sendOne();
    sendZero();
    sendOne();
    sendOne();

    sendOne();
    sendZero();
    sendOne();
    sendOne();

    sendZero();
    sendOne();
    sendZero();
    sendOne();

    sendOne();
    sendZero();
    sendOne();
    sendOne();

    sendZero();
    sendOne();
    sendZero();
    sendOne();

    sendZero();
    sendZero();
    sendZero();
    sendOne();
    sendOne();
}

void sendOne() {
  digitalWrite(PIN, LOW); 
  delayMicroseconds(PULSE_LONG);

  digitalWrite(PIN, HIGH); 
  delayMicroseconds(PULSE_SHORT);
}

void sendZero() {
  digitalWrite(PIN, LOW); 
  delayMicroseconds(PULSE_SHORT);

  digitalWrite(PIN, HIGH); 
  delayMicroseconds(PULSE_LONG);
}
