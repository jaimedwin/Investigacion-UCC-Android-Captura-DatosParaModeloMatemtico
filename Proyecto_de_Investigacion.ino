// Proyecto de investigacion final V1.0

//Se incluye la librería <SD.h> 
#include <SD.h>
  
// Se define una variable tipo file. 
File Archivo; 
 
//Pines sensor Ultrasonido  
const int EchoPin = 6; 
const int TriggerPin = 5;  
 
//Variables para el proyecto de investigacion  
int Vcarros; 
boolean entro=0;
boolean mensaje=0; 
float sumaTiempo=0.0;
float cm_t = 0.0;  
float tiempoentrocarro =0.0;
float tiempoacumulado = 0.0;
  
//Var string para concatenar datos txt 
String registrotxt; 
 
void setup() {
  Serial.begin(9600); //Habiltia puerto serial
  //pinMode(LedPin, OUTPUT); // ping para el bluetooth 
 
 
  //Para el utlrasonido 
  pinMode(TriggerPin, OUTPUT); 
  pinMode(EchoPin, INPUT); 
 
 
  pinMode(4, OUTPUT); //Se establece como salida el pin correspondiente a SS. 
  //Se muestra por el monitor si la comunicación se ha establecido correctamente 
  //o ha habido algún tipo de error. 
  if (!SD.begin(4)){ 
    Serial.println("Se ha producido un fallo al iniciar la comunicación con la SD"); 
    return; 
  } 
  Serial.println("Se ha iniciado la comunicación correctamente con la SD"); 
  Serial.println("----------<|Iniciciando Momitoreo|>-------------"); 
} 

// Funcion ping
int ping(int TriggerPin, int EchoPin) { 
  long duration, distanceCm; 
   
  digitalWrite(TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us 
  delayMicroseconds(4); 
  digitalWrite(TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us 
  delayMicroseconds(10); 
  digitalWrite(TriggerPin, LOW); 
   
  duration = pulseIn(EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos 
    
  distanceCm = duration * 10 / 292/ 2;   //convertimos a distancia, en cm 
  if(distanceCm>=5 && distanceCm<=100){ 
    return distanceCm; 
  }
  else{ 
    distanceCm=0; 
    return distanceCm; 
  } 
} 

void loop() { 
  
  int cm = ping(TriggerPin, EchoPin); 
  //Serial.print("Distancia: "); 
  //Serial.print(cm); 
  sumaTiempo = millis();
  sumaTiempo = sumaTiempo/1000; 
  
  if (cm >= 5 && cm <= 100 ){
    cm_t = cm_t+0.01; 
  }else{
    cm_t = 0.00;
  }
    
  //Serial.print("    Tiempo: ");
  //Serial.print(cm_t); 
  
  if (cm>0 && cm_t>0){ 
    if(entro==0){ 
      entro=1; 
      Vcarros=Vcarros+1;
      tiempoentrocarro = sumaTiempo;
     } 
  }
  if(cm==0){ 
    entro=0; 
  }     
  
  //Serial.print("  carro: ");   
  //Serial.print(Vcarros); 
  //Serial.print(" Suma Tiempo: "); 
  //Serial.println(sumaTiempo); 
    
  registrotxt = "";
   
  if(entro==1 && mensaje ==0){
    mensaje=1;       
    Serial.println("--------Entro carro------------");
    tiempoentrocarro =  sumaTiempo;
  }
  if(entro==0 && mensaje==1){
    mensaje=0; 
    //Se abre el documento sobre el que se va a leer y escribir. 
    Archivo = SD.open("datos.txt", FILE_WRITE); 
        
    //Se comprueba que el archivo se ha abierto correctamente y se procede a 
    //escribir en él. 


    if (Archivo){
      Serial.println("--------Salio carro------------");
      if (Vcarros == 1 ){
        registrotxt += String(Vcarros); 
        registrotxt += ";"; 
        registrotxt += String(tiempoentrocarro); 
        registrotxt += ";"; 
        registrotxt += String(tiempoentrocarro); 
        registrotxt += ";"; 
        registrotxt += String(sumaTiempo);
        registrotxt += ";"; 
        registrotxt += String(sumaTiempo - tiempoentrocarro);
        //Se escribe información en el documento de texto datos.txt. 
        tiempoacumulado = tiempoentrocarro;
      }else{
        tiempoacumulado = tiempoentrocarro - tiempoacumulado;
        registrotxt += String(Vcarros); 
        registrotxt += ";"; 
        registrotxt += String(tiempoentrocarro); 
        registrotxt += ";"; 
        registrotxt += String(tiempoacumulado); 
        registrotxt += ";"; 
        registrotxt += String(sumaTiempo);
        registrotxt += ";"; 
        registrotxt += String(sumaTiempo - tiempoentrocarro);
        //Se escribe información en el documento de texto datos.txt. 
      }
      Serial.println(registrotxt);
      Archivo.println(registrotxt); 
      //Se cierra el archivo para almacenar los datos. 
      Archivo.close(); 
    }else{ 
      Serial.println("Error al abrir archivo"); 
      /* FIN DE LA ESCRITURA DE DATOS EN LA MEMORIA SD DE ARDUINO */ 
    }
  } 
  delay(10); 
} 


