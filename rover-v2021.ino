#include <Servo.h>
Servo serDD;
Servo serDI; 
Servo serTD;
Servo serTI;
int DD = 90;
int DI = 90;
int TD = 90;
int TI = 90; 

Servo serB;
Servo serC;
Servo serE;
Servo serM;
int B;
int C;
int E;
int M;

#include <EnableInterrupt.h>
long loop_timer, tiempo_ejecucion;

// ==================== SECCIÓN AGREGADA PARA CÓDIGO DE MINI DISPLAY ==================== //
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
boolean lup=true;
// ==================== FIN DE SECCIÓN DE MINI DISPLAY ==================== // 

volatile long contPotenciaInit; // LEER MANDO RC POTENCIA  == ENDENDIDO / APAGADO
volatile int PulsoPotencia;
void INTpotencia() {
  if (digitalRead(A10) == HIGH) contPotenciaInit = micros();
  if (digitalRead(A10) == LOW)PulsoPotencia = micros() - contPotenciaInit;
}

volatile long contPitchInit; // LEER MANDO RC PITCH  ==  ACTIVACIÓN ADELANTE / ATRAS
volatile int PulsoPitch;
void INTpitch() {
  if (digitalRead(A9) == HIGH) contPitchInit = micros();
  if (digitalRead(A9) == LOW) PulsoPitch = micros() - contPitchInit;
}

volatile long contRollInit; // LEER MANDO RC ROLL 
volatile int PulsoRoll;
void INTroll() {
  if (digitalRead(A8) == HIGH) contRollInit = micros();
  if (digitalRead(A8) == LOW) PulsoRoll = micros() - contRollInit;
}

volatile long contYawInit; // LEER MANDO RC YAW  ==  CONTROL SERVOS DERECHA / IZQUIERDA
volatile int PulsoYaw;
void INTyaw() {
  if (digitalRead(A11) == HIGH) contYawInit = micros();
  if (digitalRead(A11) == LOW) PulsoYaw = micros() - contYawInit;
}

volatile long contSAInit;
volatile int PulsoSA;
void INTSA() {
  if (digitalRead(A12) == HIGH) contSAInit = micros();
  if (digitalRead(A12) == LOW) PulsoSA = micros() - contSAInit;
}

volatile long contSBInit;
volatile int PulsoSB;
void INTSB() {
  if (digitalRead(A13) == HIGH) contSBInit = micros();
  if (digitalRead(A13) == LOW) PulsoSB = micros() - contSBInit;
}

 
int DDA = 16;
int DDB = 17;
int DCA = 30;
int DCB = 31;
int DTA = 24;
int DTB = 25;

int IDA = 18;
int IDB = 19;
int ICA = 22;
int ICB = 23;
int ITA = 26;
int ITB = 27;

// ==================== FUNCIÓN PARA IMPRESIÓN DE TEXTO EN MINI DISPLAY ==================== //
void ptxt(String text, int x, int y,int size, boolean d) {
  d=false;
  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(text);
  if(d=false){
    display.display();
  }
}
// ===================================== FIN DE FUNCIÓN =====================================//

void setup() {
  pinMode(DDA, OUTPUT);
  pinMode(DDB, OUTPUT);
  pinMode(DCA, OUTPUT);
  pinMode(DCB, OUTPUT);
  pinMode(DTA, OUTPUT);
  pinMode(DTB, OUTPUT);
    
  pinMode(IDA, OUTPUT);
  pinMode(IDB, OUTPUT);
  pinMode(ICA, OUTPUT);
  pinMode(ICB, OUTPUT);
  pinMode(ITA, OUTPUT);
  pinMode(ITB, OUTPUT);
  // ************************************ SERVOS ************************************ //
  serDD.attach(2);
  serDI.attach(3);
  serTD.attach(4);
  serTI.attach(5);

  serB.attach(8);
  serC.attach(9);
  serE.attach(10);
  serM.attach(11);

  // ********************************* FIN DE SERVOS ******************************** //
  pinMode(A11, INPUT_PULLUP);                  // YAW
  enableInterrupt(A11, INTyaw, CHANGE);
  pinMode(A10, INPUT_PULLUP);                  // POTENCIA
  enableInterrupt(A10, INTpotencia, CHANGE);
  pinMode(A9, INPUT_PULLUP);                 // PITCH
  enableInterrupt(A9, INTpitch, CHANGE);
  pinMode(A8, INPUT_PULLUP);                  // ROLL
  enableInterrupt(A8, INTroll, CHANGE);
  pinMode(A12, INPUT_PULLUP);                  // ROLL
  enableInterrupt(A12, INTSA, CHANGE);
  pinMode(A13, INPUT_PULLUP);                  // ROLL
  enableInterrupt(A13, INTSB, CHANGE);

  Serial.begin(115200);
  delay(100);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  delay(2000);
  display.clearDisplay(); // Clear the buffer.

  ptxt("ARDUINO", 22, 0, 2, false);
  ptxt("ROVER", 34, 18, 2, false);
  display.display();
  delay(1000); 
  display.clearDisplay();
  ptxt("FIEE", 30, 6, 3, false);
  display.display();
  delay(1000); 
  display.clearDisplay();
  ptxt("UNMSM", 22, 5, 3, false);
  display.display();
  delay(1000); 
  display.clearDisplay();
  ptxt("<===INTEGRANTES===>", 6, 10, 1, false);
  display.display();
  delay(1000); 
  display.clearDisplay();
  ptxt("16190160", 0, 3, 1, false);
  ptxt("18190139", 0, 12, 1, false);
  ptxt("18190161", 0, 21, 1, false);
  ptxt("18190183", 55, 12, 1, false);
  ptxt("18190170", 55, 21, 1, false);
  display.display();
  delay(1000);
  display.clearDisplay();
}
int suma=0;
void loop() {
  while (micros() - loop_timer < 10000);
  int var = 0;
  digitalWrite(DDA, LOW);
  digitalWrite(DDB, LOW);
  digitalWrite(DCA, LOW);
  digitalWrite(DCB, LOW);
  digitalWrite(DTA, LOW);
  digitalWrite(DTB, LOW);

  digitalWrite(IDA, LOW);
  digitalWrite(IDB, LOW);
  digitalWrite(ICA, LOW);
  digitalWrite(ICB, LOW);
  digitalWrite(ITA, LOW);
  digitalWrite(ITB, LOW);
  
  tiempo_ejecucion = (micros() - loop_timer) / 1000;
  loop_timer = micros();
  
  if(PulsoPitch < 1001){
    PulsoPitch = 1000;
  }
  if(PulsoYaw < 1001){
    PulsoYaw = 1000;
  }
  if(PulsoPotencia < 1001){
    PulsoPotencia = 1000;
  }
  if(PulsoRoll < 1001){
    PulsoRoll = 1000;
  }
  while(lup!=false){
    display.clearDisplay();
    ptxt("ENCENDER", 22, 0, 2, false);
    ptxt("MANDO", 40, 18, 2, false);
    display.display();
    delay(500);
    display.clearDisplay();
    ptxt("________", 22, 0, 2, false);
    ptxt("-----", 40, 18, 2, false);
    display.display();
    delay(500);
    suma = PulsoPitch + PulsoRoll + PulsoYaw + PulsoPotencia;
    Serial.print(suma);
    Serial.println(" RAYOS");
    if(suma > 5500){
      Serial.println("SALIMOS");
      delay(1000);
      lup = false;
    }else{
      lup = true;
    }
  }
  if(PulsoSA < 1250){
    // SECCIÓN PARA FUNCIONES DE MOVIMIENTO DE SERVOS //
    if(PulsoYaw > 1650){
      DD = map(PulsoYaw, 1600, 2050,100,180);
      serDD.write(DD);
      DI = map(PulsoYaw, 1600, 2050,100,180);
      serDI.write(DI);
      TD = map(PulsoYaw, 1600, 2050,100,180);
      serTD.write(TD);
      TI = map(PulsoYaw, 1600, 2050,100,180);
      serTI.write(TI);
    }else if (PulsoYaw < 1350){
      DD = map(PulsoYaw, 950, 1400,0,80);
      serDD.write(DD);
      DI = map(PulsoYaw, 950, 1400,0,80);
      serDI.write(DI);
      TD = map(PulsoYaw, 950, 1400,0,80);
      serTD.write(TD);
      TI = map(PulsoYaw, 950, 1400,0,80);
      serTI.write(TI);
    }else{
      DD = 90;
      DI = 90;
      TD = 90;
      TI = 90;
      serDD.write(DD);
      serDI.write(DI);
      serTD.write(TD);
      serTI.write(TI);
    }
    // ============================================== //
    if(PulsoPitch < 1250 && PulsoPotencia > 1250){
      digitalWrite(DDA, HIGH);
      digitalWrite(DDB, LOW);
      digitalWrite(DCA, HIGH);
      digitalWrite(DCB, LOW);
      digitalWrite(DTA, HIGH);
      digitalWrite(DTB, LOW);
      var = 1;
      digitalWrite(IDA, HIGH);
      digitalWrite(IDB, LOW);
      digitalWrite(ICA, HIGH);
      digitalWrite(ICB, LOW);
      digitalWrite(ITA, HIGH);
      digitalWrite(ITB, LOW);
  
    }
    else if(PulsoPitch > 1750 && PulsoPotencia > 1250){
      digitalWrite(DDB, HIGH);
      digitalWrite(DDA, LOW);
      digitalWrite(DCB, HIGH);
      digitalWrite(DCA, LOW);
      digitalWrite(DTB, HIGH);
      digitalWrite(DTA, LOW);
      var = 2;
      digitalWrite(IDB, HIGH);
      digitalWrite(IDA, LOW);
      digitalWrite(ICB, HIGH);
      digitalWrite(ICA, LOW);
      digitalWrite(ITB, HIGH);
      digitalWrite(ITA, LOW);
    }
  }else if(PulsoSA > 1750){
    // SECCIÓN PARA FUNCIONES DE MOVIMIENTO DE SERVOS BRAZO //
     B = map(PulsoPotencia, 950, 2050,15,165);
     serB.write(B);
     C = map(PulsoPitch, 950, 2050,15,165);
     serC.write(C);
     E = map(PulsoPitch, 950, 2050,15,165);
     serE.write(E);
     M = map(PulsoSB, 1000, 2000,15,165);
     serM.write(M);
  }else{
    Serial.print("NOTHING");
  }

  Serial.print("Pot:");
  Serial.print(PulsoPotencia);
  Serial.print("\t Pitch:");
  Serial.print(PulsoPitch);
  Serial.print("\t Roll:");
  Serial.print(PulsoRoll);
  Serial.print("\t Yaw:");
  Serial.print(PulsoYaw);
  Serial.print("\t DD:");
  Serial.print(DD);
  Serial.print("\t DI:");
  Serial.print(DI);
  Serial.print("\t TD:");
  Serial.print(TD);
  Serial.print("\t TI:");
  Serial.print(TI);
  Serial.print("\t Dir:");
  Serial.print(var);
  Serial.print("\t SA:");
  Serial.print(PulsoSA);
  Serial.print("\t SB:");
  Serial.println(PulsoSB);
}
