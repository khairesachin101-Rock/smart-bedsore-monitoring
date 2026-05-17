```cpp
// =====================================================
//          SMART BEDSORE DETECTION SYSTEM
// =====================================================

#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "BedSore Detection"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ================= WIFI =================
// Replace with your own WiFi credentials

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

// ================= BLYNK AUTH TOKEN =================
// Replace with your own Blynk Authentication Token

char auth[] = "YOUR_BLYNK_AUTH_TOKEN";

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

TwoWire OLED_I2C = TwoWire(0);
TwoWire BME_I2C  = TwoWire(1);

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &OLED_I2C,
  -1
);

// ================= BME =================
Adafruit_BME280 bme;

// ================= PINS =================

// FSRs
const int FSR1_PIN = 34;
const int FSR2_PIN = 35;
const int FSR3_PIN = 32;
const int FSR4_PIN = 33;

// OLED
const int OLED_SDA = 18;
const int OLED_SCL = 19;

// BME280
const int BME_SDA = 21;
const int BME_SCL = 22;

// OUTPUTS
const int RED_LED = 25;
const int BUZZER_PIN = 26;

// ================= CONSTANTS =================

// Baseline skin temperature
const float BASELINE_TEMP = 34.5;

// Temperature rise threshold
const float TEMP_DELTA_RISK = 1.0;

// Demo-friendly force scaling
const float MAX_FORCE_N = 25.0;

// Effective contact area
const float CONTACT_AREA_CM2 = 20.0;

const float CONTACT_AREA_M2 =
      CONTACT_AREA_CM2 / 10000.0;

// ================= VARIABLES =================
unsigned long forceStart = 0;

bool forceDetected = false;

// ================= HELPERS =================
float clampValue(float x, float low, float high)
{
  if (x < low) return low;

  if (x > high) return high;

  return x;
}

// ADC → Force
float rawToForce(int raw)
{
  raw = constrain(raw, 0, 4095);

  return ((float)raw / 4095.0) * MAX_FORCE_N;
}

// Buzzer OFF
void buzzerOff()
{
  noTone(BUZZER_PIN);
}

// HIGH risk buzzer
void highRiskBuzzer(unsigned long now)
{
  if ((now / 300) % 2 == 0)
  {
    tone(BUZZER_PIN, 2200);
  }
  else
  {
    noTone(BUZZER_PIN);
  }
}

// CRITICAL risk buzzer
void criticalRiskBuzzer(unsigned long now)
{
  if ((now / 120) % 2 == 0)
  {
    tone(BUZZER_PIN, 3200);
  }
  else
  {
    noTone(BUZZER_PIN);
  }
}

// ================= SETUP =================
void setup()
{
  Serial.begin(115200);

  // ================= OLED =================
  OLED_I2C.begin(OLED_SDA, OLED_SCL, 100000);

  bool oledOK = display.begin(
      SSD1306_SWITCHCAPVCC,
      0x3C
  );

  // Try second address
  if (!oledOK)
  {
    oledOK = display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3D
    );
  }

  if (oledOK)
  {
    display.clearDisplay();

    display.setTextColor(WHITE);

    display.setTextSize(2);

    display.setCursor(0,0);
    display.println("BEDSORE");

    display.setCursor(0,25);
    display.println("SYSTEM");

    display.display();

    delay(2000);
  }
  else
  {
    Serial.println("OLED FAILED");
  }

  // ================= BME280 =================
  BME_I2C.begin(BME_SDA, BME_SCL, 100000);

  bool bmeOK = bme.begin(0x76, &BME_I2C);

  if (!bmeOK)
  {
    bmeOK = bme.begin(0x77, &BME_I2C);
  }

  if (bmeOK)
  {
    Serial.println("BME280 OK");
  }
  else
  {
    Serial.println("BME280 FAILED");
  }

  // ================= OUTPUTS =================
  pinMode(RED_LED, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RED_LED, LOW);

  buzzerOff();

  // ================= ADC =================
  analogReadResolution(12);

  // ================= BLYNK =================
  Blynk.begin(auth, ssid, pass);

  Serial.println("SYSTEM READY");
}

// ================= LOOP =================
void loop()
{
  Blynk.run();

  unsigned long now = millis();

  // =================================================
  //                  READ FSR
  // =================================================

  int raw1 = analogRead(FSR1_PIN);
  int raw2 = analogRead(FSR2_PIN);
  int raw3 = analogRead(FSR3_PIN);
  int raw4 = analogRead(FSR4_PIN);

  // =================================================
  //                  FORCE
  // =================================================

  float f1 = rawToForce(raw1);
  float f2 = rawToForce(raw2);
  float f3 = rawToForce(raw3);
  float f4 = rawToForce(raw4);

  // Average force
  float avgForce =
      (f1 + f2 + f3 + f4) / 4.0;

  // =================================================
  //               PRESSURE (mmHg)
  // =================================================

  float avgPressurePa =
      avgForce / CONTACT_AREA_M2;

  float avgPressureMmHg =
      avgPressurePa / 133.322f;

  // =================================================
  //               FORCE SPREAD
  // =================================================

  float maxForce =
      max(max(f1,f2), max(f3,f4));

  float minForce =
      min(min(f1,f2), min(f3,f4));

  float forceSpread =
      maxForce - minForce;

  // =================================================
  //               TEMPERATURE
  // =================================================

  float temperatureC =
      bme.readTemperature();

  float tempDelta =
      temperatureC - BASELINE_TEMP;

  // =================================================
  //               FORCE DURATION
  // =================================================

  if (avgPressureMmHg >= 32)
  {
    if (!forceDetected)
    {
      forceDetected = true;

      forceStart = now;
    }
  }
  else
  {
    forceDetected = false;
  }

  float forceDuration = 0.0;

  if (forceDetected)
  {
    forceDuration =
        (now - forceStart) / 1000.0;
  }

  // =================================================
  //              RISK SCORE
  // =================================================

  float forceComponent =
      clampValue(
        avgForce / 10.0,
        0.0,
        1.0
      );

  float tempComponent =
      clampValue(
        (tempDelta - TEMP_DELTA_RISK) / 3.0,
        0.0,
        1.0
      );

  float durationComponent =
      clampValue(
        forceDuration / 20.0,
        0.0,
        1.0
      );

  float spreadComponent =
      clampValue(
        forceSpread / 5.0,
        0.0,
        1.0
      );

  float riskScore =
      (forceComponent * 45.0) +
      (tempComponent * 20.0) +
      (durationComponent * 25.0) +
      (spreadComponent * 10.0);

  riskScore =
      clampValue(riskScore, 0.0, 100.0);

  // =================================================
  //                  RISK LEVEL
  // =================================================

  String riskLevel = "LOW";

  int displayRisk = 0;

  if (avgPressureMmHg < 32)
  {
    riskLevel = "LOW";
    displayRisk = 0;
  }

  if (avgPressureMmHg >= 32 &&
      avgPressureMmHg < 40)
  {
    riskLevel = "MODERATE";
    displayRisk = 25;
  }

  if (avgPressureMmHg >= 40 &&
      avgPressureMmHg < 60)
  {
    riskLevel = "HIGH";
    displayRisk = 50;
  }

  if (
      avgPressureMmHg >= 60 ||

      (
        avgPressureMmHg >= 32 &&
        forceDuration > 20
      )
     )
  {
    riskLevel = "CRITICAL";
    displayRisk = 100;
  }

  // =================================================
  //                  ALERTS
  // =================================================

  if (displayRisk == 0)
  {
    digitalWrite(RED_LED, LOW);
    buzzerOff();
  }

  else if (displayRisk == 25)
  {
    digitalWrite(RED_LED, HIGH);
    buzzerOff();
  }

  else if (displayRisk == 50)
  {
    digitalWrite(RED_LED, HIGH);
    highRiskBuzzer(now);
  }

  else
  {
    digitalWrite(RED_LED, HIGH);

    criticalRiskBuzzer(now);

    Blynk.logEvent(
      "bedsore_alert",
      "CRITICAL BEDSORE RISK!"
    );
  }

  // =================================================
  //                    BLYNK
  // =================================================

  Blynk.virtualWrite(V0, temperatureC);
  Blynk.virtualWrite(V1, riskLevel);
  Blynk.virtualWrite(V2, displayRisk);
  Blynk.virtualWrite(V3, avgPressureMmHg);
  Blynk.virtualWrite(V4, avgForce);

  // =================================================
  //                  OLED DISPLAY
  // =================================================

  display.clearDisplay();

  display.setTextColor(WHITE);

  display.setTextSize(1);

  display.setCursor(0,0);
  display.println("BEDSORE ALERTS");

  display.setCursor(0,12);
  display.print("Avg Pressure:");
  display.print(avgPressureMmHg,0);
  display.println(" mmHg");

  display.setCursor(0,24);
  display.print("Avg Force:");
  display.print(avgForce,1);
  display.println(" N");

  display.setCursor(0,36);
  display.print("Temperature:");
  display.print(temperatureC,1);
  display.println(" C");

  display.setCursor(0,48);
  display.print("Risk Score:");
  display.print(displayRisk);
  display.println("%");

  display.setCursor(0,58);
  display.print("Risk Status:");
  display.print(riskLevel);

  display.display();

  // =================================================
  //                    SERIAL
  // =================================================

  Serial.print("Pressure(mmHg): ");
  Serial.print(avgPressureMmHg);

  Serial.print("  Force(N): ");
  Serial.print(avgForce);

  Serial.print("  Temp(C): ");
  Serial.print(temperatureC);

  Serial.print("  Risk: ");
  Serial.print(riskLevel);

  Serial.print("  Score: ");
  Serial.println(displayRisk);

  delay(200);
}
```
