#include "MakerKit.h"

MakerKit KITS;
void setup() {
  Serial.begin(9600);

}

void loop() {
  KITS.setLED(2,1);
  delay(1000);
  KITS.setLED(2,0);
}
