static char *const LED = (char *)0x10;
static char *const BTN = (char *)0x13;

void start() {
  *LED = 1;
  *LED = 3;
  *LED = 0;
  *LED = 5;
  *LED = 0;
  *LED = 0;
  *LED = 0;
  *LED = *BTN;
  *LED = 0;
}
