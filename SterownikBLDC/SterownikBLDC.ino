// W - red
// V - blue
// U - green

// in second engine W - blue, V - green

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

// to increase engine speed
int shift = 50;

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

int getDelay();

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
  case 6:
    {
      // U +
      switchOnU();
      // V NC
      disconnectV();
      // W -
      switchOffW();
      
      rotorLocation = 1;
      
      break;
    } 
  case 5:
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
  case 3:
    {
      // U -
      switchOffU();
      // V NC
      disconnectV();
      // W + 
      switchOnW();

      break;
    } 
  case 2:
    {
      // U NC
      disconnectU();
      // V -
      switchOffV();
      // W +
      switchOnW();

      break;
    } 
  }

  fuse();
}

/***********************************************************************/


void myDelay(int *state, int *prevDelay, int engineSpeed)
{  
  if(*prevDelay < engineSpeed)
  {
    delay(*prevDelay);
    return;
  }
  
  if((*state)++ == 12)
  {
    *state = 0;

    *prevDelay = *prevDelay - 1;

    delay(*prevDelay);

    return;
  }

  delay(*prevDelay);
}

void myMicroDelay(int *state, int *prevDelay)
{
  if(*prevDelay < 3000)
  {
    delayMicroseconds(*prevDelay);
    return;
  }

  if((*state)++ == 6)
  {
    *state = 0;
    
    *prevDelay = *prevDelay - shift;
 
    delayMicroseconds(*prevDelay);

    return;
  }

  delayMicroseconds(*prevDelay);
}

void setup()
{
  pinMode(prohibitedStateLED, OUTPUT);
  pinMode(WB, OUTPUT);
  pinMode(WT, OUTPUT);
  pinMode(VB, OUTPUT);
  pinMode(VT, OUTPUT);
  pinMode(UB, OUTPUT);
  pinMode(UT, OUTPUT);
  pinMode(13, OUTPUT);

  switchOffTransistors();
}

void loop()
{
  int state = 0;
  int prevDelay = 50;
  int engineSpeed = 30;

  while(true)
  {
    commutation();
    
    //delay(40);
    
    myDelay(&state, &prevDelay, engineSpeed);
  
    if(prevDelay < 15)
      break;
  }

  /*prevDelay = 15000;

  digitalWrite(13, HIGH);

  while(true)
  {
    commutation();
    myMicroDelay(&state, &prevDelay);
  }*/
}




