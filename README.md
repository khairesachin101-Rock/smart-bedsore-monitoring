# smart-bedsore-monitoring
Sensor-based biomedical prototype for early pressure ulcer risk monitoring and preventive healthcare applications.
# 🩺 IoT-Enabled Smart Bedsore Detection & Monitoring System

## 📖 Overview

This project presents a low-cost IoT-enabled smart monitoring prototype designed for early bedsore (pressure ulcer) risk detection in bedridden patients. The system continuously monitors pressure and local temperature variations using embedded sensors and provides real-time alerts during abnormal conditions.

The prototype combines embedded systems, biomedical sensing, and IoT-based healthcare monitoring to support continuous patient observation and early risk identification.

---

# ⚠️ Problem Statement

Pressure ulcers (bedsores) are a major healthcare concern among bedridden, elderly, ICU, paralytic, and post-surgery patients. Prolonged pressure on specific body regions can reduce blood circulation and lead to tissue damage, inflammation, and severe complications if not detected early.

Traditional preventive methods such as periodic repositioning and pressure-relief mattresses do not provide continuous real-time monitoring. This project aims to establish a smart sensor-based monitoring framework for early bedsore risk identification.

---

# ✨ Key Features

* Real-time pressure monitoring using FSR sensors
* Local temperature monitoring using BME280 sensor
* ESP32-based embedded processing system
* OLED display for live monitoring
* Buzzer-based alert mechanism
* Wireless monitoring using Blynk IoT dashboard
* Real-time bedsore risk indication
* LOW, MODERATE, HIGH, and CRITICAL risk classification
* Portable and low-cost prototype architecture
* Continuous sensor-data acquisition framework
* Pressure and duration-based monitoring logic
* Prototype framework compatible with future machine learning integration

---

# 🏗️ System Architecture

The system integrates:

* Force Sensitive Resistor (FSR) sensors
* BME280 temperature sensor
* ESP32 microcontroller
* OLED display module
* Buzzer alert system
* Blynk IoT cloud interface

The prototype continuously evaluates pressure intensity, local temperature variation, and exposure duration to determine the bedsore risk level in real time.

---

# 💻 Technologies Used

* ESP32
* Embedded C / Arduino IDE
* FSR Sensors
* BME280 Sensor
* OLED Display
* Blynk IoT Platform
* Biomedical Sensor Integration
* Embedded Systems
* IoT Healthcare Monitoring

---

# 📊 Prototype Testing Results (Demo Conditions)
# 📊 Prototype Testing Results (Demo Conditions)

| Condition                          | Pressure Range | Time Frame   | Risk Status | Alert Response        |
| ---------------------------------- | -------------- | ------------ | ----------- | --------------------- |
| Normal Condition                   | < 32 mmHg      | Any duration | LOW         | LED OFF, Buzzer OFF   |
| Moderate Risk                      | 32–40 mmHg     | < 20 sec     | MODERATE    | LED ON                |
| High Risk                          | 40–60 mmHg     | < 20 sec     | HIGH        | Slow Buzzer Alert     |
| Critical Risk (High Pressure)      | > 60 mmHg      | Immediate    | CRITICAL    | Fast Alert Activated  |
| Critical Risk (Prolonged Pressure) | > 32 mmHg      | > 20 sec     | CRITICAL    | Continuous Fast Alert |

The table above represents prototype response behavior during controlled demonstration testing conditions.

---

# 🧠 Bedsore Risk Classification Framework

| Risk Level               | Pressure Range | Exposure Duration | System Response       | Clinical Interpretation     |
| ------------------------ | -------------- | ----------------- | --------------------- | --------------------------- |
| LOW                      | < 32 mmHg      | Brief exposure    | Normal Monitoring     | Safe condition              |
| MODERATE                 | 32–40 mmHg     | 15–30 min         | LED Warning           | Early tissue stress         |
| HIGH                     | 40–60 mmHg     | 5–15 min          | Slow Buzzer Alert     | Increased ischemic risk     |
| CRITICAL (High Pressure) | > 60 mmHg      | Immediate         | Fast Alert            | Severe pressure loading     |
| CRITICAL (Prolonged)     | > 32 mmHg      | > 1–4 hours       | Continuous Fast Alert | High bedsore formation risk |

The implemented framework is designed for prototype-level monitoring and demonstration purposes.

---

# ✅ Prototype Outcomes

* Successful real-time pressure monitoring
* Stable wireless IoT communication
* Real-time risk classification implementation
* Continuous OLED data visualization
* Successful abnormal pressure alert generation
* Functional sensor-data acquisition framework
* Stable ESP32-based embedded monitoring operation

---

# 🚀 Future Improvements

* Exploration of machine learning–based predictive monitoring
* Smart adaptive alert optimization
* Full-body pressure mapping
* Cloud-based healthcare monitoring
* Advanced data logging and analytics
* Wearable or smart mattress integration
* Extended validation under long-duration testing conditions

---

# 📌 Important Note

This project is an academic and research-oriented prototype developed for embedded healthcare monitoring experimentation and educational purposes.

The current prototype is not a clinically validated medical device and should not be used for direct medical diagnosis, treatment, or clinical decision-making.

---

# 📄 License

This project is intended for academic learning, embedded systems experimentation, IoT healthcare research, and biomedical prototype development.
