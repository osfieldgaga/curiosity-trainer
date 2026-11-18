# Curiosity Trainer

This repository contains the MPLAB X project for the **Curiosity Nano** training board for Kettering University students. This firmware framework provides hardware-specific libraries to streamline development for the onboard peripherals using the Microchip **XC16 compiler**.

## Recommended Configuration

* **IDE**: VS Code with the MPLAB Extension Pack (tested with VS Code 1.144.0 and the pack 1.2.9), see [Installing MPLAB in VS Code](docs/Installing%20MPLAB%20in%20VS%20Code.docx). The MPLAB X project files (`nbproject/`) also work with MPLAB X IDE 6.05 or newer.
* **Compiler**: MPLAB XC16 v2.10.
* **Hardware**: [Microchip Curiosity Nano](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU16/ProductDocuments/UserGuides/dsPIC33CK64MC105-Curiosity-Nano-Users-Guide-DS70005517.pdf) featuring a **dsPIC33CK64MC105** MCU. Refer to the manufacturer's datasheet for hardware specifics.
* **Device Family Pack**: **dsPIC33CK-MC_DFP 1.11.412** for the VS Code project (the MPLAB X project files use 1.5.126), and the **nEDBG_TP** tool pack for the board's debugger. The exact tested versions are in [docs/dev-environment](docs/dev-environment/).

## Features

The training board includes the following integrated hardware:
* **Output**: 4-digit 7-segment display, 8 LEDs, 1 RGB LED, and a Buzzer.
* **Input**: 3x4 reconfigurable keypad, 8 toggle switches, 5 push buttons, and 1 potentiometer.
* **Sensors**: Photoresistor (light sensor), Ultrasonic proximity sensor, Microphone and MMA8451 accelerometer.
* **Communication**: UART via USB interface and via an external header.

## Documentation

The [docs](docs/) folder contains:
* **[Curiosity Nano Programming Reference](docs/Curiosity%20Nano%20Programming%20Reference.docx)**: the full reference for every driver, including pins, functions, example code and common problems.
* **[IO_PINS_Mapped.xlsx](docs/IO_PINS_Mapped.xlsx)**: the IO map of the training board.
* **[Installing MPLAB in VS Code](docs/Installing%20MPLAB%20in%20VS%20Code.docx)**: how to set up VS Code, the MPLAB extensions and XC16, with the reference configuration.
* **[manual](docs/manual/)**: the unified LaTeX manual (KOMA-Script): the VS Code installation guide followed by the programming reference.
* **[dev-environment](docs/dev-environment/)**: snapshot of that configuration (exact versions, extension list and install scripts, the project's VS Code settings, Linux USB rule).

## Curiosity Nano Pinout

This board shares several pins across multiple devices. A complete table with the pinout mapping can be found [here](https://docs.google.com/spreadsheets/d/1qIO4c7Z20XslLgPk8PVoHkfJHTRTBEOQijrODvwtQm4/edit?usp=sharing) and in [docs/IO_PINS_Mapped.xlsx](docs/IO_PINS_Mapped.xlsx). Always ensure you are viewing the most recent sheet (indicated by the highest version number, or the sheet with the most recent date).

---

## Board Setup

### config.c / config.h
The `config.c` file handles the main initialization of the MCU. These settings were generated using the **MCC Configurator**.
* **Oscillator**: The board boots from the Internal Fast RC (**FRC**, 8 MHz), then `main()` switches to FRC + PLL: **Fosc = 32 MHz**, **Fcy = Fp = 16 MHz** (`FCY` in `config.h`). On the dsPIC33CK, Fosc is half of the PLL output (64 MHz here).
* **Debugger Guard**: The configuration includes specific settings to prevent the debugger from overriding pins, ensuring they are available for GPIO use.
* **Customization**: While not recommended, the clock can be changed via `#pragma config FNOSC` and the PLL setup in `main()`, though this will require updating `FCY`, Timer 1, the SCCP prescaler, the PWM clock divider and the neopixel `Nop()` timing.

### Timer 1
Timer 1 acts as the system "heartbeat" to provide timing functions.
* **Configuration**: It uses a 1:8 prescaler. With a peripheral bus clock of 16 MHz, the timer period is set to **1999** to trigger an interrupt every **1 millisecond**.
* **Variables**: Including `config.h` gives access to `msCount` (a running millisecond counter) and `tFlag` (a periodic flag).
* **Periodic Flag**: The rate of `tFlag` is defined by the `T1_TIME_MS` macro (default is 1000ms).
* **Period math**: Fp = Fosc / 2 = 16 MHz, and the timer clock is 16 MHz / 8 = 2 MHz. One millisecond is 2000 ticks, and since the timer counts from 0 up to and including its period, `T1_PERIOD` = 2000 - 1 = 1999.

### FCY and delays
`FCY` (16 MHz) is defined in `config.h`. It is used by `__delay_ms()` / `__delay_us()` from `libpic30.h` and by the UART baud rate calculation, so always include `config.h` before `libpic30.h`.

### utils.c / utils.h
Contains general utility functions for the board, including delay helpers and common mathematical mappings used for peripheral data.

---

## Low-Level Modules

### ADC (Analog-to-Digital Converter)
* **Resolution**: 12-bit (0 - 4095).
* **Usage**: Call `ADC_Init()` first. You must use `ADC_Set_AN9()` (or the relevant channel setter) before calling `ADC_Core_Read()`.

### PWM & SCCP
* **PWM**: Primary output is on **PWM4L**, which has been remapped to pin **RB7**. PWM4 counts at Fosc / 16 = **2 MHz** (`FP` in `pwm.h`), so the period register value for a frequency f is `FP / f - 1`. The 16-bit period register covers frequencies down to about 31 Hz. `PWM4_Disable()` turns PWM4 off and releases RB7.
* **SCCP**: Configured as an **Input Capture** module, primarily used to decode the echo signal from the ultrasonic sensor. The capture timer runs at Fcy / 16 = **1 MHz** (1 tick = 1 us).

```c
PWM4_Initialize();         // 130.8 Hz, 50% duty cycle on RB7
PG4PER = (FP / 1000) - 1;  // 1 kHz
PG4DC = PG4PER / 2;        // 50% duty cycle
PG4STATbits.UPDREQ = 1;    // apply the new period and duty cycle
```

### UART
* **Interface**: Uses the Curiosity Nano's USB-to-UART bridge.
* **Pins**: U1TX on **RC10** (RP58) and U1RX on **RC11** (RP59), wired to the debugger's virtual COM port. Settings: **9600 8N1**.
* **Functions**: Use `UartInit()` to setup and `UartSendString()` / `UartSendByte()` to transmit data to a serial terminal. `UartDataAvailable()` and `UartReadByte()` receive data. `UartInitBaud(baud)` sets any other baud rate and `UartSendBuffer()` sends raw bytes (zeros included).
* **Baud rate**: computed from `FCY` with the fine (BRGH = 1) divider: 9600 gives 9592 baud, 115200 gives 114286 baud and 250000 is exact. The debugger's USB bridge drops bytes at 500000 baud and above.
* **PPS**: some drivers lock the PPS registers (e.g. `PWM4_Disable()`), and writes to `RPORx` / `RPINRx` made while PPS is locked are silently ignored. Code that remaps pins must unlock PPS with `__builtin_write_RPCON(0x0000)` and lock it again with `__builtin_write_RPCON(0x0800)`. `UartInit()`, `SCCP_Init()` and the PWM4 functions already do this.

```c
UartInit();
UartSendString("\r\nWelcome!\r\n");
while (1) {
    if (UartDataAvailable()) {
        char c = UartReadByte();
        UartSendByte(c); // echo the byte back
    }
}
```

### I2C (software master)
* **Bus**: SDA on **RC8**, SCL on **RC9** (open drain), driven in software by Microchip's dsPIC33C I2C library (`i2c_master.c`), about 121 kHz (measured: 76 us per byte). Shared by the OLED (0x3C), the IO expander (0x20) and the accelerometer (0x1D).
* **Functions**: `I2CM_Init()`, `I2CM_Start()`, `I2CM_Write(byte)` (returns 0 = ACK, 1 = NACK), `I2CM_Read(ack)` (`ack = 1` for the last byte), `I2CM_Stop()`.

---

## High-Level Components

### OLED Display
* **Display**: Winstar WEA012864DWPP3N00003, 128 x 64 pixels, SSD1306BZ controller, I2C address 0x3C. 8 text lines (pages 0-7) of 21 characters (5 x 8 font).
* **Frame buffer**: the drawing functions (`oled_draw_string()`, `oled_draw_char()`, `oled_set_pixel()`, `oled_draw_line()`, `oled_clear()`...) only change a 1 KB buffer in RAM; `oled_update()` sends it to the screen. Measured: `oled_update()` 78.5 ms, `oled_draw_string()` 10.2 ms, `oled_clear()` 46.9 ms, so about 12 refreshes per second at most: draw what changed, then update once.
* **Bitmaps**: `oled_draw_bitmap(image)` shows a 1024-byte image, converted with image2cpp (vertical, 1 bit per pixel).
* **Note**: `init_oled()` sets RB8 (microphone) to digital, call `mic_init()` after it.

### IO Expander
* **Device**: TI TCA9534, 8 extra pins (P0-P7) at I2C address 0x20. Registers: 0x00 input port, 0x01 output port, 0x02 polarity inversion, 0x03 configuration (1 = input). Push-pull outputs (25 mA sink max), **no internal pull-ups** on the inputs.
* **Functions**: `init_IO_expander()` makes the 8 pins inputs and returns 1 if the chip answered; `io_read()` / `io_read_pin(pin)` read levels (bit i = pin Pi); `io_set_direction(inputs)` / `io_pin_mode(pin, IOEXP_INPUT or IOEXP_OUTPUT)` set directions; `io_write(value)` / `io_write_pin(pin, level)` drive outputs; `io_cmd(reg, data)` / `io_read_reg(reg)` access any register.

### 7-Segment Display
* **Operation**: The 4-digit display is multiplexed and refreshed every 1ms via the Timer 1 interrupt.
* **Control**: Use `print_seven_seg(d0, d1, d2, d3)` to set the digits.
* **Enable**: The display refresh can be toggled using the `seven_seg_enable` global variable.

### Keypad
* **Layout**: A 3x4 matrix (12 keys).
* **Pin Sharing**: Rows 1-4 share pins with toggle switches SW1-SW4.
* **Reading**: `readKey()` returns the key pressed. It can be configured for `KEYPAD_OUTPUT_CHAR` (e.g., '1', 'A') or `KEYPAD_OUTPUT_DECIMAL`.
* **Switches SW1-SW4**: the keypad and SW1-SW4 can't be used at the same time. Every switch that is ON ties its row to the other rows that are ON, so `readKey()` returns the wrong key, or a key that isn't pressed. Turn SW1-SW4 OFF while using the keypad, and call `keypad_check_rows()` before `readKey()`: it returns a bitmask of the affected rows, 0 if the keypad can be used. Example 1 shows "Keypad blocked!" on the OLED until the switches are off (the message calls them SW0-SW3).

```c
init_keypad();
while (1) {
    if (keypad_check_rows()) {
        continue; // SW1-SW4 are ON: ask the user to turn them off
    }
    unsigned char key = readKey(KEYPAD_OUTPUT_CHAR);
}
```

### Switches & Buttons
* **Toggle Switches**: 8 active-low switches read via `read_switches()`.
* **Push Buttons**: 5 active-low buttons. Note that the direction buttons (Up, Down, Left, Right) share pins with **SW5-SW8**, while **BTN_C** is a dedicated center button.

### Sensors
* **Photoresistor & Potentiometer**: Both share pin **RA2**. The photoresistor values typically range from ~155 (bright) to ~4075 (dark).
* **Buzzer**: Connected to pin **RB7** and driven by PWM4. Use `play_note(freq)` to generate tones from about 31 Hz upwards. The note macros in `buzzer.h` range from C3 (130.81 Hz) to C6 (1046.50 Hz), and `Buzzer_Init()` starts at C3.
* **Ultrasonic Sensor**: Connected via **RB7** (Trigger, output) and **RB8** (Echo, input). Use `sense_distance()` to get a reading in centimeters. It returns **7777** if no echo is received (sensor not connected), **8888** if the end of the echo is missing and **9999** if the echo is shorter than the 150 us minimum.

### Accelerometer (MMA8451)
* **Bus**: shares the software I2C bus (`i2c_master.c`, SDA on **RC8**, SCL on **RC9**) with the OLED and the IO expander. Default address **0x1D** (`MMA8451_ADDR`), 0x1C if the A pin of the breakout is tied to GND.
* **Setup**: +/-2 g range, 14 bits (4096 counts per g), 800 Hz, high resolution oversampling.
* **Functions**: `init_accelerometer()` returns 1 if the sensor answered, `accel_read(&data)` reads raw X/Y/Z, `accel_to_mg(raw)` converts to milli-g. Example 16 also shows the pitch and roll angles on the OLED.

### Microphone
* **Pin**: **RB8** (AN10), shared with the ultrasonic echo through a selector switch: set it to the microphone. The output is biased at about 1.75 V.
* **Capture**: `mic_init(sample_rate)` sets up the ADC and uses SCCP2 as the sample timer. `mic_capture(buf, len)` fills a frame of evenly spaced samples (the Timer 1 interrupt is held during the capture to avoid jitter), and `mic_send_frame(buf, len, seq)` sends it over UART.
* **Defaults** (`mic.h`): 8000 Hz, 512 samples per frame, **115200 baud** (a standard rate MATLAB accepts everywhere), about 6.5 frames/s. 250000 baud is exact and faster (~9.5 frames/s) but MATLAB on Linux rejects it.
* **Frame format** (little endian): `0xA5 0x5A | seq (u16) | len (u16) | sample rate (u16) | len samples (u16, 0-4095) | checksum (u16, sum of the samples)`.
* **Sample rate range**: 245 Hz (16-bit SCCP2 period) to 65535 Hz (16-bit parameter). Each sample takes at most 103 cycles (~6.4 us, measured), and conversions start within ~1 us of their time slot.
* **Example 17** ("Microphone" in the demo cycle) streams frames and shows the level on the OLED.
* **Under the hood**: the programming reference's Microphone > *How it works* section details the signal chain, ADC and timer setup, timing and jitter, sample rate limits, frame format, throughput math and the viewers' spectrum processing.

```c
static uint16_t __attribute__((far)) frame[MIC_FRAME_LEN]; // static + far, see Troubleshooting
mic_init(MIC_SAMPLE_RATE);
UartInitBaud(MIC_UART_BAUD);
while (1) {
    mic_capture(frame, MIC_FRAME_LEN);
    mic_send_frame(frame, MIC_FRAME_LEN, seq++);
}
```

---

## Tools

The [tools](tools/) folder contains viewers for the microphone frames, both plot the waveform and its spectrum:
* **Python**: `pip install pyserial numpy matplotlib`, then `python tools/mic_viewer.py --port COM5` (or `/dev/ttyACM0`). `--check 20` prints frame statistics without plotting, `--record frames.csv` saves every frame, `--baud` must match `MIC_UART_BAUD`.
* **MATLAB**: from the `tools` folder, `mic_viewer("COM5")` plots live until the figure is closed, and `frames = mic_viewer("COM5", 115200, 20)` returns 20 frames without plotting.

---

## Troubleshooting

### Debugger fails to start with `ImportError: No module named spitfireinterface`
Use at least MPLAB X 6.05 with the dsPIC33CK-MC_DFP 1.6.131 and nEDBG_TP 1.11.554 packs, then go to **Tools > Plugins**, check for updates, install them and restart MPLAB X.

### "The target device is not ready for debugging"
In `config.c`, make sure `#pragma config ICS = PGD3`. Sometimes, unplugging and replugging the board helps.

### UART shows nothing, or only random characters
* Open the debugger's virtual COM port (`/dev/ttyACM0` on Linux, the "Curiosity Nano" COM port on Windows) at **9600 8N1**. On Linux, if the port was open while the board was being programmed, it may come back as `/dev/ttyACM1`.
* Random characters mean a baud rate mismatch: `FCY` in `config.h` must match the actual clock (Fcy = 16 MHz).
* No output at all usually means the UART pins were not mapped: PPS must be unlocked when `UartInit()` runs.

### Keypad returns wrong keys, or example 1 shows "Keypad blocked!"
Turn the slide switches SW1-SW4 OFF: they share their pins with the keypad rows. In your own code, call `keypad_check_rows()` before `readKey()` (see Keypad).

### Link Error: Could not allocate section .ndata (or .nbss)
The 4 KB of near data memory is full. Declare large arrays `static` with `__attribute__((far))`, and never define variables in a header file: `static uint8_t fb[1024]` used to be in `oled.h` and `io_expander.h`, which created a separate 1 KB copy in every file including them.

---

## License

Copyright (c) 2025-2026 Kettering University, Osfield Gaga and Jeremy Gooch. All rights reserved, see [LICENSE](LICENSE). Changes between versions are listed in [CHANGELOG.md](CHANGELOG.md), and the notes of the current release in [RELEASE_NOTES.md](RELEASE_NOTES.md).
