const int LO_PLUS  = 10;
const int LO_MINUS = 11;

const int THRESH = 470;
const unsigned long MIN_RR   = 333;   // ~120 BPM max limit
const unsigned long MAX_RR   = 2000;  // ~30  BPM min limit

bool          aboveThresh    = false;
unsigned long lastPeakMs     = 0;
int           bpm            = 0;

void detectBPM(int ecgVal) {
  if (ecgVal > THRESH && !aboveThresh) {        // ← rising edge cross 
    unsigned long now = millis();
    unsigned long rr  = now - lastPeakMs;
    if (lastPeakMs > 0 && rr >= MIN_RR && rr <= MAX_RR) {
      bpm = (int)(60000UL / rr);               // BPM = 60s / RR-interval
    }
    lastPeakMs  = now;
    aboveThresh = true;                         // duplicate peak 
  } else if (ecgVal <= THRESH) {
    aboveThresh = false;                        // next peak 
  }
}



void setup() {
  Serial.begin(115200);
  pinMode(LO_PLUS,  INPUT);
  pinMode(LO_MINUS, INPUT);
  randomSeed(analogRead(A2));
}


void loop() {
  if (digitalRead(LO_PLUS) == HIGH || digitalRead(LO_MINUS) == HIGH) {
   
    Serial.println("ECG:0,BPM:0");
   
    aboveThresh = false;
  } else {
    int ecgVal = analogRead(A0);
    detectBPM(ecgVal);
    Serial.print("ECG:");
    Serial.print(ecgVal);
    Serial.print(",BPM:");
    Serial.println(bpm);
  }
  delay(20);   
}
