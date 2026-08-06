const int LO_PLUS  = 10;
const int LO_MINUS = 11;
const int THRESH   = 470;

const unsigned long MIN_RR = 333;   
const unsigned long MAX_RR = 2000;  

const unsigned long ECG_PRINT_INTERVAL  = 20;  
const unsigned long BPM_SAMPLE_INTERVAL = 4;  

unsigned long lastEcgPrintMs  = 0;
unsigned long lastBpmSampleMs = 0;

bool          aboveThresh = false;
unsigned long lastPeakMs  = 0;
int           bpm         = 0;
int           ecgVal      = 0;
bool          leadOff     = false;

void detectBPM(int val) {
  if (val > THRESH && !aboveThresh) {
    unsigned long now = millis();
    
    if (lastPeakMs > 0) {
      unsigned long rr = now - lastPeakMs;
      if (rr >= MIN_RR && rr <= MAX_RR) {
        bpm = (int)(60000UL / rr);
      }
    }
    
    lastPeakMs  = now;
    aboveThresh = true;
  } else if (val <= THRESH) {
    aboveThresh = false;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LO_PLUS,  INPUT);
  pinMode(LO_MINUS, INPUT);
}

void loop() {
  unsigned long now = millis();

  if (now - lastBpmSampleMs >= BPM_SAMPLE_INTERVAL) {
    lastBpmSampleMs += BPM_SAMPLE_INTERVAL;

    leadOff = (digitalRead(LO_PLUS) == HIGH || digitalRead(LO_MINUS) == HIGH);
    
    if (leadOff) {
      aboveThresh = false;
      ecgVal = 0;
      bpm = 0;
      lastPeakMs = 0;
    } else {
      ecgVal = analogRead(A0);
      detectBPM(ecgVal);
    }
  }


  if (now - lastEcgPrintMs >= ECG_PRINT_INTERVAL) {
    lastEcgPrintMs += ECG_PRINT_INTERVAL;

    if (leadOff) {
      Serial.println("ECG:0,BPM:0");
    } else {
      Serial.print("ECG:");
      Serial.print(ecgVal);
      Serial.print(",BPM:");
      Serial.println(bpm);
    }
  }
}