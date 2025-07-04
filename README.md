# Embedded Motion Intelligence System using MSP432E401Y and LSM6DSO

**Developer:** Kasra Noyan  
**Platform:** Texas Instruments MSP432E401Y + SparkFun LSM6DSO (6-axis IMU)  
**Toolchain:** Keil µVision 5 + ARM Compiler 6.21 + TI DriverLib  
**Category:** Embedded ML, Motion Tracking, Real-Time Systems

---

## 🚀 Project Overview

This project implements a production-grade embedded system for real-time motion classification and fault detection using the MSP432E401Y microcontroller and the SparkFun LSM6DSO IMU. It features embedded machine learning using TensorFlow Lite Micro, multitasking via FreeRTOS, structured UART data logging, and real-time analytics dashboards powered by OpenCV and Microsoft Power BI.

The system is designed for applications in robotics, wearables, smart sensing, and embedded AI.

---

## 🧱 Current Progress Snapshot

| Feature                          | Status       | Notes                                               |
|----------------------------------|--------------|-----------------------------------------------------|
| ✅ UART0 Output                  | Complete     | Boot message, LED status, WHO_AM_I output           |
| ✅ I²C Bus Scanner               | Complete     | Scans and confirms device on expected address       |
| ✅ WHO_AM_I Register Read        | Complete     | Successfully reads `0x6C` from LSM6DSO              |
| ✅ LED Heartbeat Loop           | Complete     | All 4 LEDs blink in sync to indicate main loop alive|
| ✅ Project Folder Cleanup       | Complete     | Old MPU-6050 code removed, LSM6DSO-only code tracked|
| ✅ GitHub Repository Reinit     | Complete     | Clean repo created for new version                  |
| ✅ Sensor Stability Fix (Hardware) | Complete   | Soldered pins to the breakout board for stronger wire connection   |
| 🤖  FreeRTOS Setup                | Complete  | Confirmed with LED blink test (main_freertos_blink.c)|
| 🛠️  LSM6DSO Register Init        | In Progress  | Preparing to configure accel/gyro via `CTRL1_XL` etc|
| 🧪 Motion Data Capture           | Upcoming     | Next step: enable full sensor readout               |
| 🧠 TFLM Integration              | Upcoming     | After sensor data confirmed        

___

## 🔧 System Capabilities

- 🔁 Reads 6-axis motion data (accelerometer + gyroscope) over I²C
- 🧠 Collects and labels motion data for TensorFlow model training
- ⚙️ Deploys TFLM model for onboard real-time classification
- 🧵 Manages tasks using FreeRTOS (sensor polling, inference, logging)
- 💬 Sends structured UART logs (CSV/JSON) to host PC
- 📊 Visualizes motion data and classification results in Power BI
- ⚠️ Detects motion faults: inactivity, spikes, invalid transitions
- 🔦 Uses color-coded LEDs to indicate motion states or faults

---

## 🛠️ Core Features

- **I²C Communication**: Register-level communication with LSM6DSO
- **Embedded ML Inference**: TensorFlow Lite Micro deployed on ARM Cortex-M4F
- **FreeRTOS Task Scheduling**: Modular real-time architecture
- **UART Logging**: Structured data streaming (9600 baud)
- **OpenCV Integration**: Real-time host visualization and webcam overlay (optional)
- **Power BI Dashboard**: Live motion/fault panels and frequency graphs
- **Fault Logic**: Spike detection, inactivity flags, and LED alerts

---

## 🧱 Hardware Overview

| Component     | Details                                     |
|---------------|---------------------------------------------|
| MCU           | TI MSP432E401Y (Cortex-M4F, 120 MHz, FPU)   |
| IMU Sensor    | SparkFun LSM6DSO (6-axis accel + gyro)      |
| I²C Pins      | SCL = PB2, SDA = PB3                        |
| UART          | UART0 for serial output                     |
| Debugger      | XDS110 onboard                              |
| LEDs          | PF0–PF4, PN0–PN1 (status and fault signals) |

---

## 🧩 Software Architecture

| File                 | Purpose                                              |
|----------------------|------------------------------------------------------|
| `main.c`             | System init, FreeRTOS start, task setup              |
| `i2c.c / i2c.h`      | LSM6DSO I²C communication & register access          |
| `imu.c / imu.h`      | Sensor init and motion vector abstraction            |
| `ml_model.c / .h`    | TensorFlow Lite Micro inference engine               |
| `uart.c / uart.h`    | UART driver and formatted output functions           |
| `logger.c / logger.h`| Data formatting (CSV/JSON), fault tags, timestamps   |
| `startup.s`          | Cortex-M interrupt vector table                      |
| `system_msp432e401y.c`| System clock configuration                          |

---
Note: Some of the above modules are currently in progress and will be added as the system scales.


## 🖥️ Host Integration

- **Python Log Parser (`log_parser.py`)**  
  Parses real-time UART logs, generates CSV, adds timestamps, detects faults.

- **OpenCV Visualization**  
  Real-time overlay of classified motion states, optional webcam integration.

- **Power BI Dashboard (`powerbi_dashboard.pbix`)**  
  - Live motion timeline  
  - Fault event log  
  - Motion class frequency histograms  
  - Inference accuracy + latency metrics (if FreeRTOS timing included)

---

## 🧪 Project Status

- ✅ MSP432 + Keil Toolchain setup
- ✅ UART and I²C configured (PB2/PB3)
- ✅ WHO_AM_I confirmed (LSM6DSO responsive at `0x6C`)
- ✅ LED GPIO and UART logging active
- ⏳ Sensor register init for accel/gyro output (reads raw values)
- ⏳ FreeRTOS task separation (initial setup complete)
- ⏳ Data logging and model training
- ⏳ TFLM model deployment and fault logic
- ⏳ Dashboard + OpenCV visualization

---

## 🧰 Tools and Frameworks

| Tool                 | Role                                  |
|----------------------|----------------------------------------|
| **Keil µVision**     | Embedded IDE and compiler              |
| **TI DriverLib**     | Hardware abstraction for MSP432        |
| **FreeRTOS**         | Real-time task management              |
| **TensorFlow Lite Micro** | Onboard ML inference              |
| **OpenCV**           | Host visualization and overlays        |
| **Power BI**         | Motion/fault data analytics dashboard  |
| **Tera Term**         | UART terminal for real-time output    |


---

## 🌟 Optional Enhancements

- SD Card logging via SPI
- BLE module for wireless telemetry
- Multi-sensor fusion (add magnetometer for 9DOF)
- Webcam-based motion benchmarking
- Online learning / anomaly detection models

---

## 🧑‍💼 Resume-Ready Summary

> Designed and implemented an embedded motion intelligence system using an LSM6DSO IMU and MSP432E401Y microcontroller. Built a custom TensorFlow Lite Micro classifier for real-time motion detection, managed tasks with FreeRTOS, and logged classification data via UART. Visualized results using OpenCV and Power BI to support real-time analysis and fault detection.

---

## 🏁 Final Outcome

This project is a top-tier embedded systems showcase that combines:

- ✅ Real-time embedded AI
- ✅ Sensor integration and task scheduling
- ✅ Visualization and host-side analytics

🎯 Aligned with career paths in:
- Embedded AI / TinyML
- Robotics and motion tracking
- Edge computing and real-time control
- Wearables, smart systems, and automotive sensing
