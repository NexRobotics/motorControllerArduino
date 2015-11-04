#define NO_PRINT 0
#define PRINT_SPEED 1

//mXeY - silnik X, pin enkodera Y
#define m1e1 23
#define m1e2 25
#define m2e1 22
#define m2e2 24
#define m3e1 13
#define m3e2 27
#define m4e1 14
#define m4e2 29

//mXkY - silnik X, pin kierunku Y
#define m1k1 23
#define m1k2 22
#define m2k1 13
#define m2k2 12
#define m3k1 27
#define m3k2 26
#define m4k1 7
#define m4k2 5

//mXPWM - PWM silnika X
#define m1PWM 2
#define m2PWM 11
#define m3PWM 4
#define m4PWM 6
// to be added
// #define PRINT_POWER 2
// #define PRINT_ERROR 3

typedef struct silnik
{
  //liczby to numery pinów
    int pwm;
    int kierunek1;
    int kierunek2;
    int enkoder1;
    int enkoder2;
    int pwmSetting;
    boolean dir_correction;
};

// Structure schema: (pwm, inA, inB, encoder inA, encoder inB, isRight?)
silnik motor1{12, 22, 31, m1e1, m1e2, 0, 0};
silnik motor2{3, 24, 33, m1e1, m1e2, 0, 1};
silnik motor3{4, 26, 27, m1e1, m1e2, 0, 0};
silnik motor4{5, 28, 29, m1e1, m1e2, 0, 1};
silnik motor5{7, 6, 8, m1e1, m1e2, 0, 0};
silnik motor6{10, 9, 11, m1e1, m1e2, 0, 11};

void initMotor(struct silnik motor)
{
  pinMode(motor.enkoder1, INPUT);
  pinMode(motor.enkoder2, INPUT);

  pinMode(motor.kierunek1, OUTPUT);
  pinMode(motor.kierunek2, OUTPUT);

  digitalWrite(motor.kierunek1, LOW);
  digitalWrite(motor.kierunek2, LOW);

  pinMode(motor.pwm, OUTPUT);

  motor.pwmSetting = 0;
}

void SetSpeed(struct silnik motor, int wypelnienie)
{
  if(wypelnienie==0)
  {
    digitalWrite(motor.kierunek1, HIGH);
    digitalWrite(motor.kierunek2, HIGH);
    analogWrite(motor.pwm, 0);
  }
  else if (!motor.dir_correction)
  {
  if(wypelnienie>0)
  {
    analogWrite(motor.pwm, wypelnienie);
    digitalWrite(motor.kierunek1, HIGH);
    digitalWrite(motor.kierunek2, LOW);
  }

  else if(wypelnienie<0)
  {
    analogWrite(motor.pwm, -wypelnienie);
    digitalWrite(motor.kierunek1, LOW);
    digitalWrite(motor.kierunek2, HIGH);
  }
    
  }else
  {
  if(wypelnienie>0)
  { 
    analogWrite(motor.pwm, wypelnienie);
    digitalWrite(motor.kierunek2, HIGH);
    digitalWrite(motor.kierunek1, LOW);
  }

  else if(wypelnienie<0)
  {
    analogWrite(motor.pwm, -wypelnienie);
    digitalWrite(motor.kierunek2, LOW);
    digitalWrite(motor.kierunek1, HIGH);
  }
    
  }
}

unsigned long czas, timeBuffer;
String inputString = "";
volatile boolean newMessageReceived = false;
//Pulses Per Revolution, Revolutions Per Revolution
int PPR, RPR;
volatile int u[6] = {0, 0, 0, 0, 0, 0};
int v1=0, e1=0, v2=0, e2=0, v3=0, e3=0, v4=0, e4=0;
int PowerOnMotor1 = 0, PowerOnMotor2 = 0, PowerOnMotor3 = 0, PowerOnMotor4 = 0;
int PWMSpeed[6] = {0, 0, 0, 0, 0, 0};
boolean slopeRise1 = 1, slopeFall1 = 0, slopeRise2 = 1, slopeFall2 = 0, slopeRise3 = 1, slopeFall3 = 0, slopeRise4 = 1, slopeFall4 = 0;
byte timeCounter = 0;
volatile unsigned int timeOfLastMessageReceived = 0;
unsigned long startingTime = 0;
int slopeCounter1 = 0, slopeCounter2 = 0, slopeCounter3 = 0, slopeCounter4 = 0;
byte Kp = 0;
byte ConstantPrint = NO_PRINT;
boolean SmartDrive = 1;
int mios = 1000; //Maksymalna Ilość Obrotów Silnika

void setup()
{
  Serial.begin(115200);
  Serial.println("Pololu & Arduino Motor Driver by MtK");
  inputString.reserve(200);
  /*md1i2.init();
  md3i4.init();
  md1i2.setM1Speed(0);
  md1i2.setM2Speed(0);
  md3i4.setM1Speed(0);
  md3i4.setM2Speed(0);*/
  initMotor(motor1);
  initMotor(motor2);
  initMotor(motor3);
  initMotor(motor4);
  initMotor(motor5);
  initMotor(motor6);
  
  PPR = 6;
  RPR = 10;
  
  Kp = 2;

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  delay(1000);
  digitalWrite(13, HIGH);
  
}

void loop()
{
  if (newMessageReceived)
    handleNewMessage();

  if(millis() - timeOfLastMessageReceived > 100)
  {
    for(int i=0; i<6; i++)
      u[i] = 0;
  }
  if (SmartDrive)
  {
//    slopeCounter1 = 0;
//    slopeCounter2 = 0;
//    slopeCounter3 = 0;
//    slopeCounter4 = 0;
//    startingTime = micros();
//    for(unsigned i = 0; i<5000; i++)
//    {
//      if (digitalRead(m1e1) && slopeRise1)
//      {
//          if(digitalRead(m1e2))
//              slopeCounter1--;
//          else
//              slopeCounter1++;
//          slopeRise1 = 0;
//          slopeFall1 = 1;
//      }
//      
//      if (!digitalRead(m1e1) && slopeFall1)
//      {
//          if(digitalRead(m1e2))
//              slopeCounter1++;
//          else
//              slopeCounter1--;
//          slopeRise1 = 1;
//          slopeFall1 = 0;
//      }
//  //----------------------------------------------------------
//      if (digitalRead(m2e1) && slopeRise2)
//      {
//          if(digitalRead(m2e2))
//              slopeCounter2--;
//          else
//              slopeCounter2++;
//          slopeRise2 = 0;
//          slopeFall2 = 1;
//      }
//      
//      if (!digitalRead(m2e1) && slopeFall2)
//      {
//          if(digitalRead(m2e2))
//              slopeCounter2++;
//          else
//              slopeCounter2--;
//          slopeRise2 = 1;
//          slopeFall2 = 0;
//      }
//  //----------------------------------------------------------
//      if (digitalRead(m3e1) && slopeRise3)
//      {
//          if(digitalRead(m3e2))
//              slopeCounter3--;
//          else
//              slopeCounter3++;
//          slopeRise3 = 0;
//          slopeFall3 = 1;
//      }
//      
//      if (!digitalRead(m3e1) && slopeFall3)
//      {
//          if(digitalRead(m3e2))
//              slopeCounter3++;
//          else
//              slopeCounter3--;
//          slopeRise3 = 1;
//          slopeFall3 = 0;
//      }
//  //----------------------------------------------------------
//      if (digitalRead(m4e1) && slopeRise4)
//      {
//          if(digitalRead(m4e2))
//              slopeCounter4--;
//          else
//              slopeCounter4++;
//          slopeRise4 = 0;
//          slopeFall4 = 1;
//      }
//      
//      if (!digitalRead(m4e1) && slopeFall4)
//      {
//          if(digitalRead(m4e2))
//              slopeCounter4++;
//          else
//              slopeCounter4--;
//          slopeRise4 = 1;
//          slopeFall4 = 0;
//      }
//    }
//    
//    czas = micros() - startingTime;
//  //  Serial.print("#Enkoders, ");
//  //  Serial.print(slopeCounter1);
//  //  Serial.print(", ");
//  //  Serial.print(slopeCounter2);
//  //  Serial.print(", ");
//  //  Serial.print(slopeCounter3);
//  //  Serial.print(", ");
//  //  Serial.print(slopeCounter4);
//  //  Serial.print(", ");
//  //  Serial.println(); // oddzielenie pustą linią dla zwiększenia czytelności
//    v1 = slopeCounter1*((60000000)/(RPR*PPR*(long)czas));
//    v2 = slopeCounter2*((60000000)/(RPR*PPR*(long)czas));
//    v3 = slopeCounter3*((60000000)/(RPR*PPR*(long)czas));
//    v4 = slopeCounter4*((60000000)/(RPR*PPR*(long)czas));
//
//    e1 = u[0]-v1;
//    e2 = u[1]-v2;
//    e3 = u[2]-v3;
//    e4 = u[3]-v4;
//    
//    PowerOnMotor1 += Kp*e1;
//    PowerOnMotor2 += Kp*e2;
//    PowerOnMotor3 += Kp*e3;
//    PowerOnMotor4 += Kp*e4;
//    
//    if(PowerOnMotor1 > 40000)
//      PowerOnMotor1 = 40000;
//    else if(PowerOnMotor1< -40000)
//      PowerOnMotor1 = -40000;
//
//      if(PowerOnMotor2 > 40000)
//      PowerOnMotor2 = 40000;
//    else if(PowerOnMotor2 < -40000)
//      PowerOnMotor2 = -40000;
//
//      if(PowerOnMotor3 > 40000)
//      PowerOnMotor3 = 40000;
//    else if(PowerOnMotor3 < -40000)
//      PowerOnMotor3 = -40000;
//
//      if(PowerOnMotor4 > 40000)
//      PowerOnMotor4 = 40000;
//    else if(PowerOnMotor4 < -40000)
//      PowerOnMotor4 = -40000;

//    if (u1 == 0)
//      SetSpeed(motor1, 0);
//    else
//      SetSpeed(motor1, PowerOnMotor1/100);
//      
//    if (u2 == 0)
//      SetSpeed(motor2, 0);
//    else
//      SetSpeed(motor2, PowerOnMotor2/100);
//      
//    if (u3 == 0)
//      SetSpeed(motor3, 0);
//    else
//      SetSpeed(motor3, PowerOnMotor3/100);
//      
//    if (u4 == 0)
//      SetSpeed(motor4, 0);
//    else
//      SetSpeed(motor4, PowerOnMotor4/100);

      SetSpeed(motor1, u[0]);
      SetSpeed(motor2, u[1]);
      SetSpeed(motor3, u[2]);
      SetSpeed(motor4, u[3]);
      SetSpeed(motor5, u[4]);
      SetSpeed(motor6, u[5]);

//      delay(2000);
//      for(int i=0; i<6; i++)
//      {
//      Serial.print(u[i]);
//      Serial.print("   ");
//      }
//      Serial.println("zeroing...");
//      u[0]=0;
//      u[1]=0;
//      u[2]=0;
//      u[3]=0;
//      u[4]=0;
//      u[5]=0;
      

  }
  else
  {
    PowerOnMotor1 = 0;
    PowerOnMotor2 = 0;
    PowerOnMotor3 = 0;
    PowerOnMotor4 = 0;
    
    SetSpeed(motor1, PWMSpeed[0]);
    SetSpeed(motor2, PWMSpeed[1]);
    SetSpeed(motor3, PWMSpeed[2]);
    SetSpeed(motor4, PWMSpeed[3]);
    SetSpeed(motor5, PWMSpeed[4]);
    SetSpeed(motor6, PWMSpeed[5]);
    
  }
 
  if(ConstantPrint == PRINT_SPEED)
  {
    Serial.println(v1);
    Serial.println(v2);
    Serial.println(v3);
    Serial.println(v4);
  }
}


void serialEvent() {
  digitalWrite(13, LOW);
  while (Serial.available()) 
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
      // if the incoming character is a newline, set a flag
      // so the main loop can do something about it:
      if (inChar == '\n')
        newMessageReceived = true;
  }
}

void handleNewMessage()
{
  int spaceBar = inputString.indexOf(' ');
  if (spaceBar != -1)
  {
    String buffer = "";
    buffer.reserve(10);
    String buffer2 = "";
    String decomposedMessage[7];
    for(int i = 0; i<7; i++)
    {
      decomposedMessage[i].reserve(10);
      decomposedMessage[i] = "";
    }
      
    buffer2.reserve(10);
    String bufferMotor = "";
    bufferMotor.reserve(10);
    for (int i=0; i<spaceBar; i++)
      buffer += inputString.charAt(i);

    for(int indexPtr = 0, segmentPtr = 0; indexPtr<(inputString.length()-1) && segmentPtr<7; indexPtr++)
    {
      decomposedMessage[segmentPtr] +=  inputString.charAt(indexPtr);
      if(inputString.charAt(indexPtr+1) == ' ')
      {
        segmentPtr++; 
        indexPtr++;
      }
    }

//    spaceBar = inputString.indexOf(' ');
//    for (int i=spaceBar+1; i<(inputString.length()-1); i++)
//      buffer2 += inputString.charAt(i);

    if(decomposedMessage[0] == "Set")
    {
      timeOfLastMessageReceived = millis();
      for (int i=0; i<6; i++)
      {
        int buffer = decomposedMessage[i+1].toInt();
        if(buffer > 0)
          u[i] = min(buffer, mios);
        else
          u[i] = max(buffer, -mios);
      }

      for (int i=0; i<6; i++)
      {
        Serial.print(u[i]);
        Serial.print(", ");
      }
      Serial.println("");
    }

    else if (buffer == "SetKp")
    {
      Kp = buffer2.toInt();
    }
//    else if (buffer == "SetPWM")
//    {
//      if(bufferMotor=="motor1")
//      PWMSpeed1 = 4*buffer2.toInt();
//      else if(bufferMotor=="motor2")
//      PWMSpeed2 = 4*buffer2.toInt();
//      else if(bufferMotor=="motor3")
//      PWMSpeed3 = 4*buffer2.toInt();
//      else if(bufferMotor=="motor4")
//      PWMSpeed4 = 4*buffer2.toInt();
//      SmartDrive = 0;
//    }
    else if (buffer == "Print")
    {
      if(buffer2 == "Speed")
      {
         Serial.println(v1);
         Serial.println(v2);
         Serial.println(v3);
         Serial.println(v4);
         Serial.println();
      }
          
      
      else if(buffer2 == "Error")
      {
        Serial.println(e1);
        Serial.println(e2);
        Serial.println(e3);
        Serial.println(e4);
        Serial.println();
      }
          
      
      else if(buffer2 == "Power")
      {
        Serial.println(PowerOnMotor1/400);
        Serial.println(PowerOnMotor2/400);
        Serial.println(PowerOnMotor3/400);
        Serial.println(PowerOnMotor4/400);
        Serial.println();
      }
          
      else if(buffer2 == "ConstSpeed")
          ConstantPrint = PRINT_SPEED;
      else if(buffer2 == "ConstDisable")
          ConstantPrint = NO_PRINT;
    }
    else if(buffer == "Kill")
    {
      SetSpeed(motor1, 0);
      SetSpeed(motor2, 0);
      SetSpeed(motor3, 0);
      SetSpeed(motor4, 0);
      SmartDrive = 0;
    }
  }
  else
    Serial.println("Message ERROR\n\rInvalid command!\n\r");
      
  newMessageReceived = false;
  inputString = "";
}

