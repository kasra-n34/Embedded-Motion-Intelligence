# Motion Fault Logger

## Overview

**Motion Fault Logger** is a real-time embedded system designed to monitor and report abnormal motion patterns using the **MSP432E401Y microcontroller** and the **MPU-6050** inertial measurement unit (IMU). The goal of this project is to provide a lightweight and extensible foundation for motion analysis in applications such as robotics, industrial monitoring, and wearable technology.

The system captures data from the IMU via I²C and transmits the results over UART in structured formats like CSV or JSON. It is designed for further integration with tools like Python data viewers and Power BI dashboards for advanced analysis and visualization.

This project is written in C, compiled with Keil uVision 5, and uses **TI’s SimpleLink DriverLib** for hardware abstraction.

---

## Features

- **Real-time data acquisition** from accelerometer and gyroscope.
- **I²C communication** with the MPU-6050 sensor.
- **UART-based data logging** for serial terminal output.
- **CSV/JSON-formatted output** for integration with analysis tools.
- **Modular codebase** for future expansion (e.g., fault classification, Edge AI).
- **Keil project integration** with proper clock configuration and peripheral initialization.

---

## Getting Started

### Prerequisites

To build and run this project, you will need:

- **MSP432E401Y LaunchPad** (TI)
- **MPU-6050 IMU module** (via I²C)
- **Keil MDK-ARM v5** (Compiler version ≥ 6.21)
- **SimpleLink MSP432E4 SDK** (version 4.20.00.12 or compatible)
- **UART terminal software** (e.g., TeraTerm, PuTTY)

### Setup Instructions

1. Clone or download this repository.
2. Open the `.uvprojx` file in Keil uVision.
3. Ensure the include paths point to the correct location of your installed SimpleLink SDK.
4. Flash the compiled binary to the MSP432E401Y board.
5. Connect via a UART terminal (default baud: 115200) to observe real-time output.
6. Confirm successful MPU-6050 connection by checking for `MPU6050 detected` in the terminal.

---

## Development Roadmap

The project is being developed in phases:

1. ✅ Microcontroller clock setup and UART configuration  
2. ✅ I²C peripheral configuration and sensor wakeup  
3. ✅ Sensor identity validation (WHO_AM_I)  
4. ⏳ Real-time data acquisition and formatting  
5. ⏳ Basic fault pattern detection  
6. ⏳ UART CSV/JSON data streaming  
7. ⏳ Python CLI and Power BI integration  
8. ⏳ Optional: TinyML-based fault classification on-device

---

## Author

**Kasra Noyan**  
Bachelor of Engineering, Computer Engineering  
McMaster University  
- GitHub: [@kasra-n34](https://github.com/kasra-n34)  
- LinkedIn: [linkedin.com/in/kasranoyan](https://www.linkedin.com/in/kasra-noyan-1b105421b/)

---

## License

This project is open source.

---

## Notes

This repository is actively under development. Future additions will include a Python-based data viewer and fault classification routines. Feedback and collaboration are welcome.
