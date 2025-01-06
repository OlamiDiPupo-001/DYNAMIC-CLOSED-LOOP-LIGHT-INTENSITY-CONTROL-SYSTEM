# Dynamic Closed-Loop Light Intensity Control System 🌟✨💡

## Overview
This project implements a Dynamic Closed-Loop Light Intensity Control System designed to dynamically regulate the output intensity of an LED diode based on desired light conditions. By leveraging sensor feedback and PWM-based actuation, the system provides efficient and adaptive light control for various applications
 
---

## Features 🌟✨💡
- Real-time light intensity adjustment with a BH1750 sensor.
- LED control via STM32 microcontroller and PWM.
- User-friendly interface with a 2x16 LCD display.
- Manual adjustments with a potentiometer.
- High accuracy with <1% steady-state error. 

---

## Key Components 🌟✨💡
1. **STM32F746ZG**: System controller.
2. **BH1750 Sensor**: Measures light intensity.
3. **2N7000 MOSFET**: Drives LED with PWM.
4. **LCD Display**: Displays system status.
5. **LED Diode**: Primary output.
6. **Potentiometer**: Manual input. 

---

## System Design 🌟✨💡
### Control Strategy
- Measures light with BH1750 sensor.
- Adjusts LED intensity via STM32 and PWM.
- Maintains consistent lighting. 

### Communication
- **I2C**
- **GPIO**
- **USART3**
- **ADC**
- **DMA**

---

## Software 🌟✨💡
- Developed in **STM32CubeIDE**.
- Modular source code with GitHub version control. 

---

## Contributors
- **Azeez Oladipupo Akinlade**
- **Daniel Manga**

**Instructor**: Adrian Wójcik  
