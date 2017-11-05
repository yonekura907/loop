
#define KNOBNUM (2)
int knobLevels[KNOBNUM];

void setup(){
  Serial.begin(115200);
}

void loop(){
  sendKnobLevels();
}

void sendKnobLevels() {
  /* OFに送る */
  for (int index = 0; index <= KNOBNUM; index++) {
    setKnobLevel(index);
    Serial.write(knobLevels[index]);
  }

  /* OF側できれいに受け取るために遅らせる */
  delay(10);
}

/* knobs 500-1023 */
int setKnobLevel(int knobPinNum) {
  int value = analogRead(knobPinNum);
  if (value <= 518) {
    knobLevels[knobPinNum] = 0;
  } else if ((530 < value) && (value <= 605)) {
    knobLevels[knobPinNum] = 1;
  } else if ((620 < value) && (value <= 692)) {
    knobLevels[knobPinNum] = 2;
  } else if ((750 < value) && (value <= 825)) {
    knobLevels[knobPinNum] = 3;
  } else if ((830 < value) && (value <= 905)) {
    knobLevels[knobPinNum] = 4;
  } else if ((910 < value) && (value <= 935)) {
    knobLevels[knobPinNum] = 5;
  } else if ((940 < value) && (value <= 965)) {
    knobLevels[knobPinNum] = 6;
  } else if ((970 < value) && (value <= 995)) {
    knobLevels[knobPinNum] = 7;
  } else if (1001 < value) {
    knobLevels[knobPinNum] = 8;
  }
}

