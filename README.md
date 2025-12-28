# Curiosity Trainer

This repository contains the MPLAB X project for the **Curiosity Nano** training board for Kettering University students. This firmware framework provides hardware-specific libraries to streamline development for the onboard peripherals using the Microchip **XC16 compiler**.

## Recommended Configuration

* **IDE**: MPLAB X 6.25.
* **Hardware**: [Microchip Curiosity Nano](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU16/ProductDocuments/UserGuides/dsPIC33CK64MC105-Curiosity-Nano-Users-Guide-DS70005517.pdf) featuring a **dsPIC33CK64MC105** MCU. Refer to the manufacturer's datasheet for hardware specifics.
* **Device Family Pack**: This project is configured to use **dsPIC33CK-MC_DFP version 1.8.299** or newer.

## Features

The training board includes the following integrated hardware:
* **Output**: 4-digit 7-segment display, 8 LEDs, 1 RGB LED, and a Buzzer.
* **Input**: 3x4 reconfigurable keypad, 8 toggle switches, 5 push buttons, and 1 potentiometer.
* **Sensors**: Photoresistor (light sensor) and an Ultrasonic proximity sensor.
* **Communication**: UART via USB interface and via an external header.

## Curiosity Nano Pinout

This board shares several pins across multiple devices. A complete table with the pinout mapping can be found [here](https://docs.google.com/spreadsheets/d/1qIO4c7Z20XslLgPk8PVoHkfJHTRTBEOQijrODvwtQm4/edit?usp=sharing). Always ensure you are viewing the most recent sheet (indicated by the highest version number, or the sheet with the most recent date).

---

## Board Setup

### config.c / config.h
The `config.c` file handles the main initialization of the MCU. These settings were generated using the **MCC Configurator**.
* **Oscillator**: The board is set to run at **8 MHz** using the Internal Fast RC (**FRC**).
* **Debugger Guard**: The configuration includes specific settings to prevent the debugger from overriding pins, ensuring they are available for GPIO use.
* **Customization**: While not recommended, the Oscillator Source can be changed via `#pragma config FNOSC`, though this will require recalculating Timer 1 periods and PWM frequencies.

### Timer 1
Timer 1 acts as the system "heartbeat" to provide timing functions.
* **Configuration**: It uses a 1:8 prescaler. With a peripheral bus clock of 4 MHz, the timer period is set to **500** to trigger an interrupt every **1 millisecond**.
* **Variables**: Including `config.h` gives access to `msCount` (a running millisecond counter) and `tFlag` (a periodic flag).
* **Periodic Flag**: The rate of `tFlag` is defined by the `T1_TIME_MS` macro (default is 1000ms).

### utils.c / utils.h
Contains general utility functions for the board, including delay helpers and common mathematical mappings used for peripheral data.

---

## Low-Level Modules

### ADC (Analog-to-Digital Converter)
* **Resolution**: 12-bit (0 - 4095).
* **Usage**: Call `ADC_Init()` first. You must use `ADC_Set_AN9()` (or the relevant channel setter) before calling `ADC_Core_Read()`.

### PWM & SCCP
* **PWM**: Primary output is on **PWM4L**, which has been remapped to pin **RB7**.
* **SCCP**: Configured as an **Input Capture** module, primarily used to decode the echo signal from the ultrasonic sensor.

### UART
* **Interface**: Uses the Curiosity Nano's USB-to-UART bridge.
* **Functions**: Use `UartInit()` to setup and `UartSendString()` to transmit data to a serial terminal.

---

## High-Level Components

### 7-Segment Display
* **Operation**: The 4-digit display is multiplexed and refreshed every 1ms via the Timer 1 interrupt.
* **Control**: Use `print_seven_seg(d0, d1, d2, d3)` to set the digits.
* **Enable**: The display refresh can be toggled using the `seven_seg_enable` global variable.

### Keypad
* **Layout**: A 3x4 matrix (12 keys).
* **Pin Sharing**: Rows 1-4 share pins with toggle switches SW1-SW4.
* **Reading**: `readKey()` returns the key pressed. It can be configured for `KEYPAD_OUTPUT_CHAR` (e.g., '1', 'A') or `KEYPAD_OUTPUT_DECIMAL`.

### Switches & Buttons
* **Toggle Switches**: 8 active-low switches read via `read_switches()`.
* **Push Buttons**: 5 active-low buttons. Note that the direction buttons (Up, Down, Left, Right) share pins with **SW5-SW8**, while **BTN_C** is a dedicated center button.

### Sensors
* **Photoresistor & Potentiometer**: Both share pin **RA2**. The photoresistor values typically range from ~155 (bright) to ~4075 (dark).
* **Buzzer**: Connected to pin **RB7**. Use `play_note(freq)` to generate tones.
* **Ultrasonic Sensor**: Connected via **RB7** (Trigger) and **RB8** (Echo). Use `sense_distance()` to get a reading in centimeters.