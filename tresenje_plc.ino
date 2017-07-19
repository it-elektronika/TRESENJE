
#include <SPI.h>
#include <Ethernet.h>
#include "Mudbus.h"
#include <SPI.h>


/// INPUTS PINS ///
int enable_pin = 49;
int stop_total_pin = 22;
int varnostna_vrata_zaprta_pin = 23;
int varnostna_vrata_zaklenjena_pin = 24;
int akd_no_fault_pin = 25;
int driver_kolesa_ready_pin = 26;
int start_cikla_pin = A0;
int stop_cikla_pin = A1;
int kontroliran_stop_tipka_pin = A2;
int home_position_tipka_pin = A3;
int zavora_odpuscena_pin = A4;
int odpiranje_gripperja_tipka_pin = A5;
int zapiranje_gripperja_tipka_pin = 27;
int preklop_ciklov_pin = 28;
int tlacni_senzor_vhod_stroja_pin = 29;
int gripper_stevni_senzor_pin = 30;
int gripper_doza_postavljena_pin = 31;
int gripper_odstevalni_senzor_pin = A6;
int loputa_zaprta_pin = A7;
int tlacni_senzor_gripper_zaprt_pin = A8;
int home_position_reached_pin = A9;
int max_position_reached_pin = A10;
int konec_tresenja_pin = A11;
int praznenje_pin = 2;
int kolo_vhod_senzor_pin = 33;
int kolo_izhod_senzor_pin = 34;
int kolo_obrat_senzor_pin = 35;
//int reset_modbus_pin = 36;
int debelina_doze1_pin = A12;
int debelina_doze2_pin = A13;
////////////////////////////
/// OUTPUTS PINS  ///
///////////////////////////
int odpiranje_gripperja_pin = 36;
int zapiranje_gripperja_pin = 37;
int zapora_drce_pin = 38;
int start_tresenja_pin = 39;
int kontroliran_stop_pin = 40;
int fault_reset_pin = 41;
int home_position_pin = 42;
int odpuscanje_zavore_motorja_pin = 43;
int obracanje_dozirnega_kolesa_pin = 44;
int zapora_vrat_pin = 45;
int stroj_pripravljen_pin = 46;
int stroj_running_pin = 47;
int stroj_napaka_pin = 48;
int kolo_frekvencnik_pin = 49;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INPUTS ///
int stop_total = 0;
int varnostna_vrata_zaprta = 0;
int varnostna_vrata_zaklenjena = 0;
int akd_no_fault = 0;
int driver_kolesa_ready= 0;
int start_cikla= 0;
int stop_cikla = 0;
int kontroliran_stop_tipka= 0;
int home_position_tipka= 0;
int zavora_odpuscena = 0;
int odpiranje_gripperja_tipka = 0;
int zapiranje_gripperja_tipka = 0;
int preklop_ciklov = 0;
int tlacni_senzor_vhod_stroja = 0;
int gripper_stevni_senzor = 0;
int gripper_doza_postavljena = 0;
int gripper_odstevalni_senzor = 0;
int loputa_zaprta = 0;
int tlacni_senzor_gripper_zaprt = 0;
int home_position_reached = 0;
int max_position_reached = 0;
int konec_tresenja = 0;
int praznenje = 0;
int kolo_vhod_senzor = 0;
int kolo_izhod_senzor = 0;
int kolo_obrat_senzor = 0;
int reset_modbus = 0;
int debelina_doze1 = 0;
int debelina_doze2 = 0;
////////////////////////////
/// OUTPUTS PINS  ///
///////////////////////////
int odpiranje_gripperja = 0;
int zapiranje_gripperja = 0;
int zapora_drce = 0;
int start_tresenja = 0;
int kontroliran_stop = 0;
int fault_reset = 0;
int home_position = 0;
int odpuscanje_zavore_motorja = 0;
int obracanje_dozirnega_kolesa = 0;
int zapora_vrat = 0;
int stroj_pripravljen = 0;
int stroj_running = 0;
int stroj_napaka = 0;
int kolo_frekvencnik = 0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// COUNTERS ///
int kolo_stevec = 0;
int gripper_stevec = 0;
int gripper_odstevec = 0;
int gripper_odstevec_old = 0;
int last_kolo_stevni_senzor = 0;
int last_gripper_stevni_senzor = 0;
int last_gripper_odstevalni_senzor = 0;
int last_konec_tresenja = 0;
//////////////////////////////////////////////
int state = -1;
long startTime;
long elapsedTime;
int timerState;
long storeTime;
int firstRun = 1;
int doze_number = 0;
int cakanje_timer = 18000000;
/////////////////////////////////////////
int last_start_cikla = 0;
int last_stop_cikla = 0;
int cikel = 0;
/////////////////////////////////////////
bool doza_velika;
bool doza_srednja;
bool doza_mala;
////////////////////////////////////////
int command_enable = 0;
int last_obrat_senzor = 0;
int tid = 0;
/////////////////////////7
int last_kolo_vhod_senzor;
///////////////////////////////////////////
Mudbus Mb;
byte drive[] = { 192, 168, 1, 30 };
byte obuf [17];
EthernetClient client;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SETUP ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
  //delay(30000);  
  uint8_t mac[]     = { 0x90, 0xA2, 0xDA, 0x00, 0x51, 0x06 };
  uint8_t ip[]      = { 192, 168, 1, 90 };
  uint8_t gateway[] = { 192, 168, 1, 1 };
  uint8_t subnet[]  = { 255, 255, 255, 0 };
  Serial.begin(9600);
  Ethernet.begin(mac, ip, gateway, subnet);
  delay(5000);  //Time to open the terminal
  ////////////////////////////////////////////////////////////////////////////////////////
  Mb.R[0] = 0;
  Mb.R[1] = 0;
  Mb.R[2] = 0;
  Mb.R[3] = 0;
  Mb.R[4] = 0;
  Mb.R[5] = 0;
  Mb.R[6] = 0;
  Mb.R[7] = 0;
  Mb.R[8] = 0;
  Mb.R[9] = 0;
  Mb.R[10] = 0;
  Mb.R[11] = 0;
  Mb.R[12] = 0;
  Mb.R[13] = 0;
  Mb.R[14] = 0;
  Mb.R[15] = 0;
  Mb.R[16] = 0;
  Mb.R[17] = 0;
  Mb.R[18] = 0;
  Mb.R[19] = 0;
  ////////////////////////////////////////////////////////////////////////////////////////
  Serial.println("WELCOME");// zacni serial komunikacijo. zaradi izpisovanja vrednosti 
  pinMode(stop_total_pin, INPUT);
  pinMode(varnostna_vrata_zaprta_pin, INPUT);
  pinMode(varnostna_vrata_zaklenjena_pin, INPUT);
  pinMode(akd_no_fault_pin, INPUT);
  pinMode(driver_kolesa_ready_pin, INPUT);
  pinMode(start_cikla_pin, INPUT);
  pinMode(stop_cikla_pin, INPUT);
  pinMode(kontroliran_stop_tipka_pin, INPUT);
  pinMode(home_position_tipka_pin, INPUT);
  pinMode(odpiranje_gripperja_tipka_pin, INPUT);
  pinMode(zapiranje_gripperja_tipka_pin, INPUT);
  pinMode(zavora_odpuscena_pin, INPUT);
  pinMode(preklop_ciklov_pin, INPUT);
  pinMode(tlacni_senzor_vhod_stroja_pin, INPUT);
  pinMode(gripper_stevni_senzor_pin, INPUT);
  pinMode(gripper_doza_postavljena_pin, INPUT);
  pinMode(gripper_odstevalni_senzor_pin, INPUT);
  pinMode(loputa_zaprta_pin, INPUT);
  pinMode(tlacni_senzor_gripper_zaprt_pin, INPUT);
  pinMode(home_position_reached_pin, INPUT);
  pinMode(max_position_reached_pin, INPUT);
  pinMode(konec_tresenja_pin, INPUT);
  pinMode(praznenje_pin, INPUT);
  pinMode(kolo_vhod_senzor_pin, INPUT);
  pinMode(kolo_izhod_senzor_pin, INPUT);
  pinMode(kolo_obrat_senzor_pin, INPUT);
  pinMode(debelina_doze1, INPUT);
  pinMode(debelina_doze2, INPUT);
  pinMode(odpiranje_gripperja_pin, OUTPUT);
  pinMode(zapiranje_gripperja_pin, OUTPUT);
  pinMode(zapora_drce_pin, OUTPUT);
  pinMode(start_tresenja_pin, OUTPUT);
  pinMode(kontroliran_stop_pin, OUTPUT);
  pinMode(fault_reset_pin, OUTPUT);
  pinMode(home_position_pin, OUTPUT);
  pinMode(odpuscanje_zavore_motorja_pin, OUTPUT);
  pinMode(obracanje_dozirnega_kolesa_pin, OUTPUT);
  pinMode(zapora_vrat_pin, OUTPUT);
  pinMode(stroj_pripravljen_pin, OUTPUT);
  pinMode(stroj_running_pin, OUTPUT);
  pinMode(stroj_napaka_pin, OUTPUT);
  pinMode(kolo_frekvencnik_pin, OUTPUT);

}

char rx_byte = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////// LOOP ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Mb.Run();
  Mb.R[0] = digitalRead(varnostna_vrata_zaprta_pin);
  Mb.R[1] = digitalRead(varnostna_vrata_zaklenjena_pin);
  Mb.R[2] = digitalRead(akd_no_fault_pin);
  Mb.R[3] = digitalRead(driver_kolesa_ready_pin);
  Mb.R[4] = digitalRead(preklop_ciklov_pin);
  Mb.R[5] = digitalRead(tlacni_senzor_vhod_stroja_pin);
  Mb.R[6] = digitalRead(loputa_zaprta_pin);
  Mb.R[7] = digitalRead(tlacni_senzor_gripper_zaprt_pin);
  Mb.R[8] = digitalRead(home_position_reached_pin);
  Mb.R[9] = digitalRead(max_position_reached_pin);
  Mb.R[10] = digitalRead(konec_tresenja_pin);
  Mb.R[11] = kolo_stevec;
  Mb.R[12] = gripper_stevec;
  Mb.R[13] = gripper_odstevec;
  Mb.R[14] = doza_velika;
  Mb.R[15] = doza_srednja;
  Mb.R[16] = doza_mala;
  Mb.R[17] = cikel;
  Mb.R[18] = elapsedTime;
  /////////////////////////////////////////////////////////////////////////
  startTime = millis();
  /////////////////////////////////////////////////////////////////////////
  stop_total = digitalRead(stop_total_pin);
  varnostna_vrata_zaprta = digitalRead(varnostna_vrata_zaprta_pin);
  varnostna_vrata_zaklenjena = digitalRead(varnostna_vrata_zaklenjena_pin);
  akd_no_fault = digitalRead(akd_no_fault_pin);
  driver_kolesa_ready = digitalRead(driver_kolesa_ready_pin);
  start_cikla = digitalRead(start_cikla_pin);
  stop_cikla = digitalRead(stop_cikla_pin);
  kontroliran_stop = digitalRead(kontroliran_stop_pin);
  home_position_tipka = digitalRead(home_position_tipka_pin);
  odpiranje_gripperja_tipka = digitalRead(odpiranje_gripperja_tipka_pin);
  zapiranje_gripperja_tipka = digitalRead(zapiranje_gripperja_tipka_pin);
  ////////////////////////////////////////////////////////////////////////
  zavora_odpuscena = digitalRead(zavora_odpuscena_pin);
  preklop_ciklov = digitalRead(preklop_ciklov_pin);
  tlacni_senzor_vhod_stroja = digitalRead(tlacni_senzor_vhod_stroja_pin);
  gripper_stevni_senzor = digitalRead(gripper_stevni_senzor_pin);
  gripper_doza_postavljena = digitalRead(gripper_doza_postavljena_pin);
  gripper_odstevalni_senzor = digitalRead(gripper_odstevalni_senzor_pin);
  loputa_zaprta = digitalRead(loputa_zaprta_pin);
  tlacni_senzor_gripper_zaprt = digitalRead(tlacni_senzor_gripper_zaprt_pin);
  home_position_reached = digitalRead(home_position_reached_pin);
  max_position_reached = digitalRead(max_position_reached_pin);
  konec_tresenja = digitalRead(konec_tresenja_pin);
  praznenje = digitalRead(praznenje_pin);
  kolo_vhod_senzor = digitalRead(kolo_vhod_senzor_pin);
  kolo_izhod_senzor = digitalRead(kolo_izhod_senzor_pin);
  kolo_obrat_senzor = digitalRead(kolo_obrat_senzor_pin);
  //reset_modbus = digitalRead(reset_modbus_pin);
  debelina_doze1 = digitalRead(debelina_doze1_pin);
  debelina_doze2 = digitalRead(debelina_doze2_pin);
  //////////////////////////////////////////////////////////////////////
  odpiranje_gripperja = digitalRead(odpiranje_gripperja_pin);
  zapiranje_gripperja = digitalRead(zapiranje_gripperja_pin);
  zapora_drce = digitalRead(zapora_drce_pin);
  start_tresenja = digitalRead(start_tresenja_pin);
  kontroliran_stop = digitalRead(kontroliran_stop_pin);
  fault_reset = digitalRead(fault_reset_pin);
  home_position = digitalRead(home_position_pin);
  odpuscanje_zavore_motorja = digitalRead(odpuscanje_zavore_motorja_pin);
  obracanje_dozirnega_kolesa = digitalRead(obracanje_dozirnega_kolesa_pin);
  zapora_vrat = digitalRead(zapora_vrat_pin);
  stroj_pripravljen = digitalRead(stroj_pripravljen_pin);
  stroj_running = digitalRead(stroj_running_pin);
  stroj_napaka = digitalRead(stroj_napaka_pin);
  kolo_frekvencnik = digitalRead(kolo_frekvencnik_pin);
  Serial.println(digitalRead(2));
  //Serial.println(kolo_stevec);
  ///////////////////////////////////////////////////////////////////////////
  //doze_check();
  //Serial.println("command enable:");
  //Serial.println(command_enable);
  //Serial.println("kolo_obrat_senzor");
  //Serial.println(kolo_obrat_senzor);
  //Serial.println("##################");
  //Serial.println("preklop:");
  //Serial.println(preklop_ciklov);
  //Serial.println("kolo stevec:");
  //Serial.println(kolo_stevec);
  //Serial.println("gripper stevec:");
  //Serial.println(gripper_stevec);
  //Serial.println("gripper odstevec:");
  //Serial.println(gripper_odstevec);
  
  //check_start_cikla();
  //check_stop_cikla();
  
  //if(cikel == 1 and preklop_ciklov == 0)// and vrata_zaklenjena == HIGH and vrata_zaprta == HIGH)
  //{
  //  error_check();
  //  home_position_manual();
  //  preparation_shaking();
  //  kolo_stetje();
  //  gripper_stetje();
  //  gripper_odstevanje();
  //  pogoji_start();
  //  obrat_kolesa();
  //  prepis_vrednosti();
  //  stop_motion();
  //  resetTimer();
  //  kolo_timer();
  //}

  //else if(cikel == 0)
  //{
    //digitalWrite(enable_pin, LOW);
   // state = -1;
   // kolo_stevec = 0;
   // gripper_stevec = 0;
   //  gripper_odstevec = 0; 
   // 
   // gripper_manual();
   //start_tresenja_pin = LOW;
  //  digitalWrite(obracanje_dozirnega_kolesa_pin, LOW);
  //  digitalWrite(zapora_drce_pin, LOW);
  //}  

  //else if(cikel == 1 and preklop_ciklov == 1) //and vrata_zaklenjena == HIGH and vrata_zaprta == HIGH)
  //{

  //  obrat_kolesa_simple();
  //  preparation_no_shaking();
  //}
  obrat_kolesa();
  gripper_manual();

  //////////////////////////////////////////////////////////
  Serial.println("state:");
  Serial.println(state);  
  //////////////////////////////////////////////////////////
  if (Serial.available() > 0) 
  {    // is a character available?
    rx_byte = Serial.read();       // get the character
  
    // check if a number was received
    if ((rx_byte == '1')) 
    {
      Serial.print("ODPIRANJE GRIPPERJA");
      digitalWrite(odpiranje_gripperja_pin, HIGH);
      Serial.println(digitalRead(odpiranje_gripperja_pin));
      delay(1000);
      digitalWrite(odpiranje_gripperja_pin, LOW);
      Serial.println(digitalRead(odpiranje_gripperja_pin));  
    }

    else if ((rx_byte == '0'))
    {
      Serial.print("ZAPIRANJE GRIPPERJA");
      digitalWrite(zapiranje_gripperja_pin, HIGH);
      Serial.println(digitalRead(zapiranje_gripperja_pin));
      delay(1000); /// add timing
      digitalWrite(zapiranje_gripperja_pin, LOW);
      Serial.println(digitalRead(zapiranje_gripperja_pin));
    }
    
    else if ((rx_byte == '3'))
    {
      Serial.println("CLOSING LATCH");
      digitalWrite(zapora_drce_pin, HIGH);
      Serial.println(digitalRead(zapora_drce_pin));
      //delay(1000);
      //digitalWrite(zapora_drce_pin, LOW);
      Serial.println(digitalRead(zapora_drce_pin));
    }

    else if ((rx_byte == '4'))
    {
      Serial.println("START MOTION");
      digitalWrite(start_tresenja_pin, HIGH);
      Serial.println(digitalRead(start_tresenja_pin));
      delay(1000);
      digitalWrite(start_tresenja_pin, LOW);
    }

   
    else if ((rx_byte == '6'))
    {
      digitalWrite(enable_pin, HIGH);  
    }
    
    else if ((rx_byte == '7'))
    {
      digitalWrite(enable_pin, LOW);  
    }

    
    else if ((rx_byte == '8'))
    {
      digitalWrite(home_position_pin, HIGH);
      delay(1000);
      digitalWrite(home_position_pin, LOW);  
    }

    else if ((rx_byte == '9'))
    {
      digitalWrite(fault_reset_pin, HIGH);
      delay(1000);
      digitalWrite(fault_reset_pin, LOW);  
    }

  } // end: if (Serial.available() > 0)
  

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
}


////////////////////////////////////////////////////////////////////////
///////////////// FUNKCIJE /////////////////////////////////////////////

void check_start_cikla()
{
  if(start_cikla != last_start_cikla)
  {
    if(start_cikla == HIGH)
    {
      cikel = 1;
    }
  }
  last_start_cikla = start_cikla;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void check_stop_cikla()
{
  if(stop_cikla != last_stop_cikla)
  {
    if(stop_cikla == HIGH)
    {
      cikel = 0;
    }
  }
  last_stop_cikla = stop_cikla;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void kolo_stetje()   // stejem doze, ki grejo cez kolo.
{
  if (kolo_izhod_senzor != last_kolo_stevni_senzor)
  {
    if (kolo_izhod_senzor == HIGH and (kolo_stevec < doze_number))
    {
      kolo_stevec++;

    }
  }
  last_kolo_stevni_senzor = kolo_izhod_senzor;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gripper_stetje()   // stejem doze, ki pridejo mimo vhodnega senzorja. senzor na gripperju.
{
  if (gripper_stevni_senzor != last_gripper_stevni_senzor)
  {
    if (gripper_stevni_senzor == HIGH and (gripper_stevec < doze_number))
    {
      gripper_stevec++;
    }
  }
  last_gripper_stevni_senzor = gripper_stevni_senzor;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gripper_odstevanje() // stejem doze, katere grejo iz gripperja ven. po koncanem tresenju
{
  if (gripper_odstevalni_senzor != last_gripper_odstevalni_senzor)
  {
    if (gripper_odstevalni_senzor == HIGH and (0 < gripper_odstevec))
    {
      gripper_odstevec--;
    }
  }
  last_gripper_odstevalni_senzor = gripper_odstevalni_senzor;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void pogoji_start()   // gledam, ce so izpolnjeni pogoji za start tresenja. 1. prislo je 18 doz v gripper. 2. pretekel je cas. vsaj ena doza je v gripperju.
{
  if(state == 0)
  {
    if (kolo_stevec == doze_number and gripper_stevec == doze_number and gripper_odstevec == 0 )//or elapsedTime > cakanje_timer and gripper_odstevec == 0 and firstRun == 0 and kolo_stevec == gripper_stevec) 
    {
      delay(5000);
      Serial.println("IZPOLNJENI");
      digitalWrite(zapora_drce_pin, HIGH);
      digitalWrite(zapiranje_gripperja_pin, HIGH);
      delay(500);
      digitalWrite(zapiranje_gripperja_pin, LOW);
      
      delay(500);
      digitalWrite(start_tresenja_pin, HIGH);
      delay(1000);
      digitalWrite(start_tresenja_pin, LOW);
      state = 1;
  
    }    
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////
void obrat_kolesa() //obracanje kolesa. ko ne trese
{
  if (state == 0) 
  {
    if(kolo_vhod_senzor != last_kolo_vhod_senzor)
    {
      if (kolo_vhod_senzor == HIGH and kolo_stevec < doze_number and kolo_izhod_senzor == LOW)
      {
        //firstRun = 0;
        digitalWrite(obracanje_dozirnega_kolesa_pin, HIGH);
        //timerState = 1;
      }
  
      if (kolo_obrat_senzor == HIGH)
      {
        digitalWrite(obracanje_dozirnega_kolesa_pin, LOW);
        //timerState = 0;
      }
    }
    last_kolo_vhod_senzor = kolo_vhod_senzor;  
    if (kolo_obrat_senzor == HIGH)
    {
      digitalWrite(obracanje_dozirnega_kolesa_pin, LOW);
      //timerState = 0;
    }  
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////
void obrat_kolesa_simple() //obracanje kolesa. ko ne trese
{
  if (state == 0) 
  {
    if(kolo_vhod_senzor != last_kolo_vhod_senzor)
    {
      if (kolo_vhod_senzor == HIGH and kolo_izhod_senzor == LOW)
      {
        
        digitalWrite(obracanje_dozirnega_kolesa_pin, HIGH);
        
      }
  
      if (kolo_obrat_senzor == HIGH)
      {
        digitalWrite(obracanje_dozirnega_kolesa_pin, LOW);
        
      }
    }
    last_kolo_vhod_senzor = kolo_vhod_senzor;  
    if (kolo_obrat_senzor == HIGH)
    {
      digitalWrite(obracanje_dozirnega_kolesa_pin, LOW);
      
    }  
  }
}
////////////////////////////////////////////////////////////////////////////////////////
void prepis_vrednosti() // prepis vrednosti v vmesnem stanju med netresenjem in tresenjem
{
  if (state == 1) {
    gripper_odstevec_old = gripper_stevec;
    gripper_odstevec = gripper_odstevec_old;
    kolo_stevec = 0;
    gripper_stevec = 0;
    elapsedTime = 0;

    state = 2;
  }
}
////////////////////////////////////////////////////////////////////////////////////////
void stop_motion()    // izkljucim signal za tresenje, ko konca tresti
{
  if(state==2)
  {
    if (konec_tresenja != last_konec_tresenja)
    {
      if (konec_tresenja == HIGH )
      {
        delay(2000);
        //digitalWrite(start_tresenja, LOW);
        digitalWrite(zapora_drce_pin, LOW);
        digitalWrite(odpiranje_gripperja_pin, HIGH);
        delay(500);
        digitalWrite(odpiranje_gripperja_pin, LOW);
        delay(500);
        //digitalWrite(zapiranje_gripperja_pin, HIGH);
        //delay(500);
        //digitalWrite(zapiranje_gripperja_pin, LOW);
        state = 93;
      }
    }
    last_konec_tresenja = konec_tresenja;
  }
}
///////////////////////////////////////////////////////////////////////////////////
void kolo_timer() // casovnik za merjenje casa od zadnje doze, ki je sla skozi kolo
{
  if (timerState == 1)
  {
    storeTime = startTime;
  }
  elapsedTime = startTime - storeTime;
}


/////////////////////////////////////////////////////////////////////////////////
void resetTimer() // ponastavljanje casovnika
{
  if (state == 3)
  {
    elapsedTime = 0;
    firstRun = 1;
    state = 0;
  }
}
//////////////////////////////////////////////////////////////////////////////////
void modbus_reset()
{
  if(digitalRead(reset_modbus) == 1)
  {
    uint8_t mac[]     = { 0x90, 0xA2, 0xDA, 0x00, 0x51, 0x06 };
    uint8_t ip[]      = { 192, 168, 1, 90 };
    uint8_t gateway[] = { 192, 168, 1, 1 };
    uint8_t subnet[]  = { 255, 255, 255, 0 };
    Serial.begin(9600);
    Ethernet.begin(mac, ip, gateway, subnet);
    
    delay(5000);  //Time to open the terminal  
  }
}
//////////////////////////////////////////////////////////////////////////////////
void doze_check()
{
  if(debelina_doze1 == 1 and debelina_doze2 == 0)
  {
    doze_number = 20;
    doza_velika = false;
    doza_srednja = true;
    doza_mala = false;
  }
  else if(debelina_doze1 == 0 and debelina_doze2 == 1)
  {
    doze_number = 18;
    doza_velika = true;
    doza_srednja = false;
    doza_mala = false;
  }
  else if(debelina_doze1 == 1 and debelina_doze2 == 1)
  {  
    doze_number = 22;
    doza_velika = false;
    doza_srednja = false;
    doza_mala = true;
  }
}
////////////////////////////////////////////////////////////////////////////////
void go_home()
{
  home_position_pin = HIGH;
  delay(1000);
  home_position_pin = LOW;
}
/////////////////////////////////////////////////////////////////////////////////
void en_obrat_kolesa()
{
  digitalWrite(obracanje_dozirnega_kolesa, HIGH);
  if (kolo_obrat_senzor == HIGH)
  {
    digitalWrite(obracanje_dozirnega_kolesa, LOW);
  }
}
/////////////////////////////////////////////////////////////////////////////
void gripper_manual()
{
  if(odpiranje_gripperja_tipka == HIGH)
    {
      digitalWrite(odpiranje_gripperja_pin,HIGH);
      delay(500);
      digitalWrite(odpiranje_gripperja_pin,LOW);
    }
  if(zapiranje_gripperja_tipka == HIGH)
    {
      digitalWrite(zapiranje_gripperja_pin,HIGH);
      delay(500);
      digitalWrite(zapiranje_gripperja_pin,LOW);
    } 
}
////////////////////////////////////////////////////////////////////////////////////
void error_check()
{
  if(tlacni_senzor_gripper_zaprt == 0)
  {
    digitalWrite(kontroliran_stop_pin, HIGH);
  }
}
/////////////////////////////////////////////////////////////////////////////////
void home_position_manual()
{
  if(home_position_tipka == HIGH)
  {
    digitalWrite(home_position_pin, HIGH);
    delay(1000);
    digitalWrite(home_position_pin, LOW);
  }
}
///////////////////////////////////////////////////////////////////////////////////
void preparation_shaking()
{
  if (state == -1)
  {
    digitalWrite(enable_pin, HIGH);
    digitalWrite(zapora_drce_pin, HIGH);
    digitalWrite(odpiranje_gripperja_pin, HIGH);
    delay(500);
    digitalWrite(odpiranje_gripperja_pin, LOW);
    state = 0;
  }  
}
//////////////////////////////////////////////////////////////////////////////////
void preparation_no_shaking()
{
  digitalWrite(odpiranje_gripperja_pin, HIGH);
  delay(500);
  digitalWrite(odpiranje_gripperja_pin, LOW);
  digitalWrite(zapora_drce_pin, LOW);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
////state = - 1 (preparation)
////state = 0 (waiting for start-shaking conditions to be fullfiled)
////state = 1 (shaking and value copying) 
////state = 2 (end of shaking)
////state = 3 (timer reset)
///////////////////////////////////////////////////////////////////////////////////

