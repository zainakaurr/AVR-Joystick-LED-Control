

void setup() 
{
    Serial.begin(9600);       // Enable Serial Monitor
    ADCSRA |= (1 << ADEN);    // Enable ADC
    ADMUX  |= (1 << REFS0);   // Set Vref to AVcc
    ADMUX  |= (1 << ADLAR);   // Left justify output for 8-bit mode
    ADCSRA |= (1 << ADSC);    // Start conversion

  //DDRD's
    asm("sbi 0x0A, 2");  // Set bit 2 in PORTD for output (Left LED)
    asm("sbi 0x0A, 3");  // Set bit 3 in PORTD for output (Up LED)
    asm("sbi 0x0A, 4");  // Set bit 4 in PORTD for output (Right LED)
    asm("sbi 0x0A, 5");  // Set bit 5 in PORTD for output (Down LED)
    asm("sbi 0x0A, 6");  // Set bit 6 in PORTD for output (Red LED)
    
    asm("cbi 0x0A, 7");  // Clear bit 7 in PORTD for input (Joystick Button)
    asm("sbi 0x0B, 7");  // Enable pull-up resistor on pin 7
}

uint8_t analog8(uint8_t channel)
{
    ADMUX &= 0xF0;   // Clear last 4 bits of ADMUX
    ADMUX |= channel; // Select channel
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADCH; // Return high byte (8-bit value)
}

void loop() 
{
    // Read joystick values
    int x_axis = analog8(0);
    asm("lds r16, 0x79"); // Load X-axis into r16

    int y_axis = analog8(1);
    asm("lds r17, 0x79"); // Load Y-axis into r17

  // These following lines are printing the coordinates 
    Serial.print("< ");
    Serial.print(x_axis);
    Serial.print(", ");
    Serial.print(y_axis);
    Serial.println(" >");

    // Button press
    asm("in r25, 0x09");  // Load I/O location into register 25
    asm("andi r25, 0x80"); // Logical AND with immediate constant. This will set all bits except bit 7 to 0
    asm("cpi r25, 0x80");  // Compare with immediate constant 0x80. it will set a flag
    // button pressed = low(0), button not pressed = high(1)
    asm("brne button_pressed"); // if zero flag is 0 it will branch to button_pressed
    asm("cbi 0x0B, 6");  // Turn off Red LED
    asm("rjmp check_up"); // if button not pressed, jump to checking if joystick is up

    asm("button_pressed:"); // label for if button is pressed 
    asm("sbi 0x0B, 6");  // Turn on Red LED if button pressed

    // Check Up (Y < 80)
    asm("check_up:"); // label for checking if joystick is up
    asm("cpi r17, 80"); // compare the valye in the Y-axis, register 17, with 80 and set a flag
    asm("brsh no_up"); // Branch if same or higher, so ifr17>= 80, branch to no_up
    asm("sbi 0x0B, 3");  // Turn on Up LED
    asm("rjmp check_down"); // jump to checking if down LED is also on or something 

    asm("no_up:"); // Label for if up LED is not on
    asm("cbi 0x0B, 3");  // Turn off Up LED

    // Check Down (Y > 175)
    asm("check_down:"); // Label for checking down LED
    asm("cpi r17, 175"); // Compare r17 with 175 and set flag 
    asm("brlo no_down"); // if r17 < 175 branch to no_down
    asm("sbi 0x0B, 5");  // Turn on Down LED
    asm("rjmp check_left"); // jump to check_left to check if joystick is towards the left

    asm("no_down:"); // if r17 < 175 branch to this label
    asm("cbi 0x0B, 5");  // Turn off Down LED

    // Check Left 
    asm("check_left:"); // label for checking if joystick is towards the left 
    asm("cpi r16, 80"); // compare r16 to 80
    asm("brsh no_left"); // Branch if same or higher so if X > 80 branch to not left 
    asm("sbi 0x0B, 2");  // Turn on Left LED if not branched to no_left
    asm("rjmp check_right"); // jump to check if right LED is supposed to be on

    asm("no_left:"); // label for if joystick is not towards the left 
    asm("cbi 0x0B, 2");  // Turn off Left LED

    // Check Right (X > 175)
    asm("check_right:"); // Label for joystick which is towards the right 
    asm("cpi r16, 175"); // compare r16 with 175 and set flags 
    asm("brlo no_right"); // Branch if lower so if r16 < 175, branch to the label no_right
    asm("sbi 0x0B, 4");  // Turn on Right LED
    asm("rjmp end_loop"); // jump to the label end_loop 

    asm("no_right:"); // Label for right LED not having to be on 
    asm("cbi 0x0B, 4");  // Turn off Right LED

    asm("end_loop:"); // label for end loop 
    delay(2); // delay
}

