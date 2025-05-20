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

