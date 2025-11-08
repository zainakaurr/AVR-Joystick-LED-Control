# AVR-Joystick-LED-Control

Joystick LED Controller

This project uses an analog joystick to control the direction of multiple LEDs using AVR assembly instructions embedded within an Arduino C program. The joystick’s X and Y axes determine which directional LEDs (up, down, left, right) turn on, and pressing the joystick button toggles a red LED. It demonstrates low-level control of I/O ports, branching logic, and analog input handling using inline assembly.

Features

- Reads analog joystick input through the ADC

- Controls LEDs for up, down, left, and right movement

- Red LED activates when the joystick button is pressed

- Demonstrates inline AVR assembly for direct port manipulation and branching

- Serial monitor outputs X and Y axis values for debugging




Hardware Setup

- Microcontroller: Arduino Uno (ATmega328P)

- Joystick module (connected to A0 and A1 for X/Y axes, digital pin 7 for button)

- LEDs connected to PORTD pins 2–6

- 220Ω resistors for each LED

Common ground shared between joystick and Arduino



How It Works

- The ADC reads X and Y analog values.

- Inline assembly (asm) instructions compare values using cpi, brlo, and brsh to detect joystick direction.

- Corresponding LEDs turn on/off depending on joystick position.

- The red LED turns on when the joystick button is pressed.

- The serial monitor displays joystick coordinates in real time.




Usage

- Upload the code to your Arduino Uno.

- Open the Serial Monitor at 9600 baud.

- Move the joystick — the LEDs will light up based on the direction, and the red LED will turn on when the button is pressed.
