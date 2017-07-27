
#include <SPI.h>
#include <Ethernet.h>
#include "Mudbus.h"
#include <SPI.h>
#include <EEPROM.h>


/// INPUTS PINS ///

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
int konec_tresenja_pin = A14;
int praznenje_pin = 2;
int kolo_vhod_senzor_pin = 33;
int kolo_izhod_senzor_pin = 34;
int kolo_obrat_senzor_pin = 35;
int kolo_vhod_pred_doze_senzor_pin = A15;
int debelina_doze1_pin = A12;
int debelina_doze2_pin = A13;
////////////////////////////
/// OUTPUTS PINS  ///
///////////////////////////
int enable_pin = 49;
int odpiranje_gripperja_pin = 36;
int zapiranje_gripperja_pin = 37;
int zapora_drce_pin = 4;
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
int start_tresenja_tipka;
int stop_total;
int varnostna_vrata_zaprta;
int varnostna_vrata_zaklenjena;
int akd_no_fault;
int driver_kolesa_ready;
int start_cikla;
int stop_cikla;
int kontroliran_stop_tipka;
int home_position_tipka;
int odpiranje_gripperja_tipka;
int zapiranje_gripperja_tipka;
int zavora_odpuscena;
int preklop_ciklov;
int tlacni_senzor_vhod_stroja;
int gripper_stevni_senzor;
int gripper_doza_postavljena;
int gripper_odstevalni_senzor;
int loputa_zaprta;
int tlacni_senzor_gripper_zaprt;
int home_position_reached;
int max_position_reached;
int konec_tresenja;
int praznenje;
int kolo_vhod_senzor;
int kolo_izhod_senzor;
int kolo_obrat_senzor;
int kolo_vhod_pred_doze_senzor;
int debelina_doze1;
int debelina_doze2;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// COUNTERS ///
int kolo_stevec = 0;
int gripper_stevec = 0;
int gripper_odstevec = 0;
int gripper_odstevec_old = 0;
int last_kolo_izhod_senzor = 0;
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
int cakanje_timer;
long dovoljen_cas_cakanja;
/////////////////////////////////////////
int last_start_cikla = 0;
int last_stop_cikla = 0;
int cikel = 0;
/////////////////////////////////////////
bool doza_velika;
bool doza_srednja;
bool doza_mala;
////////////////////////////////////////
int last_obrat_senzor = 0;
int tid = 0;
/////////////////////////
int last_kolo_vhod_senzor;
int last_kolo_obrat_senzor;
int last_kolo_vhod_pred_doze_senzor;
///////////////////////////////////////////
////////// TIMER /////////////////////////
long elTime;
long stTime;
int timState = 0;
//////////////////////////////////////////
/////// LEFT IN WHEEL ///////////////////
int obrat_count = 0;
int left_in_wheel = 0;
bool emptied;
int obrat_odstevec;
/////////////////////////////
bool stopped = 0;
////////////////////////////////
long nazadnje_obrnjeno = 0;
bool shranjen_obrat = false;
long razlika_obrat = 0;
///////////////////////////////////
long nazadnje_izhod = 0;
bool shranjen_izhod = false;
long razlika_izhod = 0;
///////////////////////////////////
long nazadnje_treseno = 0;
bool shranjeno_tresenje = false;
long razlika_tresenje = 0;
////////////////////////////////////
long t1 = 0;
////////////////////////////////////////
long t2 = 0;
////////////////////////////////////
int turn_count = 0;
/////////////////////////////////////
long t3 = 0;
bool shranjeno_preddoze = false;
long razlika_preddoze;
//////////////////////////////
long t4 = 0;
bool shranjeno_zapiranje_drce = false;
long razlika_zapiranje_drce;
long zapiranje_drce_timer;
bool first_time = true;
////////////////////////////////
long timer1;
long timer2;
long timer3;

long last_timer1;
long last_timer2;
long last_timer3;
///////////////////////////////
Mudbus Mb;
byte drive[] = { 192, 168, 1, 30 };
byte obuf [17];
EthernetClient client;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SETUP ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
  delay(30000);  
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
  Mb.R[20] = 0;
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
  pinMode(kolo_vhod_pred_doze_senzor_pin, INPUT);
  pinMode(debelina_doze1_pin, INPUT);
  pinMode(debelina_doze2_pin, INPUT);
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
  pinMode(enable_pin, OUTPUT);
  ////////////////////////////////////////////////////////////////////////////////

}
char rx_byte = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////// LOOP ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
  //zavora_odpuscena = digitalRead(zavora_odpuscena_pin);  
  //////////////////////////////////////////////////////////////////////////////
  if (state == -2)
  {
    Serial.println("-2");
  }
  if (state == -1)
  {
    Serial.println("-1");
  }
  if (state == 0)
  {
    Serial.println("0");
  }
  if (state == 1)
  {
    Serial.println("1111111111");
  }
  if (state == 2)
  {
    Serial.println("22222222222222222222");
  }
  if (state == 3)
  {
    Serial.println("333333333333333333333333333333");
  }
  if (state == 4)
  {
    Serial.println("4444444444444444444444444444444444444444");
  }
  if (state == 5)
  {
    Serial.println("55555555555555555555555555555555555555555555555555");
  }
  if (state == 6)
  {
    Serial.println("666666666666666666666666666666666666666666666666666666666666");
  }
  if (state == 7)
  {
    Serial.println("7777777777777777777777777777777777777777777777777777777777777777777777");
  }
  ///////////////////////////////////////////////////////////////////////////////
  Mb.Run();
  Mb.R[0] = varnostna_vrata_zaprta;
  Mb.R[1] = varnostna_vrata_zaklenjena;
  Mb.R[2] = akd_no_fault;
  Mb.R[3] = driver_kolesa_ready;
  Mb.R[4] = preklop_ciklov;
  Mb.R[5] = tlacni_senzor_vhod_stroja;
  Mb.R[6] = loputa_zaprta;
  Mb.R[7] = tlacni_senzor_gripper_zaprt;
  Mb.R[8] = home_position_reached;
  Mb.R[9] = max_position_reached;
  Mb.R[10] = konec_tresenja;
  Mb.R[11] = kolo_stevec;
  Mb.R[12] = gripper_stevec;
  Mb.R[13] = gripper_odstevec;
  Mb.R[14] = doza_velika;
  Mb.R[15] = doza_srednja;
  Mb.R[16] = doza_mala;
  Mb.R[17] = cikel;
  

  timer1 = (Mb.R[18])*60000;
  if(timer1 != last_timer1)
  {
    EEPROM.write(0, timer1);
  }
  if(timer2 != last_timer2)
  {
    EEPROM.write(1, timer2);
  }
  if(timer3 != last_timer3)
  {
    EEPROM.write(2, timer3);    
  }
  
  timer2 = (Mb.R[19])*1000;
  timer3 = (Mb.R[20])*1000;
  Serial.println("T1:");
  Serial.println(timer1);
  Serial.println("T2:");
  Serial.println(timer2);
  Serial.println("T3:");
  Serial.println(timer3);
  
  /////////////////////////////////////////////////////////////////////////
  if(start_tresenja_tipka == 1)
  {   
    digitalWrite(start_tresenja_pin, HIGH);
    delay(3000);
    digitalWrite(start_tresenja_pin, LOW);
  }
  //////////////////////////////////////////////////////////////////////////////////////////////
  if(kontroliran_stop_tipka == 1)
  {
    digitalWrite(kontroliran_stop_pin, HIGH);
    delay(1000);
    digitalWrite(kontroliran_stop_pin, LOW);
  }
  //////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////  
  if(cikel == 1 && preklop_ciklov == 0)// && vrata_zaklenjena == HIGH && vrata_zaprta == HIGH)
  {

    tlacni_senzor_vhod_stroja = digitalRead(tlacni_senzor_vhod_stroja_pin);
    stop_cikla = digitalRead(stop_cikla_pin);

    
    startTime = 0;
    startTime = millis();
    check_stop_cikla();
    //////////////////////////////////////////////////////////////
    if(shranjen_obrat == false)
    {
      razlika_obrat = startTime - startTime;
    }
    else
    {
      razlika_obrat = startTime - nazadnje_obrnjeno;
    }

    if(shranjeno_tresenje == false)
    {
      razlika_tresenje = startTime - startTime;
    }
    else
    {
      razlika_tresenje = startTime - nazadnje_treseno;
    } 

    
    if(shranjen_izhod == false)
    {
      razlika_izhod = startTime - startTime;
    }
    else
    {
      razlika_izhod = startTime - nazadnje_izhod;
    } 

    if(shranjeno_zapiranje_drce == false)
    {
      razlika_zapiranje_drce = startTime - startTime;
    }
    else
    {
      razlika_zapiranje_drce = startTime - t4;
    } 
    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    if(state == 7)
    {
      resetTimer();
    } 
    
    if(state == 6)
    {
      gripper_odstevalni_senzor = digitalRead(gripper_odstevalni_senzor_pin);
      zapiranje_drce();
      gripper_odstevanje();
      
    }
    
    if(state == 5)
    {
      tlacni_senzor_gripper_zaprt = digitalRead(tlacni_senzor_gripper_zaprt_pin);
      konec_tresenja = digitalRead(konec_tresenja_pin);
      kontroliran_stop_tipka = digitalRead(kontroliran_stop_tipka_pin);
      varnostna_vrata_zaprta = digitalRead(varnostna_vrata_zaprta_pin);
      varnostna_vrata_zaklenjena = digitalRead(varnostna_vrata_zaklenjena_pin);
      loputa_zaprta = digitalRead(loputa_zaprta_pin);
      stop_total = digitalRead(stop_total_pin);
      max_position_reached = digitalRead(max_position_reached_pin);
      stop_motion();
    }

    if(state == 4)
    {
      prepis_vrednosti();  
    }

    if(state == 3)
    {
      zacne_tresti();
    }

    if(state == 2)
    {
      gripperTimer();
      priprava_tresenje();
    }

    if (state == 1)
    {
      gripperTimer();
      predTresenje();   
    }

    if(state == 0)
    {
      kolo_vhod_senzor = digitalRead(kolo_vhod_senzor_pin);
      kolo_izhod_senzor = digitalRead(kolo_izhod_senzor_pin);
      kolo_obrat_senzor = digitalRead(kolo_obrat_senzor_pin);
      gripper_stevni_senzor = digitalRead(gripper_stevni_senzor_pin);
      kolo_vhod_pred_doze_senzor = digitalRead(kolo_vhod_pred_doze_senzor_pin);
      
      gripper_doza_postavljena = digitalRead(gripper_doza_postavljena_pin);
      home_position_tipka = digitalRead(home_position_tipka_pin);
      praznenje = digitalRead(praznenje_pin);
      home_position_reached = digitalRead(home_position_reached_pin);

      gripperTimer();
      gripper_stetje();
      kolo_stetje();
      obrat_kolesa();
      pogoji_start();  
    }
    
    if(state == -1)
    {
      preparation_shaking();  
      go_home();
  
    }      
    
  }

  else if(cikel == 0)
  {
    varnostna_vrata_zaprta = digitalRead(varnostna_vrata_zaprta_pin);
    varnostna_vrata_zaklenjena = digitalRead(varnostna_vrata_zaklenjena_pin);
    debelina_doze1 = digitalRead(debelina_doze1_pin);
    debelina_doze2 = digitalRead(debelina_doze2_pin);
    debelina_doze2 = digitalRead(debelina_doze2_pin);
    odpiranje_gripperja_tipka = digitalRead(odpiranje_gripperja_tipka_pin);
    zapiranje_gripperja_tipka = digitalRead(zapiranje_gripperja_tipka_pin);
    preklop_ciklov = digitalRead(preklop_ciklov_pin);
    start_cikla = digitalRead(start_cikla_pin);
    akd_no_fault = digitalRead(akd_no_fault_pin);
    driver_kolesa_ready = digitalRead(driver_kolesa_ready_pin);
    shranjen_obrat = false;
    shranjeno_tresenje = false;
    
    doze_check();
    check_start_cikla();
    home_position_manual();
    gripper_manual();
    ///////////////////////////////////////////
    startTime = 0;
    //elapsedTime = 0;
    storeTime = 0;
    
    digitalWrite(enable_pin, LOW);
    state = -1;
    kolo_stevec = 0;
    gripper_stevec = 0;
    gripper_odstevec = 0; 
    elTime = 0;
    stTime = 0;
    
    //digitalWrite(start_tresenja_pin, LOW)
    digitalWrite(obracanje_dozirnega_kolesa_pin, LOW);
    digitalWrite(zapora_drce_pin, LOW);
  }  

  //else if(cikel == 1 && preklop_ciklov == 1) //&& vrata_zaklenjena == HIGH && vrata_zaprta == HIGH)
  //{

    //obrat_kolesa_simple();
    //preparation_no_shaking();
  //}
  //obrat_kolesa();
  //gripper_manual();

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////// FUNKCIJE ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////STATE 0//////////////////////////////////////////////////////////////////////////////
void pogoji_start()
{  
  if ((kolo_stevec == doze_number && gripper_stevec == doze_number && gripper_odstevec == 0) || (razlika_izhod > timer2 && gripper_odstevec == 0 && ((kolo_stevec > 0) && (kolo_stevec < doze_number))   && kolo_stevec == gripper_stevec)) 
  {
    state = 1;
    timState = 1;
  }  
}
//////////////////////////////////////////////////////////////////////////STATE 1//////////////////////////////////////////////////////////////////////////////////////
void predTresenje()
{
  timState = 0;
  state = 2;
}
///////////////////////////////////////////////////////////////////STATE 2//////////////////////////////////////////////////////////////////////////////////////
void priprava_tresenje()   // gledam, ce so izpolnjeni pogoji za start tresenja. 1. prislo je 18 doz v gripper. 2. pretekel je cas. vsaj ena doza je v gripperju.
{
  if(elTime > 0)
  {
      digitalWrite(zapora_drce_pin, HIGH);
      digitalWrite(zapiranje_gripperja_pin, HIGH);
      delay(100);
      digitalWrite(zapiranje_gripperja_pin, LOW);
      state = 3;
  }
}
///////////////////////////////////////////////////////////////////STATE 3//////////////////////////////////////////////////////////////////////////////////////
void zacne_tresti()   // gledam, ce so izpolnjeni pogoji za start tresenja. 1. prislo je 18 doz v gripper. 2. pretekel je cas. vsaj ena doza je v gripperju.
{
  delay(100);
  digitalWrite(start_tresenja_pin, HIGH);
  delay(10);
  digitalWrite(start_tresenja_pin, LOW);

  state = 4;         
}
//////////////////////////////////////////////////////////////////////////STATE 4///////////////////////////////////////////////////////////////////////////////
void prepis_vrednosti() // prepis vrednosti v vmesnem stanju med netresenjem in tresenjem
{
  gripper_odstevec_old = gripper_stevec;
  gripper_odstevec = gripper_odstevec_old;
  
  left_in_wheel = obrat_count - kolo_stevec;
  kolo_stevec = 0;
  obrat_count = 0;
  emptied = false;
  gripper_stevec = 0;
  //elapsedTime = 0;
  elTime = 0;
  turn_count = 0;
  first_time = true;
  
  state = 5;
}
//////////////////////////////////////////////////////////////////////////////STATE 5////////////////////////////////////////////////////////////////////////////
void stop_motion()    // izkljucim signal za tresenje, ko konca tresti
{
  if(tlacni_senzor_gripper_zaprt == 0)
  {
    digitalWrite(kontroliran_stop_pin, HIGH);
    delay(5000);
    digitalWrite(kontroliran_stop_pin, LOW);
    state = -1;
    cikel = 0;
  }
  if (konec_tresenja == 1)
  {
    delay(2000);
    digitalWrite(zapora_drce_pin, LOW);
    digitalWrite(odpiranje_gripperja_pin, HIGH);
    delay(100);
    digitalWrite(odpiranje_gripperja_pin, LOW);
    delay(100);
    nazadnje_treseno = startTime;
    shranjeno_tresenje = true;
    state = 6;
  }
}
///////////////////////////////////////////////////////////////////////////////////STATE 6//////////////////////////////////////////////////////////////////////////
void zapiranje_drce ()
{
  if(gripper_odstevec == 0)
  {  
      if(first_time)
      {
        t4 = startTime;
        shranjeno_zapiranje_drce = true;
        first_time = false;  
      }
      
      if(razlika_zapiranje_drce > timer3)
      {
        digitalWrite(zapora_drce_pin, HIGH);
        state = 7;  
        shranjeno_zapiranje_drce = false;
        first_time = true;
      }
  }
}
///////////////////////////////////////////////////////////////////////////////// STATE 7 ////////////////////////////////////////////////////////////////////////7
void resetTimer() // ponastavljanje casovnika
{
  elTime = 0;
  //elapsedTime = 0;
  firstRun = 1;
  state = 0;
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
//////////////////////////////////////////////////////////////////////////////////////////
void check_start_cikla()
{
  if(start_cikla != last_start_cikla)
  {
    if(start_cikla == 1)
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
    if(stop_cikla == 1)
    {
      cikel = 0;
    }
  }
  last_stop_cikla = stop_cikla;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void kolo_stetje()   // stejem doze, ki grejo cez kolo.
{
  if (kolo_izhod_senzor != last_kolo_izhod_senzor)
  {
    if (kolo_izhod_senzor == 1 && (kolo_stevec < doze_number))
    {
      kolo_stevec++;
      nazadnje_izhod = startTime;
      shranjen_izhod = true;
    }
  }
  last_kolo_izhod_senzor = kolo_izhod_senzor;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gripper_stetje()   // stejem doze, ki pridejo mimo vhodnega senzorja. senzor na gripperju.
{
  if (gripper_stevni_senzor != last_gripper_stevni_senzor)
  {
    if (gripper_stevni_senzor == 1 && (gripper_stevec < doze_number))
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
    if (gripper_odstevalni_senzor == 1 && (0 < gripper_odstevec))
    {
      gripper_odstevec--;
    }
  }
  last_gripper_odstevalni_senzor = gripper_odstevalni_senzor;
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void doze_check()
{
  if(debelina_doze1 == 1 && debelina_doze2 == 0)
  {
    doze_number = 20;
    doza_velika = false;
    doza_srednja = true;
    doza_mala = false;
  }
  else if(debelina_doze1 == 0 && debelina_doze2 == 1)
  {
    doze_number = 18;
    doza_velika = true;
    doza_srednja = false;
    doza_mala = false;
  }
  else if(debelina_doze1 == 1 && debelina_doze2 == 1)
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
  delay(1000);
  digitalWrite(home_position_pin, HIGH);
  delay(1000);
  digitalWrite(home_position_pin, LOW);
}
/////////////////////////////////////////////////////////////////////////////////
void en_obrat_kolesa()
{
  digitalWrite(obracanje_dozirnega_kolesa_pin, HIGH);
  if (kolo_obrat_senzor == 1)
  {
    digitalWrite(obracanje_dozirnega_kolesa_pin, LOW);
  }
}
/////////////////////////////////////////////////////////////////////////////
void gripper_manual()
{
  if(odpiranje_gripperja_tipka == 1)
    {
      digitalWrite(odpiranje_gripperja_pin,HIGH);
      delay(500);
      digitalWrite(odpiranje_gripperja_pin,LOW);
    }
  if(zapiranje_gripperja_tipka == 1)
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
    delay(5000);
    digitalWrite(kontroliran_stop_pin, LOW);
    state = -1;
    cikel = 0;
  }
}
/////////////////////////////////////////////////////////////////////////////////
void home_position_manual()
{
  if(home_position_tipka == 1)
  {
    digitalWrite(home_position_pin, HIGH);
    delay(1000);
    digitalWrite(home_position_pin, LOW);
  }
}
///////////////////////////////////////////////////////////////////////////////////
void preparation_shaking()
{
  digitalWrite(enable_pin, HIGH);
  digitalWrite(zapora_drce_pin, HIGH);
  digitalWrite(odpiranje_gripperja_pin, HIGH);
  delay(500);
  digitalWrite(odpiranje_gripperja_pin, LOW);
  state = 0; 
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
void gripperTimer()
{
  if(timState == 1)
  {
    
    stTime = startTime;
    
  }
  elTime = startTime - stTime;
}
///////////////////////////////////////////////////////////////////////////////
void obrat_kolesa()
{
  
  if(kolo_stevec <= doze_number && kolo_izhod_senzor == LOW && kolo_vhod_senzor == HIGH && kolo_vhod_pred_doze_senzor == HIGH && kolo_obrat_senzor == HIGH)
  {
    digitalWrite(obracanje_dozirnega_kolesa_pin, HIGH);  
    delay(382);
    digitalWrite(obracanje_dozirnega_kolesa_pin, LOW);
    delay(300);  
    nazadnje_obrnjeno = startTime;
    shranjen_obrat = true;        
  }
  
  if((kolo_stevec <= doze_number) && (kolo_izhod_senzor == 0) && (razlika_tresenje > timer1)) ///(kolo_stevec <= doze_number) && (kolo_izhod_senzor == 0) && (razlika_tresenje > dovoljen_cas_cakanja) && (kolo_vhod_senzor == HIGH)
  {
    if(digitalRead(kolo_obrat_senzor_pin) == 1 && (kolo_stevec < doze_number))
    {
      if(startTime - t1 >= 1000 && turn_count < 7)
      {
        t1 = startTime;
        digitalWrite(obracanje_dozirnega_kolesa_pin, HIGH);
        delay(150);
        digitalWrite(obracanje_dozirnega_kolesa_pin, LOW); 
        turn_count++;
      }
    }
  }
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void prvi_obrat_kolesa()
{
  digitalWrite(obracanje_dozirnega_kolesa_pin, HIGH);  
  delay(582);
  digitalWrite(obracanje_dozirnega_kolesa_pin, LOW);
  delay(300);  
  nazadnje_obrnjeno = startTime;
  shranjen_obrat = true;
}
void prvo_tresenje()   // gledam, ce so izpolnjeni pogoji za start tresenja. 1. prislo je 18 doz v gripper. 2. pretekel je cas. vsaj ena doza je v gripperju.
{
  digitalWrite(zapora_drce_pin, HIGH);
  digitalWrite(zapiranje_gripperja_pin, HIGH);
  delay(100);
  digitalWrite(zapiranje_gripperja_pin, LOW);
  delay(100);
  digitalWrite(start_tresenja_pin, HIGH);
  delay(100);
  digitalWrite(start_tresenja_pin, LOW);     
}
/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
////state = - 1 (preparation)
////state = 0 (waiting for start-shaking conditions to be fullfiled)
////state = 1 (conditions met)
////state = 2 (predtresenje)
////state = 3 (priprava tresenje)
////state = 4 (shaking and value copying)
////state = 5 (end of shaking)
////state = 6 (closing latch)
////state = 7 (timer reset)
///////////////////////////////////////////////////////////////////////////////////

