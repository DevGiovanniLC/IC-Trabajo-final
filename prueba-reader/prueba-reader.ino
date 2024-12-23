uint8_t count = 0;
uint8_t sqr = 0;


void setup(){
}


void loop(){
	count++;
	sqr = pow(count,2);
	Serial.write(count);
	Serial.write(sqr);
  delay(3000);
}