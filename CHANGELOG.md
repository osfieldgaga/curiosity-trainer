# Changelog

All notable changes to the Curiosity Trainer firmware and its documentation are recorded in this file.
The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) and the project uses
[Semantic Versioning](https://semver.org/).

## [1.0.0] - 2026-11-17

First release. It gathers the development work since November 2025 into a tested firmware framework
for the Curiosity Trainer board (Curiosity Nano dsPIC33CK64MC105), with its documentation.

### Added

- **Drivers** for every device of the board:
  - Low level: clock and Timer 1 (1 ms tick, `msTicks`, `tFlag`), ADC, PWM, SCCP input capture, UART
    (debugger's virtual COM port, any baud rate, receive functions) and a software I2C bus.
  - High level: keypad, 4-digit 7-segment display, switches, LEDs and RGB LED, push buttons,
    photoresistor, potentiometer, buzzer, ultrasonic distance sensor, addressable LEDs (neopixels),
    128 x 64 OLED display, TCA9534 IO expander, MMA8451 accelerometer and microphone.
- **IO expander writing**: pin directions (`io_set_direction()`, `io_pin_mode()`), outputs (`io_write()`,
  `io_write_pin()`), single-pin and register reads (`io_read_pin()`, `io_read_reg()`), register names.
- **Microphone** driver (`mic.c`): evenly spaced ADC samples timed by SCCP2, frames sent over UART.
- **Keypad check**: `keypad_check_rows()` detects switches SW1-SW4 interfering with the keypad rows.
- **Demo cycle** of 16 examples, the center button moves to the next one, including "UART Hello
  World" and "Microphone".
- **Tools**: `tools/mic_viewer.py` (Python) and `tools/mic_viewer.m` (MATLAB), live waveform and
  spectrum of the microphone frames.
- **Documentation**:
  - Programming reference (`docs/Curiosity Nano Programming Reference.docx`): every driver with pins,
    functions, examples, internals of the microphone, I2C, OLED and IO expander, common problems.
  - Installation guide for VS Code and the MPLAB extensions (`docs/Installing MPLAB in VS Code.docx`).
  - Unified LaTeX manual (KOMA-Script) combining both, with the scripts that regenerate it
    (`docs/manual`).
  - Snapshot of the tested development environment with install scripts (`docs/dev-environment`).
  - IO map of the board (`docs/IO_PINS_Mapped.xlsx`) and component datasheets (`docs/datasheets`).
- `LICENSE` and this changelog.

### Changed

- The CPU now runs at its intended speed: Fosc = 32 MHz, Fcy = Fp = 16 MHz (PLL post divider 2).
  All the timing depends on it, see Fixed.
- `UartInit()` computes the baud rate from `FCY` with the fine divider (9600 gives 9592 baud).
  `UartInitBaud()`, `UartSendBuffer()`, `UartDataAvailable()` and `UartReadByte()` were added.
- PWM4 (buzzer) counts at Fosc / 16 = 2 MHz (`FP` in `pwm.h`), so notes from about 31 Hz up fit the
  16-bit period register.
- `init_IO_expander()` returns 1 when the TCA9534 answers, 0 otherwise. Example 14 shows
  "TCA9534 not found" on the OLED when it does not.
- `ADC_Init()` returns `void` (it never returned a value).
- The demo starts on the default screen (bulldog and uptime).

### Fixed

- **Clock**: the PLL gave Fcy = 8 MHz while the code assumed 16 MHz, so every delay was twice as
  long, Timer 1 ticked every 2 ms, the UART ran at 4800 baud and the ultrasonic distances were halved.
- **UART**: nothing was sent, because the pin mapping (PPS) was written while PPS was locked by
  `PWM4_Disable()`. The UART now unlocks PPS, sets the pin directions and waits on the transmit buffer.
- **Ultrasonic sensor**: the echo input mapping was ignored for the same PPS reason.
- **Buzzer**: notes played about two octaves too high (wrong PWM clock assumption).
- **Neopixels**: bit timing adjusted for the 16 MHz clock (measured within the SK6812 specification).
- **RAM**: the OLED frame buffer was defined in `oled.h` and `io_expander.h`, giving every file that
  included them its own 1 KB copy and filling the near data memory. It is now defined once.
- **PWM**: `PWM1_Initialize()` cleared PWM4's output register and `PWM3_Initialize()` turned on PWM4.
- **Build**: `accelerometer.c` was missing from the MPLAB X project. The 65 compiler warnings
  (missing prototypes and includes, nested comments) are gone.
- Comments and documentation: real OLED refresh rate, PWM frequencies, file names in the headers.

### Known limitations

- A full OLED update takes about 79 ms (measured), so the screen refreshes about 12 times per second
  at most. `oled_clear()` takes about 47 ms.
- The microphone sends frames with gaps between them (about 6.5 frames per second at 115200 baud),
  not a continuous recording.
- The debugger's USB serial port drops bytes at 500000 baud and above.
- `init_oled()` sets RB8, the microphone input, to digital: call `mic_init()` after it.
- The TCA9534 has no internal pull-ups: unconnected input pins read random levels.
- Examples 6 (buzzer song) and 7 (button counter) are not part of the demo cycle.
- Not yet verified on hardware: IO expander outputs driving a load, neopixel colors and buzzer
  pitch after the clock changes.

[1.0.0]: https://github.com/osfieldgaga/curiosity-trainer/releases/tag/v1.0.0
