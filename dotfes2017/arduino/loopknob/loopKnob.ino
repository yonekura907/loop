
#define KNOBNUM (5)
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
//    Serial.println(knobLevels[index]);
  }

  /* OF側できれいに受け取るために遅らせる */
  delay(10);
}

/* knobs 500-1023 */
int setKnobLevel(int knobPinNum) {
  int value = analogRead(knobPinNum);
  if (value <= 558) {
    knobLevels[knobPinNum] = 1;
  } else if ((562 < value) && (value <= 692)) {
    knobLevels[knobPinNum] = 2;
  } else if ((696 < value) && (value <= 892)) {
    knobLevels[knobPinNum] = 3;
  } else if ((896 < value) && (value <= 933)) {
    knobLevels[knobPinNum] = 4;
  } else if ((937 < value) && (value <= 973)) {
    knobLevels[knobPinNum] = 5;
  } else if (977 < value) {
    knobLevels[knobPinNum] = 6;
  }
}

