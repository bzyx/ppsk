  const byte NUMBER_OF_PATTERNS = 6;
  int led = 13;  // LED mount on Arduino board
  bool startCondition = false; // Was start conditon meet?

void p(char *fmt, ... ){
        char buf[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(buf, 128, fmt, args);
        va_end (args);
        Serial.print(buf);
}

void setup() {
  Serial.begin(9600);        //Serial on 9600 baud rate 8N1
  pinMode(led, OUTPUT);      //Set the led to output
  randomSeed(analogRead(0)); //Initialize random number generator by reading unconected pin

}

void loop() {
  while(!startCondition){     //Wait for start condition
    startCondition = Serial.findUntil("B","\n\r");  // Start condition is: "B\n\r"
    delay(1);  //Short delay
  }
  
  // Start condition was meet we can send the values
  byte timeToWait = random(5,30);  //Calculate time to wait before next value between 5-30[s]
  
  char* patterns[NUMBER_OF_PATTERNS]={"0,0,1,1,1,1","0,1,1,1,1,1","0,0,1,0,1,1","1,0,1,1,1,0","1,1,1,1,1,1","0,0,0,0,0,0"};
  byte rPattern = random(NUMBER_OF_PATTERNS);

  /*  
  You should really forget about code you see here. It's gross, but acording to this 
  http://forum.arduino.cc/index.php/topic,40790.0.html Arduino doesn't link floating
  point operations to sprintf by default...
  
  This should really need to be redone using fixed point arithmetic but it's not worth of doing.

  Soory, M.
  */
  
  float rValue1 = (float)random(500,1000)/100;   int rrValue1 = (rValue1 - (int)rValue1) * 100;
  float rValue2 = (float)random(1400,1500)/100;  int rrValue2 = (rValue2 - (int)rValue2) * 100;
  float rValue3 = (float)random(1100,1300)/100;  int rrValue3 = (rValue3 - (int)rValue3) * 100;
  float rValue4 = (float)random(500,1500)/100;   int rrValue4 = (rValue4 - (int)rValue4) * 100;
  float rValue5 = (float)random(1,500)/100;      int rrValue5 = (rValue5 - (int)rValue5) * 100;
  float rValue6 = (float)random(1800,2000)/100;  int rrValue6 = (rValue6 - (int)rValue6) * 100;
  float rValue7 = (float)random(1900,2000)/100;  int rrValue7 = (rValue7 - (int)rValue7) * 100;
  float rValue8 = (float)random(1900,1950)/100;  int rrValue8 = (rValue8 - (int)rValue8) * 100;
  float rValue9 = (float)random(1850,1950)/100;  int rrValue9 = (rValue9 - (int)rValue9) * 100;
  float rValue10 = (float)random(1850,1950)/100; int rrValue10 = (rValue10 - (int)rValue10) * 100;
  float rValue11 = (float)random(700,800)/100;   int rrValue11 = (rValue11 - (int)rValue11) * 100;
  float rValue12 = (float)random(750,850)/100;   int rrValue12 = (rValue12 - (int)rValue12) * 100;
  float rValue13 = (float)random(400,650)/100;   int rrValue13 = (rValue13 - (int)rValue13) * 100;
  float rValue14 = (float)random(200,300)/100;   int rrValue14 = (rValue14 - (int)rValue14) * 100;
  
  p("%2d.%d,%2d.%d,%2d.%d,%2d.%d,%2d.%d,%2d.%d,%2d.%d,%2d.%d,%2d.%d,%s,%2d.%d,%2d.%d,%2d.%d,%2d.%d\n\r",
    (int)rValue1,rrValue1,(int)rValue2,rrValue2,(int)rValue3,rrValue3,
    (int)rValue4,rrValue4,(int)rValue5,rrValue5,(int)rValue6,rrValue6,
    (int)rValue7,rrValue7,(int)rValue8,rrValue8,(int)rValue9,rrValue9,
    patterns[rPattern],(int)rValue11,rrValue11,(int)rValue12,rrValue12,
    (int)rValue13,rrValue13,(int)rValue14,rrValue14);
  
  // put your main code here, to run repeatedly:
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  
  
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}
