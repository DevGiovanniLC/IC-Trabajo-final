uint8_t count = 0;
uint8_t directions[3] = {0x92, 0x93, 0x82};


void setup(){
}


void loop(){
	count++;

	Serial.write(directions[count%3]);
	Serial.write(count);
  delay(10000);
}