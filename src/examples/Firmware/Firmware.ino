#include "MakerKit.h"

MakerKit Maker;

void setup() {
  Serial.begin(115200);
}

void loop() {
  Maker.run();
}
