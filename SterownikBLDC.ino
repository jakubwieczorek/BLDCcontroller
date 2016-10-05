
// Top transistors PWM
#define WT 11
#define VT 10
#define UT 9
// Bottom transistors 
#define WB 8
#define VB 7
#define UB 6
// prohibet state is signalised by LED
#define prohibitedStateLED 5

int rotorLocation = 1;

// fuse in order to indicate whether some keys are in prohibited state
void fuse();

void switchOffTransistors();

void switchOffW();
void switchOffV();
void switchOffU();

void switchOnW();
void switchOnV();
void switchOnU();

void disconnectW();
void disconnectV();
void disconnectU();

void commutation();

/************************************************************************/

void switchOnW()
{
   digitalWrite(WT, HIGH);
   digitalWrite(WB, LOW);
}

void switchOnV()
{
   digitalWrite(VT, HIGH);
   digitalWrite(VB, LOW);
}

void switchOnU()
{
   digitalWrite(UT, HIGH);
   digitalWrite(UB, LOW);
}

void switchOffW()
{
   digitalWrite(WT, LOW);
   digitalWrite(WB, HIGH);
}

void switchOffV()
{
   digitalWrite(VT, LOW);
   digitalWrite(VB, HIGH);
}

void switchOffU()
{
   digitalWrite(UT, LOW);
   digitalWrite(UB, HIGH);
}

void disconnectW()
{
  digitalWrite(WT, LOW);
  digitalWrite(WB, LOW);
}

void disconnectV()
{
  digitalWrite(VT, LOW);
  digitalWrite(VB, LOW);
}

void disconnectU()
{
  digitalWrite(UT, LOW);
  digitalWrite(UB, LOW);
}

void switchOffTransistors()
{
  disconnectU();
  disconnectW();
  disconnectV();
}

void fuse()
{
  //if both keys (bottom and top) are switched on in any phase 
  if((digitalRead(WT) == HIGH && digitalRead(WB) == HIGH) || 
     (digitalRead(VT) == HIGH && digitalRead(VB) == HIGH) || 
     (digitalRead(UT) == HIGH && digitalRead(UB) == HIGH))
  {
     switchOffTransistors();
     
     // signal that state is prohibited
     while(true)
     {
       digitalWrite(prohibitedStateLED, !digitalRead(prohibitedStateLED));
       delay(300);
     }
  }
}

void commutation()
{
  switch(rotorLocation++)
  {
     case 1:
     {
       // U +
       switchOnU();
       // V -
       switchOffV();
       // W NC
       disconnectW();
       
       break;
     }
     case 2:
     {
       // U +
       switchOnU();
       // V NC
       disconnectV();
       // W -
       switchOffW();
       
       break;
     } 
     case 3:
     {
       // U NC
       disconnectU();
       // V +
       switchOnV();
       // W -
       switchOffW();
      
       break;
     } 
     case 4:
     {
       // U -
       switchOffU();
       // V +
       switchOnV();
       // W NC
       disconnectW();
       
       break;
     } 
     case 5:
     {
       // U -
       switchOffU();
       // V NC
       disconnectV();
       // W + 
       switchOnW();
       
       break;
     } 
     case 6:
     {
       // U NC
       disconnectU();
       // V -
       switchOffV();
       // W +
       switchOnW();
       
       rotorLocation = 1;
       
       break;
     } 
  }
  
  fuse();
}

/***********************************************************************/

void setup()
{
  pinMode(prohibitedStateLED, OUTPUT);
  pinMode(WB, OUTPUT);
  pinMode(WT, OUTPUT);
  pinMode(VB, OUTPUT);
  pinMode(VT, OUTPUT);
  pinMode(UB, OUTPUT);
  pinMode(UT, OUTPUT);
  
  switchOffTransistors();
}

void loop()
{
  commutation();
  
  delay(5); // setting engine speed 
}



