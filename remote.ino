#define PULSE_SHORT 340
#define PULSE_LONG 612
#define STRIP 150
#define SYNC_INTERVAL 10000
#define PIN 3
#define RX_PIN 2
#define MODE_READ 1

volatile static int code[25];
volatile static int currentIndex = 0;

volatile static boolean started = false;
volatile unsigned long prevTime = 0;
volatile unsigned long prevDiff = 0;

void setup() {
    Serial.begin(9600);
    
    pinMode(PIN, OUTPUT);
    digitalWrite(PIN, LOW);
    
    pinMode(RX_PIN, INPUT);
    
    if (1 == MODE_READ) {
        attachInterrupt(digitalPinToInterrupt(RX_PIN), onChange, CHANGE);
        interrupts();
    }
}

void loop() { 
    if (0 == MODE_READ) {
        for (int i = 0; i < 3; i++) {
            sendCode("1011101101011011010100011");
        }
        
        Serial.println("Sent");
        delay(10000);
    }
}

void sendCode(char code[]) {
    digitalWrite(PIN, LOW); 
    delayMicroseconds(24000);
    
    int i = 0;
    
    while (code[i] != '\0') {
        if ('0' == code[i]) {
            digitalWrite(PIN, LOW); 
            delayMicroseconds(PULSE_SHORT);
            
            digitalWrite(PIN, HIGH); 
            delayMicroseconds(PULSE_LONG);
        } else {
            digitalWrite(PIN, LOW); 
            delayMicroseconds(PULSE_LONG);
            
            digitalWrite(PIN, HIGH); 
            delayMicroseconds(PULSE_SHORT);
        }

        i++;
    }
}

void onChange() {
    int state = digitalRead(RX_PIN);
    
    unsigned long currentTime = micros();
    unsigned long diff = currentTime - prevTime;
    prevTime = currentTime;
    
    if (!started && HIGH == state && diff >= SYNC_INTERVAL) {
        started = true;
        prevDiff = diff;
        
        return;
    }
    
    if (started && LOW == state) {
        if (
            currentIndex > 1 &&
            ((diff + prevDiff) < (PULSE_SHORT + PULSE_LONG) - STRIP || (diff + prevDiff) > (PULSE_SHORT + PULSE_LONG) + STRIP)
        ) {
            currentIndex = 0;
            prevDiff = diff;
            started = false;
            
            return;
        }
        
        if (diff <= prevDiff) {
            code[currentIndex++] = 1;
        } else {
            code[currentIndex++] = 0;
        }
        
        prevDiff = diff;
        
        if (currentIndex == 25) {
            for (int i = 0; i < 25; i++) {
                if (i == 0) {
                    Serial.print(1);
                } else {
                    Serial.print(code[i]);
                }
            }
        
            Serial.print(" || ");
            
            for (int i = 0; i < 25; i++) {
                if (i == 0) {
                    Serial.print(0);
                } else {
                    Serial.print(code[i]);
                }
            }
            
            Serial.println("");
        
            started = false;
            prevDiff = diff;
            diff = 0;
            currentIndex = 0;
        }
    } else {
        prevDiff = diff;
    }
}
