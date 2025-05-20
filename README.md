# STM32F4 ADC Driver for Secure Data Logging

This project demonstrates the implementation of a low-power, high-performance embedded system using the **STM32F401RE** microcontroller (ARM Cortex-M4). It features a keypad and LCD-based user interface, password-based access control, real-time ADC monitoring, and UART-based data transmission—designed for SCADA and other industrial applications.

---

## 🔧 Features

- 🧠 **User Interface**: 4x3 Matrix Keypad for password input and LCD display for status
- 🎛️ **ADC Driver**: 12-bit SAR ADC with continuous conversion mode
- 🔐 **Access Control**: Basic password authentication before enabling ADC logging
- 📤 **UART Transmission**: ADC values sent to a virtual terminal (9600 baud)
- 🔌 **GPIO Configuration**: Manual setup using low-level register access
- 🧪 **Simulation**: Fully tested in Proteus environment
- ⚡ **Low Power**: Optimized use of STM32 features for reduced energy consumption

---

## 🛠️ Tools Used

- **Keil uVision** – Embedded C firmware development  
- **Proteus** – Circuit simulation and validation  
- **STM32 HAL/CMSIS** – Peripheral control  
- **ST-Link** – For flashing and debugging (if using physical board)

---

## 📁 Project Structure
STM32_ADC_Logging/
├── Src/ # Source code files (main.c, adc.c, uart.c, ui.c)
├── Inc/ # Header files
├── KeilProject/ # Keil uVision project files
├── Proteus/ # .DSN Proteus simulation file
├── README.md # Project overview
└── Documentation/ # IEEE-format paper and results


## 🚀 Getting Started

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/STM32_ADC_Logging.git
   
2.Open KeilProject.uvprojx in Keil uVision.

3.Build and flash the firmware to STM32F401RE (or run in Proteus simulation).

4.Use the keypad to enter the correct password.

5.Observe ADC values on the LCD and Virtual Terminal.

📷 Demo
✅ Access Granted

❌ Access Denied

📈 Real-time ADC value logging

📚 Future Scope
1.IoT integration via BLE or Wi-Fi
2.Cloud-based data logging and monitoring
3.AI-driven predictive analytics for sensor data

👨‍💻 Contributors
Rushikesh Karupothula

Maanasa Pamarthy

Varshita B

Abid Ali Khan (Guide)






