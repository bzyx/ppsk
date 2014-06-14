  const byte NUMBER_OF_PATTERNS = 6;
  int led = 13;  // LED mount on Arduino board
  bool startCondition = false; // Was start conditon meet?

void p(char *fmt, ... ){
/*
  Prepare a string like using sprintf and send it via Serial 
*/
  char buf[128];
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
  byte timeToWait = random(2,7);  //Calculate time to wait before next value between 5-30[s]
  
  char* patterns[NUMBER_OF_PATTERNS]={"0,0,1,1,1,1","0,1,1,1,1,1","0,0,1,0,1,1","1,0,1,1,1,0","1,1,1,1,1,1","0,0,0,0,0,0"};
  byte rPattern = random(NUMBER_OF_PATTERNS);

  /*  
  You should really forget about code you see here. It's gross, but acording to this 
  http://forum.arduino.cc/index.php/topic,40790.0.html Arduino doesn't link floating
  point operations to sprintf by default...
  
  This should really need to be redone using fixed point arithmetic but it's not worth of doing.

  Soory, M.
  */
  
  float rValue1 = (float)random(500,1000)/100;   int rrValue1 = (rValue1 - (int)rValue1) * 100; rrValue1 = rrValue1<9?(rrValue1+1)*10:rrValue1;
  float rValue2 = (float)random(1400,1500)/100;  int rrValue2 = (rValue2 - (int)rValue2) * 100; rrValue2 = rrValue2<9?(rrValue2+1)*10:rrValue2;
  float rValue3 = (float)random(1100,1300)/100;  int rrValue3 = (rValue3 - (int)rValue3) * 100; rrValue3 = rrValue3<9?(rrValue3+1)*10:rrValue3;
  float rValue4 = (float)random(500,1500)/100;   int rrValue4 = (rValue4 - (int)rValue4) * 100; rrValue4 = rrValue4<9?(rrValue4+1)*10:rrValue4;
  float rValue5 = (float)random(1,500)/100;      int rrValue5 = (rValue5 - (int)rValue5) * 100; rrValue5 = rrValue5<9?(rrValue5+1)*10:rrValue5;
  float rValue6 = (float)random(1800,2000)/100;  int rrValue6 = (rValue6 - (int)rValue6) * 100; rrValue6 = rrValue6<9?(rrValue6+1)*10:rrValue6;
  float rValue7 = (float)random(1900,2000)/100;  int rrValue7 = (rValue7 - (int)rValue7) * 100; rrValue7 = rrValue7<9?(rrValue7+1)*10:rrValue7;
  float rValue8 = (float)random(1900,1950)/100;  int rrValue8 = (rValue8 - (int)rValue8) * 100; rrValue8 = rrValue8<9?(rrValue8+1)*10:rrValue8;
  float rValue9 = (float)random(1850,1950)/100;  int rrValue9 = (rValue9 - (int)rValue9) * 100; rrValue9 = rrValue9<9?(rrValue9+1)*10:rrValue9;
  float rValue10 = (float)random(1850,1950)/100; int rrValue10 = (rValue10 - (int)rValue10) * 100; rrValue10 = rrValue10<9?(rrValue10+1)*10:rrValue10;
  float rValue11 = (float)random(700,800)/100;   int rrValue11 = (rValue11 - (int)rValue11) * 100; rrValue11 = rrValue11<9?(rrValue11+1)*10:rrValue11;
  float rValue12 = (float)random(750,850)/100;   int rrValue12 = (rValue12 - (int)rValue12) * 100; rrValue12 = rrValue12<9?(rrValue12+1)*10:rrValue12;
  float rValue13 = (float)random(400,650)/100;   int rrValue13 = (rValue13 - (int)rValue13) * 100; rrValue13 = rrValue13<9?(rrValue13+1)*10:rrValue13;
  float rValue14 = (float)random(200,300)/100;   int rrValue14 = (rValue14 - (int)rValue14) * 100; rrValue14 = rrValue14<9?(rrValue14+1)*10:rrValue14;
  float rValue15 = (float)random(-200,200)/100;  int rrValue15 = abs((rValue15 - (int)rValue15) * 1000);

  digitalWrite(led, HIGH);   // turn the LED
  // Send the string from pseudo-"ABTOW"
  p("%2d.%d,%2d.%d,%2d.%d,%2d.%d,%2d.%d,%2d.%d,%2d.%d,%2d.%d,%2d.%d,%s,%2d.%d,%2d.%d,%2d.%d,%2d.%d, %2d.%d\n\r",
    (int)rValue1,rrValue1,(int)rValue2,rrValue2,(int)rValue3,rrValue3,
    (int)rValue4,rrValue4,(int)rValue5,rrValue5,(int)rValue6,rrValue6,
    (int)rValue7,rrValue7,(int)rValue8,rrValue8,(int)rValue9,rrValue9,
    patterns[rPattern],(int)rValue11,rrValue11,(int)rValue12,rrValue12,
    (int)rValue13,rrValue13,(int)rValue14,rrValue14,(int)rValue15,rrValue15);
  // Wait a second and turn off the led
  delay(1000);
  digitalWrite(led, LOW);    // turn the LED off
  delay((timeToWait*1000)-1000); // Wait the rest of time before next line
}
