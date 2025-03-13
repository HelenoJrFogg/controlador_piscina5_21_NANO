

////ARDUINO NANO///

#include <EEPROM.h>


//////////////////////Sensores
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS_PISC         2         // Pino do sensor da temperatura da piscina
#define ONE_WIRE_BUS_PAINEL       3         // Pino do sensor do painel de aquecimento
#define ONE_WIRE_BUS_RETORNO      4         // Pino sensor de retorno de agua
OneWire SensorPiscina(ONE_WIRE_BUS_PISC);
OneWire oneWire_out(ONE_WIRE_BUS_PAINEL);
OneWire oneWire_out_1(ONE_WIRE_BUS_RETORNO);
DallasTemperature sensor_piscina(&SensorPiscina);
DallasTemperature sensor_painel(&oneWire_out);
DallasTemperature sensor_retorno(&oneWire_out_1);



/////////////////////////////////////Botões:
unsigned long lastDebounceTime = 0 ;
//int analogreading;
int buttonPinSet = 11;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin
int ledState = HIGH;         // the current state of the output pin
int buttonStateSet;             // the current reading from the input pin
bool lastButtonStateSet = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTimeSet = 0;  // the last time the output pin was toggled
int debounceDelay = 50;    // the debounce time; increase if the output flickers

int buttonPinUp = 12;    // the number of the pushbutton pin
int buttonStateUp;             // the current reading from the input pin
bool lastButtonStateUp = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTimeUp = 0;  // the last time the output pin was toggled

 int buttonPinDw = 10;    // the number of the pushbutton pin
int buttonStateDw;             // the current reading from the input pin
bool lastButtonStateDw = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTimeDw = 0;  // the last time the output pin was toggled
/////////////////////////////////////////////////////////////////////////////////////////////
//unsigned long difmillis =0;


   //bool chamarMenu = 0;
   unsigned long tempochamarmenu = 0;
   bool estadochamarmenu;
   unsigned long lastDebounceTimeMenu = 0;
   bool mudancaLCD;
   unsigned long tempomenu;
   bool botoes = LOW;
   int buttonPushCounter = 0;
   unsigned long whilelastTime = 0;
   bool ultimoestadobotoes ;
  int temporizador;


   
//LCD...

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,20,4);
//LiquidCrystal_I2C lcd(0x27,20,4);


//Array simbolo grau
byte grau[8] ={ B00001100,
                B00010010,
                B00010010,
                B00001100,
                B00000000,
                B00000000,
                B00000000,
                B00000000,};





//int s1;

bool memupdate = LOW ;

int SetTemperPiscina;
float SetTemperPiscFloat;
int TemperPiscEEPROM;

int SetDifTempEntrSaida;
float SetDifTemperEntrSaidaFloat;
int DifTemperEntrSaidaEEPROM;

int SetTemperSuperAq;
float SetTemperSuperAqfloat;
int SetTemperSuperAqEEPROM;

int SetTemperDegelo;
float SetTemperDegeloFloat;
int SetTemperDegeloEEPROM;

int SetTempoAcionBomba;
float SetTempoAcionBombaFloat;
int SetTempoAcionBombaEEPROM;

int SetTempoBombaDesl;
float SetTempoBombaDeslFloat;
int SetTempoBombaDeslEEPROM;





//float tempreal;
//float tempreal1;

int bomba1 = 2;// pino acionamento da bomba de aquecimenti lento.
int bomba2 = 2;// pino acionamento da bomba de aquecimento rapido.
int bomba3 = 2;// pino acionamento da bomba de filtragem.


/////////////////////////////////////////





int contador = 0;
boolean botaoset;
boolean botaodw;
boolean botaoup;


boolean chamadamenu = 0;
int tempodemenu = 5000;

// diferença milisegundos
unsigned long ti;
static unsigned long tf;
int tdif;
static unsigned long tp1;

int segundos, minutos, horas, total, total1;

unsigned long previousMillis;
const long interval = 800; 

int temperaturapiscina;
int tempersaidaaquecedor;
int temperaturaplaca;

boolean disparoaquecerpiscina;
boolean placaaquecida;
boolean difentrsaida;
boolean circularagua;
boolean aquecendo;
boolean circulacaodeprotecao;

unsigned long tbstart;
int tbstop;
unsigned long basetempo10seg;
unsigned long basetempo30seg;
int TAcionBomba;
int TIntervBomba;
boolean bombacionada;
unsigned long tbomba;
unsigned long tempobombaacioncircprot; //tempo de acionamento da bomba no acionamento de proteção.
unsigned long tempointervdeprot;       // tempo de intervalo da bomba no acionamento de proteção. 

 unsigned long tempo;


 bool DisparoProtecao;


void setup(void)


{

    sensor_piscina.begin();
    sensor_painel.begin();
    sensor_retorno.begin();
    sensor_piscina.setWaitForConversion(false);
    sensor_painel.setWaitForConversion(false);
    sensor_retorno.setWaitForConversion(false);


   pinMode(buttonPinSet, INPUT_PULLUP);
   pinMode(buttonPinUp, INPUT_PULLUP);
   pinMode(buttonPinDw, INPUT_PULLUP);

  
 

  
  pinMode(bomba1, OUTPUT);
  pinMode(bomba2, OUTPUT);
  pinMode(bomba3, OUTPUT);
  
  Serial.begin(9600);
  Serial.println(" ");
  
  
 

//EEPROM.begin(50);


  
     lcd.init();
delay(500);
     lcd.backlight();
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("CONTROLADORES ");
       lcd.setCursor(0, 1);
       lcd.print("FOGGIATTO V2.1");
       delay(2000);
       lcd.clear();
       lcd.createChar(0, grau);
    
  tp1 = -9000;
  
  TemperPiscEEPROM = EEPROM.read(0);
  DifTemperEntrSaidaEEPROM = EEPROM.read(1);
  SetTemperSuperAqEEPROM = EEPROM.read(2);
  SetTemperDegeloEEPROM = EEPROM.read(3)- 100;
  SetTempoAcionBombaEEPROM = EEPROM.read(4);
  SetTempoBombaDeslEEPROM = EEPROM.read(5);
  
  
  SetTemperPiscFloat = TemperPiscEEPROM ;
  SetDifTemperEntrSaidaFloat = DifTemperEntrSaidaEEPROM;
  SetTemperSuperAqfloat = SetTemperSuperAqEEPROM;
  SetTemperDegeloFloat = SetTemperDegeloEEPROM;
  SetTempoAcionBombaFloat = SetTempoAcionBombaEEPROM;
  SetTempoBombaDeslFloat = SetTempoBombaDeslEEPROM;
  
tbstart = millis()/ 10000;

 TAcionBomba = -1 - SetTempoAcionBombaEEPROM;
}





void loop(void){
    sensor_piscina.requestTemperatures();
    sensor_painel.requestTemperatures();
    sensor_retorno.requestTemperatures();
    
    //leituraanalogbotoes();
    controle_botaoSet();
    controle_botaoUp();
    controle_botaoDw();



    Serial.print("// piscina:");
    Serial.print(sensor_piscina.getTempCByIndex(0));
    Serial.print(" painel:");
    Serial.print(sensor_painel.getTempCByIndex(0));
    Serial.print(" retorno:");
    Serial.print(sensor_retorno.getTempCByIndex(0),3);


    Serial.print("// Set:");
    Serial.print(buttonStateSet);
    Serial.print("  Up");
    Serial.print(buttonStateUp);
    Serial.print("  Dw");
    Serial.print(buttonStateDw);
    Serial.print(" dif milis ");
   // Serial.println((millis()-difmillis));
//difmillis = millis();



/////////////Menu
if (buttonStateSet ==LOW){
  tempochamarmenu = millis();
}
Serial.println(tempochamarmenu);

if ((buttonStateSet == HIGH && estadochamarmenu == LOW) && (millis()-  tempochamarmenu > 300)) {
Serial.println(millis());
chamadamenu = HIGH;
contador = 0;
} 



 basetempo10seg = millis() / 10000;
 basetempo30seg = basetempo10seg / 3;

 lcd.setCursor(0, 2);
 lcd.print(basetempo10seg);
 lcd.print("<10/30>");
 lcd.print(basetempo30seg); 

 // unsigned long start = millis();
    //int botoes;
  //int tempomenu;
 
  SetTemperPiscina = TemperPiscEEPROM;
  SetDifTempEntrSaida = DifTemperEntrSaidaEEPROM;
  SetTemperSuperAq = SetTemperSuperAqEEPROM;
  SetTemperDegelo = SetTemperDegeloEEPROM;
  SetTempoAcionBomba = SetTempoAcionBombaEEPROM;
  SetTempoBombaDesl = SetTempoBombaDeslEEPROM;
  
  
  




  
  if (chamadamenu != mudancaLCD){
      lcd.clear();
      mudancaLCD = chamadamenu;
      }
  
      //.println();
      Serial.print("chamadamenu  ");
     Serial.println(chamadamenu);
     // Serial.println("on");



while   (chamadamenu == HIGH){

    //leituraanalogbotoes();
    controle_botaoSet();
    controle_botaoUp();
    controle_botaoDw();




if ( buttonStateSet == HIGH || buttonStateUp == HIGH || buttonStateDw == HIGH){
       tempomenu = millis();
       botoes = HIGH;
    } else botoes = LOW;

 
  Serial.print("millis()");
   Serial.print(millis());
  //Serial.println("menu_on");
  // Serial.println(botoes);
  Serial.print("    tempomenu");
   Serial.println(tempomenu);
  //Serial.println("menu_on");


 if (millis() -  tempomenu > 10000) {
    
  chamadamenu = LOW;
  
  SetTemperPiscina = SetTemperPiscFloat;
  SetDifTempEntrSaida = SetDifTemperEntrSaidaFloat;
  SetTemperSuperAq = SetTemperSuperAqfloat;
  SetTemperDegelo = SetTemperDegeloFloat;
  SetTempoAcionBomba = SetTempoAcionBombaFloat;
  SetTempoBombaDesl = SetTempoBombaDeslFloat;
  
     
     atualizaeeprom();//funcao atualiza memoria

  }else{
    Serial.println(botoes);
  Serial.println("menu_on");
  
  }



if ((botoes != ultimoestadobotoes)  || (millis() - whilelastTime > 1000 ) ) {


           if (botoes == HIGH)  {
                    lcd.clear();
                    buttonPushCounter++;// if the current state is HIGH then the button went from off to on:
                    Serial;Serial.println("on");
                    Serial.print("number of button pushes: ");
                     Serial.println(buttonPushCounter);
                     //Serial.println(joyst_map);
                     
                         if (buttonPushCounter > 1) {
                            whilelastTime = millis()  - (buttonPushCounter * 100) ;  //              
                              } else whilelastTime = millis();



    
    
    if (buttonStateSet == HIGH){
        contador = contador + 1;
        lcd.clear();
         }
        
    if (contador > 6)    {
      contador = 1;
      }
     
      
      
 switch (contador){
      
   case 1:
        if (buttonStateUp == HIGH && SetTemperPiscFloat < 80){
            SetTemperPiscFloat = SetTemperPiscFloat + 1;
            }
             
        if (buttonStateDw == HIGH && SetTemperPiscFloat > 40){
            SetTemperPiscFloat = SetTemperPiscFloat - 1;
            }
            
        lcd.setCursor(0, 0);
        lcd.print("1 Temperatura Maxima");                    
        lcd.setCursor(3, 1);
        lcd.print("da Piscina");                    
        //lcd.setCursor(0, 0);
        //lcd.print("1 Temperatura Maxima");
        lcd.setCursor(3, 3);
        lcd.print(SetTemperPiscFloat/2,1);
          lcd.setCursor(7, 3);
          lcd.write((byte)0);
          lcd.print(">");
          lcd.print(SetTemperPiscFloat);
       break; 
       
  case 2:
        if (buttonStateUp == HIGH && SetDifTemperEntrSaidaFloat <= 99){
          SetDifTemperEntrSaidaFloat = SetDifTemperEntrSaidaFloat + 1;
          }
        
        if (buttonStateDw == HIGH && SetDifTemperEntrSaidaFloat > 0){
            SetDifTemperEntrSaidaFloat = SetDifTemperEntrSaidaFloat - 1;
            }
            lcd.setCursor(0, 0);
            lcd.print("2 Ganho minimo de");
            lcd.setCursor(0, 1);
            lcd.print("temperatura do");
            lcd.setCursor(0, 2);
            lcd.print("painel");
            lcd.setCursor(3, 3);
            lcd.print(SetDifTemperEntrSaidaFloat/10);
            lcd.setCursor(8, 3);
            lcd.write((byte)0);
            lcd.print(">");
            lcd.print(SetDifTemperEntrSaidaFloat);
                      
          break;

        
   case 3:
        if (buttonStateUp == HIGH && SetTemperSuperAqfloat < 140){
            SetTemperSuperAqfloat = SetTemperSuperAqfloat + 1;
            }
        if (buttonStateDw == HIGH && SetTemperSuperAqfloat > 100){
            SetTemperSuperAqfloat = SetTemperSuperAqfloat - 1;
            }
        lcd.setCursor(0, 0);
        lcd.print("3 Temperatura de");
        lcd.setCursor(0, 1);
        lcd.print("Acionamento da bomba");
        lcd.setCursor(0, 2);
        lcd.print("Protecao por calor");
        lcd.setCursor(3, 3);
        lcd.print(SetTemperSuperAqfloat /2 );

            lcd.setCursor(7, 3);
            lcd.write((byte)0);
            lcd.print(">" );
            lcd.print(SetTemperSuperAqfloat);

       break;


    case 4:
        if (buttonStateUp == HIGH && SetTemperDegeloFloat < 160){
          SetTemperDegeloFloat = SetTemperDegeloFloat + 1;
          }
        
        if (buttonStateDw == HIGH && SetTemperDegeloFloat > 50){
            SetTemperDegeloFloat = SetTemperDegeloFloat - 1;
            }
            lcd.setCursor(0, 0);
            lcd.print("4 Temperatra de Inicio");
            lcd.setCursor(5, 1);
            lcd.print("do Degelo");
            //lcd.setCursor(0, 0);
            //lcd.print("6 Temperatra deInicDegelo");
            lcd.setCursor(5, 3);
            lcd.print((SetTemperDegeloFloat/10) -10);
            lcd.setCursor(10, 3);
            lcd.write((byte)0);
            lcd.print(">");
            lcd.print(SetTemperDegeloFloat);
        break;

  

        
    case 5:
        if (buttonStateUp == HIGH && SetTempoAcionBombaFloat <= 80){
            SetTempoAcionBombaFloat = SetTempoAcionBombaFloat + 1;
            }
        if (buttonStateDw == HIGH && SetTempoAcionBombaFloat > 0){
           SetTempoAcionBombaFloat = SetTempoAcionBombaFloat - 1;
           }
           lcd.setCursor(0, 0);
           lcd.print("5 Tempo de Acionamen");
            lcd.setCursor(0, 1);
           lcd.print("to da Bomba");
           lcd.setCursor(0, 2);
           lcd.print("(ciclo Circalacao)");
           // lcd.print(SetTemperSuperAqFloat);
           total = SetTempoAcionBombaFloat * 10;
           horas = (total / 3600);
           minutos = ((total - (horas * 3600)) / 60);
           segundos = (total % 60);

              // lcd.setCursor(0, 1);
               lcd.print(SetTempoAcionBombaFloat);

           lcd.setCursor(2, 3);
        if (horas < 10){
          lcd.print("0");
           }
           lcd.print(horas);
           lcd.print(":");
           lcd.setCursor(5, 3);
        if (minutos < 10){
           lcd.print("0");
           }
           lcd.print(minutos);
           lcd.print(":");
           lcd.setCursor(8, 3);
        if (segundos < 10){
            lcd.print("0");
            }
            lcd.print(segundos); 
lcd.print(">");
lcd.print(SetTempoAcionBombaFloat);
            
          break;
          
    
     case 6:       
        if (buttonStateUp == HIGH && SetTempoBombaDeslFloat <= 239){
            SetTempoBombaDeslFloat = SetTempoBombaDeslFloat + 1;
            }
        if (buttonStateDw == HIGH && SetTempoBombaDeslFloat > 0){
           SetTempoBombaDeslFloat = SetTempoBombaDeslFloat - 1;
           }
           lcd.setCursor(0, 0);
           lcd.print("6 Tempo minimo do");
           lcd.setCursor(0, 1);
           lcd.print("Intervalo de Bomba");
           lcd.setCursor(1, 2);
           lcd.print("Desligada");
           // lcd.print(SetTemperSuperAqFloat);
           total = SetTempoBombaDeslFloat * 30;
           horas = (total / 3600);
           minutos = ((total - (horas * 3600)) / 60);
           segundos = (total % 60);
           
           lcd.setCursor(2, 3);
        if (horas < 10){
          lcd.print("0");
           }
           lcd.print(horas);
           lcd.print(":");
           lcd.setCursor(5, 3);
        if (minutos < 10){
           lcd.print("0");
           }
           lcd.print(minutos);
           lcd.print(":");
           lcd.setCursor(8, 3);
        if (segundos < 10){
            lcd.print("0");
            }
            lcd.print(segundos);
            lcd.print(">");
            lcd.print(SetTempoBombaDeslFloat);
          break;

        

        
      default:
        contador = 0;
    }
   // return;


} else {
           buttonPushCounter = 0;
            // Serial.println("off");// if the current state is LOW then the button went from on to off:
           }



ultimoestadobotoes = botoes ; 

   
  } //fim do if




 // Serial.println("fim do while");

 delay(100);
 }  ////////////////////////fim do while
  
  


  


  temperaturapiscina = 10 * sensor_piscina.getTempCByIndex(0);
  temperaturaplaca = 10 * sensor_painel.getTempCByIndex(0);
  tempersaidaaquecedor = 10 * sensor_retorno.getTempCByIndex(0);
  
  Serial.print("  tempPisc: ");
  Serial.print(temperaturapiscina);
  Serial.print("< >");
  //Serial.print("  tempPisc: ");
  Serial.print(EEPROM.read(0));
  Serial.print(" <<< ");
  
  
//  Serial.println(leitura0);
    lcd.setCursor(0, 0);
    lcd.print("Pi:");
    lcd.print(sensor_piscina.getTempCByIndex(0), 2);
    lcd.setCursor(9, 0);
    lcd.print("Aq:");
    lcd.print(sensor_retorno.getTempCByIndex(0), 2);
    lcd.setCursor(0, 1);
    lcd.print("Pa:");
    lcd.print(sensor_painel.getTempCByIndex(0),2);

    lcd.setCursor(9, 1);
    lcd.print("Dif:");
    lcd.print(sensor_piscina.getTempCByIndex(0) - sensor_retorno.getTempCByIndex(0));
         
    lcd.setCursor(0, 3);
    lcd.print(temperaturaplaca / 5);
    // lcd.print("Bomba:");
     if (digitalRead(bomba1) == 1){
       lcd.print("    Ligada");
       } else    lcd.print(" Desligada");

// unsigned long stop = millis();

    lcd.print("CP:");
    lcd.print(circulacaodeprotecao);

    lcd.setCursor(12, 2);
    lcd.print("tp:");
    lcd.print(temperaturaplaca);


  

 if ((temperaturaplaca / 5  >  SetTemperSuperAqEEPROM ) || (temperaturaplaca < SetTemperDegeloEEPROM - 100 ) && millis() > 10000) 
     {
       circulacaodeprotecao = HIGH;
     }  
   
   
   
   if ((temperaturaplaca >= (SetTemperSuperAqEEPROM /5) + 40) || (temperaturaplaca * 10 < SetTemperDegeloEEPROM))
        {
          digitalWrite(bomba1, HIGH);
         tempo = basetempo10seg;
        }     
      else{
        if (tempo + 6 <= basetempo10seg ){
             digitalWrite(bomba1, LOW);
             }
        
      
       else { circulacaodeprotecao = LOW;

              tempointervdeprot = basetempo30seg;
              tempobombaacioncircprot = basetempo10seg;
      
       }

if (temperaturaplaca * 2 >= SetTemperSuperAqEEPROM && circulacaodeprotecao == LOW ){
      disparoaquecerpiscina = HIGH;
      TAcionBomba = basetempo10seg;
      }
       else {disparoaquecerpiscina = LOW;
        
      }




int difT = temperaturapiscina * 10 + DifTemperEntrSaidaEEPROM;
int temperAq = tempersaidaaquecedor * 10;

if (  temperAq >= difT){
    difentrsaida = HIGH;
    TAcionBomba = basetempo10seg;
    }
    else difentrsaida = LOW; 



 int ligarbomba;
 


 tbomba = basetempo10seg - TAcionBomba;

 if ( (tbomba  < SetTempoAcionBombaEEPROM) || disparoaquecerpiscina == HIGH )  {
     bombacionada = HIGH;
     //digitalWrite(bomba, HIGH);
     }
 else{bombacionada = LOW;
    // digitalWrite(bomba, LOW);
     }


      }
     

  

 
}


/////////Fim do Loop






////////////////////////Função botão
/*

void leituraanalogbotoes(){
     analogreading = analogRead(A0);
     if (analogreading > 360  && analogreading < 440){
     buttonPinSet = 1;
     Serial.print("Set");
     } else buttonPinSet =0;
     
     if (analogreading > 660  && analogreading < 740){
     buttonPinUp = 1;
     Serial.print("Up");
     } else buttonPinUp =0;
     
     if (analogreading > 960 ){
     buttonPinDw = 1;
     Serial.print("Dw");
     } else buttonPinDw =0;
     
  }


*/

void controle_botaoSet(){
    int reading = !digitalRead(buttonPinSet);
  if (reading != lastButtonStateSet) {
       lastDebounceTimeSet = millis();
       }
  if ((millis() - lastDebounceTimeSet) > debounceDelay) {
   
      if (reading != buttonStateSet) {
               buttonStateSet = reading;
                // only toggle the LED if the new button state is HIGH
               //if (buttonState == HIGH) {
              //  ledState = !ledState;
              //}
      }
  }
  lastButtonStateSet = reading;
  }

  
  void controle_botaoUp(){
    int reading = !digitalRead(buttonPinUp);
  if (reading != lastButtonStateUp) {
       lastDebounceTimeUp = millis();
       }
  if ((millis() - lastDebounceTimeUp) > debounceDelay) {
   
      if (reading != buttonStateUp) {
               buttonStateUp = reading;
                // only toggle the LED if the new button state is HIGH
               //if (buttonState == HIGH) {
              //  ledState = !ledState;
              //}
      }
  }
  lastButtonStateUp = reading;
  }

  void controle_botaoDw(){
    int reading = !digitalRead(buttonPinDw);
  if (reading != lastButtonStateDw) {
       lastDebounceTimeDw = millis();
       }
  if ((millis() - lastDebounceTimeDw) > debounceDelay) {
   
      if (reading != buttonStateDw) {
               buttonStateDw = reading;
                // only toggle the LED if the new button state is HIGH
               //if (buttonState == HIGH) {
              //  ledState = !ledState;
              //}
      }
  }
  lastButtonStateDw = reading;
  }




  void  atualizaeeprom(){
//    Serial.print();
Serial.print("SetTemperPiscina: ");
Serial.print(SetTemperPiscina);
Serial.print(" TemperPiscEEPROM:");
Serial.println(TemperPiscEEPROM);
    
   if (SetTemperPiscFloat != TemperPiscEEPROM){
      EEPROM.write(0, (SetTemperPiscFloat ));
      memupdate = HIGH;  
      }
   if (SetDifTempEntrSaida != DifTemperEntrSaidaEEPROM){
      EEPROM.write(1, SetDifTempEntrSaida);
      memupdate = HIGH;
      } 
  if (SetTemperSuperAqEEPROM != SetTemperSuperAq){
      EEPROM.write(2, SetTemperSuperAq);
      memupdate = HIGH;
      }
  if (SetTemperDegelo != SetTemperDegeloEEPROM){
      EEPROM.write(3, SetTemperDegelo + 100);
      memupdate = HIGH;  
      }    
  if (SetTempoAcionBomba != SetTempoAcionBombaEEPROM){
      EEPROM.write(4, SetTempoAcionBomba);
      memupdate = HIGH;
      }
  if (SetTempoBombaDesl != SetTempoBombaDeslEEPROM){
      EEPROM.write(5, SetTempoBombaDesl);
      memupdate = HIGH;
      }

    if (memupdate == HIGH){
   //   EEPROM.commit();
      memupdate = LOW;
      
      Serial.println("atualiza memoria");
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("Salvando na memoria");
            //lcd.setCursor(0, 1);
           //lcd.print("    memoria");
        delay(5000);
            
    TemperPiscEEPROM = EEPROM.read(0);
    DifTemperEntrSaidaEEPROM = EEPROM.read(1);
    SetTemperSuperAqEEPROM = EEPROM.read(2);
    SetTemperDegeloEEPROM = EEPROM.read(3)- 100;
    SetTempoAcionBombaEEPROM = EEPROM.read(4);
    SetTempoBombaDeslEEPROM = EEPROM.read(5);
    
     }
    }

    //////////Função TEMPORIZADOR/////
    void timer (){






    }