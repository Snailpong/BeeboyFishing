#include <TM1637Display.h>

#define CLK_SEG 8
#define DIO_SEG 9

int t = 0;

TM1637Display dsp(CLK_SEG, DIO_SEG);

void setup() {
  dsp.setBrightness(7);
}

void loop() {
  t = (++t) % 1500;
  delay(10);
  dsp.showNumberDec(t);
}
