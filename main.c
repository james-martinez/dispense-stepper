/* ------------------------------------------------------------
 *
 * 8-step half-step drive for ATtiny13/13A
 * PB1-PB4 : stepper-driver inputs  (LSB = PB1)
 * PB0     : push-button to GND
 *
 * Hold the button at power-up → run in reverse
 * Press/hold the button after power-up → run forward (or reverse)
 * Release the button → motor stops (coils de-energised)
 *
 * Hardware used for testing:
 *  ATtiny13A + ULN2003 + 28BYJ-48 5 V stepper
 * ------------------------------------------------------------ */

#define F_CPU 9600000UL /* internal RC @ 9.6 MHz          */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

/* Half-step pattern for coils on PB4…PB1  (bit0 is PB1, bit3 is PB4) */
static const uint8_t stepTable[8] = {
    0b0001, /* A  */
    0b0011, /* A+B */
    0b0010, /* B  */
    0b0110, /* B+C */
    0b0100, /* C  */
    0b1100, /* C+D */
    0b1000, /* D  */
    0b1001  /* D+A */
};

volatile uint8_t state = 0;     /* current step index 0…7          */
volatile uint8_t direction = 0; /* 0 = forward, 1 = reverse        */

/* ------------------------------------------------------------ */
/* Timer-0 Compare-Match ISR: executes every ~10 ms when button
 * is held low.  Generates one half-step per interrupt.          */
ISR(TIM0_COMPA_vect)
{
    /* If button is released, just de-energise coils and exit */
    if (PINB & (1 << PB0))
    {                        /* PB0 high ⇒ button up    */
        PORTB &= (1 << PB0); /* clear PB4..PB1, keep PB0 */
        return;
    }

    /* Advance or rewind step index */
    if (direction)
        state = (state == 0) ? 7 : state - 1;
    else
        state = (state + 1) & 0x07;

    /* Build output byte: keep PB0, update PB4..PB1 */
    uint8_t out = stepTable[state] << 1; /* shift to PB4..PB1     */
    PORTB &= (1 << PB0);                 /* clear outputs, keep PB0 */
    PORTB |= out;
}

/* ------------------------------------------------------------ */
static void timer0_init(void)
{
    /* CTC mode */
    TCCR0A = _BV(WGM01);

    OCR0A = 93; /* 94 ticks → 10 ms           */

    /* clk / 1024 prescaler (CS02 | CS00) */
    TCCR0B = _BV(CS02) | _BV(CS00);

    TIMSK0 = _BV(OCIE0A); /* enable compare–match IRQ   */
}
/* ------------------------------------------------------------ */
int main(void)
{
    /* PB1-PB4 outputs, PB0 input with pull-up */
    DDRB = (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4);
    PORTB = (1 << PB0); /* enable pull-up on PB0         */

    /* Determine direction at power-up (button held ⇒ reverse) */
    if (!(PINB & (1 << PB0)))
        direction = 1;

    timer0_init();
    sei(); /* global interrupt enable       */

    /* Idle loop – sleep until an interrupt occurs */
    set_sleep_mode(SLEEP_MODE_IDLE);
    for (;;)
        sleep_mode();
}
