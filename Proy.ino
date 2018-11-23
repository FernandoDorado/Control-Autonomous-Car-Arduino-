#include <SoftwareSerial.h>

SoftwareSerial BTserial(8, 9); // RX | TX
const long baudRate = 38400; 
char c=' ';
boolean NL = true;
 






// Pins Ultrasonido
const int TRIG_PIN = 7;
const int ECHO_PIN = 8;
const int TRIG_PIN2 = 5;
const int ECHO_PIN2 = 6;

//Pins motores
/*Motor A*/
const int in1 = 10;  // Pin que controla el sentido de giro
const int in2 = 9;  // Pin que controla el sentido de giro

/*Motor B*/
const int enB = 3;  // Pin habilita motor B - PWM
const int in3 = 11;  // Pin que controla el sentido de giro
const int in4 = 12;  // Pin que controla el sentido de giro








//Cualquier cosa a más de 400 (23200 pulsos) está fuera del rango
const unsigned int MAX_DIST = 500;


void setup() {

  Serial.begin(9600);
      Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
  BTserial.begin(baudRate);
      Serial.print("BTserial started at "); Serial.println(baudRate);
    Serial.println(" ");  
  
  // Configura todos los pines del motor como salida
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}


void VelocidadFija(int parar)
{
  // Gira el Motor A en ambas direcciones a una velocidad fija
  // Gira el motor A en sentido horario
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  if (parar){
      // Detenemos el Motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  }

  

}

void VelocidadVariable()
{
  // Giramos el Motor B en un rango de velocidad
  // Usamos PWM para enviar analogWrite() y controlar la maxima velocidad posible
  
  // Giramos los motores en un sentido
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
 
  // Aceleramos desde 0 hasta la velocidad maxima
  for(int i = 0; i < 255; i++)
  {
    analogWrite(enB, i); // Usamos PWM para variar la velocidad
    Serial.print("Velocidad: ");
    Serial.println(i);
    delay(20);
  }
 
  // Desaceleramos desde la velocidad maxima hasta llegar a 0 
  for(int i = 255; i > 0; i--)
  {
    analogWrite(enB, i); // Usamos PWM para variar la velocidad
    Serial.print("Velocidad: ");
    Serial.println(i);
    delay(20);
  }
 
  // Detenemos los motores
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
 




int ultraSon(int TRIG_PINf,int ECHO_PINf){
    unsigned long pulse_width;
    unsigned long t1;
    unsigned long t2;
    float cm;

  
    pinMode(TRIG_PINf,OUTPUT);
    pinMode(ECHO_PINf,INPUT);

     digitalWrite(TRIG_PINf,HIGH);
     delayMicroseconds(10);
     digitalWrite(TRIG_PINf, LOW);
  
    // Esperamos a recibir la señal
    while ( digitalRead(ECHO_PINf) == 0);
    //Mide cuando tiempo esta el echo pin a nivel alto
    // Note: the micros() counter will overflow after ~70 min
    t1 = micros();
    while ( digitalRead(ECHO_PINf) == 1);
    t2 = micros();
    pulse_width = t2 - t1;
    cm = pulse_width / 58.0;

    return cm;
}



void imprimeSon(float cm, float cm_2)
{
    if ( cm > MAX_DIST ) {
    //Serial.println("Fuera de rango(1) \t");
  } else {
    Serial.print(cm);
    Serial.print("\t");

  }

  if ( cm_2 > MAX_DIST ) {
    //Serial.println("Fuera de rango(2) \n");
  } else {
   Serial.print(cm_2);
    Serial.print("\n");
  }

  return 0;
}





void loop() {
  float cm;
  float cm_2;
  int parar;
  
  cm=ultraSon(TRIG_PIN,ECHO_PIN);
  delay(10);
  cm_2=ultraSon(TRIG_PIN2,ECHO_PIN2);
  delay (10);
  if(cm<30 || cm_2<30) parar=1;
  else parar=0;
  
  imprimeSon(cm,cm_2);
  VelocidadFija(parar);

  /*Leemos del Bluetooth y enviamos a Serial Monitor*/
    if (BTserial.available())
    {
        c = BTserial.read();
        
        
        Serial.write(c);
    }


  /*Leemos del Serial Monitor y enviamos a Bluetooth*/
   if (Serial.available())
    {
        c = Serial.read();
        BTserial.write(c);   
 
        // Echo the user input to the main window. The ">" character indicates the user entered text.
        if (NL) { Serial.print(">");  NL = false; }
        Serial.write(c);
        if (c==10) { NL = true; }
    }
  
  






  
  
  // Wait at least 60ms before next measurement
  delay(60);
}
