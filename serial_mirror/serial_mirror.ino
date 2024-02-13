const long baudRate = 57600; 
int incomingByte;

void  setup()
{
  Serial.begin(baudRate);
}


void loop()
{
  if(Serial.available() != 0)
  {
    incomingByte = Serial.read();
    Serial.write(incomingByte);    
  }
}
