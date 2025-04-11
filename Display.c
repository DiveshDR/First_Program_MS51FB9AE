#include "MS51_16K.H"

// Define segment pins (common anode, active low)
sbit SEG_A = P0^0; // Segment a
sbit SEG_B = P0^1; // Segment b
sbit SEG_C = P0^3; // Segment c
sbit SEG_D = P0^4; // Segment d
sbit SEG_E = P1^2; // Segment e (reassigned to P1.2, DP1)
sbit SEG_F = P1^5; // Segment f
sbit SEG_G = P1^7; // Segment g

// Define digit common pins (common anode, active high)
sbit DIGIT1 = P1^4; // Digit 1 (CC1)
sbit DIGIT2 = P1^1; // Digit 2 (CC2)
sbit DIGIT3 = P1^0; // Digit 3 (CC3)

// Segment patterns for digits 0–9 (common anode: 0 = on, 1 = off)
unsigned char code segment_patterns[10] = {
    0xC0, // 0: a,b,c,d,e,f (~0x3F)
    0xF9, // 1: b,c (~0x06)
    0xA4, // 2: a,b,d,e,g (~0x5B)
    0xB0, // 3: a,b,c,d,g (~0x4F)
    0x99, // 4: b,c,f,g (~0x66)
    0x92, // 5: a,c,d,f,g (~0x6D)
    0x82, // 6: a,c,d,e,f,g (~0x7D)
    0xF8, // 7: a,b,c (~0x07)
    0x80, // 8: a,b,c,d,e,f,g (~0x7F)
    0x90  // 9: a,b,c,d,f,g (~0x6F)
};

// Delay function (approx. ms at 16MHz Fsys, derived from 24MHz Fosc / 1.5)
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 160; j++); // Tuned for ~16MHz system clock
}

// Display a single digit at the specified position
void display_digit(unsigned char digit, unsigned char position) {
    // Turn off all digits (low for common anode)
    DIGIT1 = 0;
    DIGIT2 = 0;
    DIGIT3 = 0;

    // Set segment pattern (low = on)
    P0 = (segment_patterns[digit % 10] & 0x7F) | 0x80; // Clear P0.7, preserve pattern
    P1 = (P1 & 0xFC) | ((segment_patterns[digit % 10] >> 7) & 0x03); // Set P1.2, P1.5, P1.7

    // Activate the selected digit (high = on for common anode)
    switch (position) {
        case 1:
            DIGIT1 = 1;
            break;
        case 2:
            DIGIT2 = 1;
            break;
        case 3:
            DIGIT3 = 1;
            break;
    }
}

// Display a 3-digit number (0–999)
void display_number(unsigned int num) {
    unsigned char hundreds, tens, units;

    // Extract digits
    num = num % 1000; // Ensure number is 0–999
    hundreds = num / 100;
    tens = (num / 10) % 10;
    units = num % 10;

    // Multiplex digits
    display_digit(hundreds, 3);
    delay_ms(5); // Short delay for visibility
    display_digit(tens, 2);
    delay_ms(5);
    display_digit(units, 1);
    delay_ms(5);
}

void main(void) {
    unsigned int counter = 0;

    // Configure P0.0–P0.6 for segments (assume default push-pull)
    P0 = 0xFF;   // Initialize segments off (high for common anode)
    P0M1 = 0x00; // Quasi-bidirectional or push-pull (low nibble clear)
    P0M2 = 0x7F; // Set P0.0–P0.6 as push-pull (0b01111111)

    // Configure P1.0–P1.7 for digits and segments (assume default push-pull)
    P1 = 0x00;   // Initialize digits off (low for common anode)
    P1M1 = 0x00; // Quasi-bidirectional or push-pull
    P1M2 = 0xFF; // Set all P1 pins as push-pull (0b11111111)
    P1S = 0x00;  // Set P1 as standard I/O (not Schmitt trigger)

    while (1) {
        // Display counter (0–999)
        display_number(counter);

        // Increment counter every second
        delay_ms(1000);
        counter++;
        if (counter > 999) counter = 0;
    }
}
