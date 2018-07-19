////// IT - ELEKTRONIKA //////////////////////
//////// TRESENJE //////////////////////////
/////////////////////////////////////////////
// - main control of system logic
// - communication with HMI(RPI)
//
///////////LIBRARIES////////////////////////////
#include <SPI.h>
#include <Ethernet.h>
#include "Mudbus.h"
#include <SPI.h>
#include <EEPROM.h>
#include <digitalWriteFast.h>

#define gripper_stevni_senzor_pin 30
#define gripper_odstevalni_senzor_pin A6
#define kolo_izhod_senzor_pin 34
///////////////////////////////////////////////
/// INPUT PINS ///
int stop_total_pin = 22;
int varnostna_vrata_zaprta_pin = 23;
int varnostna_vrata_zaklenjena_pin = 24;
int akd_no_fault_pin = 25;
int driver_kolesa_ready_pin = 26;
int start_cikla_pin = A0;
int stop_cikla_pin = A1;
int kontroliran_stop_tipka_pin = A2;
int home_position_tipka_pin = A3;
int rocno_tresenje_pin = A4;
int odpiranje_gripperja_tipka_pin = A5;
int zapiranje_gripperja_tipka_pin = 27;
int preklop_ciklov_pin = 28;
int tlacni_senzor_vhod_stroja_pin = 29;
int gripper_doza_postavljena_pin = 31;
int loputa_zaprta_pin = A7;
int tlacni_senzor_gripper_zaprt_pin = A11;
int home_position_reached_pin = A9;
int max_position_reached_pin = A10;
int konec_tresenja_pin = A14;
int cikel2_pin = A8;
int miza_polna_pin = A15;
int kolo_obrat_senzor_pin = 35;
int kolo_vhod_pred_doze_senzor_pin = 33; // prej je bil A15
int debelina_doze1_pin = A12;
int debelina_doze2_pin = A13;
////////////////////////////
/// OUTPUT PINS  ///
///////////////////////////
int enable_pin = 49;
int odpiranje_gripperja_pin = 36;
int zapiranje_gripperja_pin = 37;
int zapora_drce_pin = 4;
int start_tresenja_pin = 39;
int kontroliran_stop_pin = 40;
int fault_reset_pin = 41;
int home_position_pin = 42;
int praznenje_pin = 43;
int obracanje_dozirnega_kolesa_pin = 44;
int zapora_vrat_pin = 45;
int stroj_pripravljen_pin = 46;
int stroj_running_pin = 47;
int stroj_napaka_pin = 48;
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
int gripper_doza_postavljena;
int loputa_zaprta;
int tlacni_senzor_gripper_zaprt;
int home_position_reached;
int max_position_reached;
int konec_tresenja;
int cikel2;
int kolo_obrat_senzor;
int kolo_vhod_pred_doze_senzor;
int debelina_doze1;
int debelina_doze2;
int miza_polna;
int rocno_tresenje;

bool vrti = false;
bool gripper_stevni_senzor;
bool gripper_odstevalni_senzor;
bool kolo_izhod_senzor;
////////////////////////////////////////////////////////////////////////
/// COUNTERS ///
int kolo_stevec = 0;
int gripper_stevec = 0;
int gripper_odstevec = 0;
int gripper_odstevec_old = 0;
bool last_kolo_izhod_senzor = 0;
bool last_gripper_stevni_senzor = 0;
bool last_gripper_odstevalni_senzor;
int last_konec_tresenja = 0;
//////////////////////////////////////////////
int state = -1;
long startTime;
int doze_number = 0;
/////////////////////////////////////////
int last_start_cikla = 0;
int last_stop_cikla = 0;
int cikel = 0;
/////////////////////////////////////////
bool doza_velika;
bool doza_srednja;
bool doza_mala;
////////////////////////////////////////
//int last_kolo_vhod_senzor;
int last_kolo_obrat_senzor;
int last_kolo_vhod_pred_doze_senzor;
////////////////////////////////////
bool prepared = false;
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
long t2 = 0;
long t3 = 0;
long t4 = 0;
long t5 = 0;
long t_izpust = 0;
long t_prihod = 0;
////////////////////////////////////
int turn_count = 0;
/////////////////////////////////////
bool shranjeno_preddoze = false;
long razlika_preddoze;
//////////////////////////////
bool shranjeno_zapiranje_drce = false;
long razlika_zapiranje_drce;
long zapiranje_drce_timer;
bool first_time = true;
////////////////////////////////
bool shranjen_t5 = false;
long razlika_t5;
////////////////////////////////
long shranjen_tIzpust;
long razlika_tIzpust;
bool startRun = true;
////////////////////////////////
long shranjen_tPrihod;
long razlika_tPrihod;
bool startRunPrihod = true;
////////////////////////////////
long timer1;  // dovoljen cas netresenja
long timer2;  // cas pred tresenjem
long timer3;  // cas do nepolnega tresenja
long timer5; // izpust doz
long timer6; // prihod doz
long kilo = 1000;
long two = 2;

long last_timer1;
long last_timer2;
long last_timer3;
long last_timer4;
long last_timer5;
long last_timer6;

bool praznenje = false;
/////////////////////////////
bool first_run = true;
///////////////////////////////
Mudbus Mb;
byte drive[] = { 192, 168, 1, 30 };
byte obuf [17];
EthernetClient client;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SETUP ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  delay(30000);  
  uint8_t mac[]     = { 0x90, 0xA2, 0xDA, 0x00, 0x51, 0x06 };
  uint8_t ip[]      = { 192, 168, 1, 90 };
  uint8_t gateway[] = { 192, 168, 1, 1 };
  uint8_t subnet[]  = { 255, 255, 255, 0 };
  Serial.begin(9600);
  Ethernet.begin(mac, ip, gateway, subnet);
  ////////////////////////////////////////////////////////////////////////////////////////
  Mb.R[0];
  Mb.R[1];
  Mb.R[2];
  Mb.R[3];
  Mb.R[4];
  Mb.R[5];
  Mb.R[6];
  Mb.R[7];
  Mb.R[8];
  Mb.R[9];
  Mb.R[10];
  Mb.R[11];
  Mb.R[12]; 
  Mb.R[13]; 
  Mb.R[14]; 
  Mb.R[15]; 
  Mb.R[16]; 
  Mb.R[17]; 
  Mb.R[18]; 
  Mb.R[19]; 
  Mb.R[20]; 
  Mb.R[21]; 
  Mb.R[22];
  Mb.R[23];
  Mb.R[24];
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
  pinMode(preklop_ciklov_pin, INPUT);
  pinMode(tlacni_senzor_vhod_stroja_pin, INPUT);
  pinModeFast(gripper_stevni_senzor_pin, INPUT);
  pinMode(gripper_doza_postavljena_pin, INPUT);
  pinModeFast(gripper_odstevalni_senzor_pin, INPUT);
  pinMode(loputa_zaprta_pin, INPUT);
  pinMode(tlacni_senzor_gripper_zaprt_pin, INPUT);
  pinMode(home_position_reached_pin, INPUT);
  pinMode(max_position_reached_pin, INPUT);
  pinMode(konec_tresenja_pin, INPUT);
  pinMode(cikel2_pin, INPUT);
  pinModeFast(kolo_izhod_senzor_pin, INPUT);
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
  pinMode(praznenje_pin, OUTPUT);
  pinMode(obracanje_dozirnega_kolesa_pin, OUTPUT);
  pinMode(zapora_vrat_pin, OUTPUT);
  pinMode(stroj_pripravljen_pin, OUTPUT);
  pinMode(stroj_running_pin, OUTPUT);
  pinMode(stroj_napaka_pin, OUTPUT);
  pinMode(enable_pin, OUTPUT);
  pinMode(miza_polna_pin, INPUT);
  pinMode(rocno_tresenje_pin, INPUT);
 ////////////////////////////////////////////////////////////////////////////////
}
char rx_byte = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////// LOOP ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  
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
  //Mb.R[18]
  //Mb.R[19]
  //Mb.R[20]
  //Mb.R[21]
  Mb.R[23] = razlika_tresenje/1000;
  Mb.R[24] = gripper_doza_postavljena;
  
  
  
  //if(EEPROM.read(0) >  999 && EEPROM.read(0) < 256000)
  //{
  //  timer1 = (EEPROM.read(0))*1000;  
  //}
  //else if(EEPROM.read(0) >  99 && EEPROM.read(0) < 1000)
  //{
  //  timer1 = (EEPROM.read(0))*100;
  //}
  //else if(EEPROM.read(0) >  9 && EEPROM.read(0) < 100)
  //{
  //  timer1 = (EEPROM.read(0))*10;
  //}
  //else if(EEPROM.read(0) < 10)
  //{
  //  timer1 = (EEPROM.read(0));
  //}
  
  timer1 = (EEPROM.read(0))*kilo;
  timer2 = (EEPROM.read(10))*kilo;
  timer3 = (EEPROM.read(20))*kilo;
  timer5 = (EEPROM.read(30))*kilo; 
  timer6 = (EEPROM.read(40)) *kilo;
  
  if(first_run)
  {   
    Mb.R[18] = timer1 / kilo;
    Mb.R[19] = timer2 / kilo;
    Mb.R[20] = timer3 / kilo;
    Mb.R[21] = timer5 / kilo;
    Mb.R[22] = timer6 / kilo;
    first_run = false; 
  }

  //if(Mb.R[18] >  999 && Mb.R[18] < 256000)
  //{
  //  EEPROM.update(0, Mb.R[18]/1000);    
  //}
  //else if(Mb.R[18] >  99 && Mb.R[18] < 1000)
  //{
  //  EEPROM.update(0, Mb.R[18]/100);
  //}
  //else if((Mb.R[18] >  9) && (Mb.R[18] < 100))
  //{
  //  EEPROM.update(0, Mb.R[18]/10);
  //}
  //else if(Mb.R[18] < 10)
  //{
  //  EEPROM.update(0, Mb.R[18]);
  //}
  //Serial.println("AKD NO FAULT:");
  //Serial.println(akd_no_fault);
  //Serial.println("TIMER1:");
  //Serial.println(timer1);
  ///Serial.println(EEPROM.read(0));
  //Serial.println("TIMER2:");
  //Serial.println(timer2);
  //Serial.println(EEPROM.read(10));
  //Serial.println("TIMER3:");
  //Serial.println(timer3);
  //Serial.println(EEPROM.read(20));
  //Serial.println("TIMER5:");
  //Serial.println(timer5);
  //Serial.println(EEPROM.read(30));
  //Serial.println("TIMER6:");
  //Serial.println(timer6);
  //Serial.println(EEPROM.read(40));


  EEPROM.update(0, Mb.R[18]);  
  EEPROM.update(10, Mb.R[19]);
  EEPROM.update(20, Mb.R[20]);    
  EEPROM.update(30, Mb.R[21]);  
  EEPROM.update(40, Mb.R[22]);

  //Serial.println("EEPROM:");
  

  //Serial.println("MB");
  //Serial.println(Mb.R[18]);

  /////////////////////////////////////////////////////////////////////////
  kontroliran_stop_tipka = digitalRead(kontroliran_stop_tipka_pin);
  tlacni_senzor_vhod_stroja = digitalRead(tlacni_senzor_vhod_stroja_pin);
  tlacni_senzor_gripper_zaprt = digitalRead(tlacni_senzor_gripper_zaprt_pin);
  varnostna_vrata_zaprta = digitalRead(varnostna_vrata_zaprta_pin);
  varnostna_vrata_zaklenjena = digitalRead(varnostna_vrata_zaklenjena_pin);
  loputa_zaprta = digitalRead(loputa_zaprta_pin);
  stop_total = digitalRead(stop_total_pin);
  gripper_doza_postavljena = digitalRead(gripper_doza_postavljena_pin);

  stop_cikla = digitalRead(stop_cikla_pin);

  konec_tresenja = digitalRead(konec_tresenja_pin);
  max_position_reached = digitalRead(max_position_reached_pin);
  cikel2 = digitalRead(cikel2_pin);
  home_position_reached = digitalRead(home_position_reached_pin);
  akd_no_fault = digitalRead(akd_no_fault_pin);
  
  miza_polna = digitalRead(miza_polna_pin);
  ///////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////
  if(kontroliran_stop_tipka == 1)
  {
    digitalWrite(kontroliran_stop_pin, HIGH);
    delay(1000);
    digitalWrite(kontroliran_stop_pin, LOW);
    cikel = 3;
  }
  //////////////////////////////////////////////////////////////////////////////////////////////
  if(akd_no_fault== HIGH)
  {
    cikel = 3;
  }
  //////////////////////////////////////////// DELOVNI CIKEL ///////////////////////////////////////////////////  
  if(cikel == 1 && preklop_ciklov == 0)
  {
    //Serial.println("CIKEL 1");
    digitalWrite(zapora_vrat_pin, HIGH);
    if(varnostna_vrata_zaklenjena == HIGH)
    {
      napaka_stetje();
      //Serial.println("VRATA ZAKLENJENA");
      akd_no_fault = digitalRead(akd_no_fault_pin);
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
  
      if(shranjen_t5 == false)
      {
        razlika_t5 = startTime - startTime;
      }
      else
      {
        razlika_t5 = startTime - t5;
      } 

      if(shranjen_tIzpust == false)
      {
        razlika_tIzpust = startTime - startTime;
      }
      else
      {
        razlika_tIzpust = startTime - t_izpust;
      } 

      if(shranjen_tPrihod == false)
      {
        razlika_tPrihod = startTime - startTime;
      }
      else
      {
        razlika_tPrihod = startTime - t_izpust;
      } 
      ////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////
      if(state == 7)
      {
        resetStep();
      } 
      if(state == 6)
      { 
      }    
      if(state == 5)
      {
        error_check();
        konec_tresenja = digitalRead(konec_tresenja_pin);
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
        priprava_tresenje();
      }
      if (state == 1)
      {
        
        predTresenje();   
      }
      if(state == 0)
      {
        kolo_vhod_pred_doze_senzor = digitalRead(kolo_vhod_pred_doze_senzor_pin);
        home_position_tipka = digitalRead(home_position_tipka_pin);
        home_position_reached = digitalRead(home_position_reached_pin);
        gripper_stetje();
        kolo_stetje();
        if(razlika_tPrihod > timer6 && startRunPrihod == false)
        {
          obrat_kolesa();      
        }
        else if(startRunPrihod == true)
        {
          //Serial.println("prvi run");
          
          shranjen_tPrihod = false;
          obrat_kolesa();
        }
        zapiranje_drce();
        pogoji_start();  
      }
      if(state == -1)
      {
        preparation_shaking();  
      } 
    }
  }
  //////////////////////////////// STOP CIKLA  - mirovanje//////////////////////////
  else if(cikel == 0)
  {
    varnostna_vrata_zaprta = digitalRead(varnostna_vrata_zaprta_pin);
    varnostna_vrata_zaklenjena = digitalRead(varnostna_vrata_zaklenjena_pin);
    debelina_doze1 = digitalRead(debelina_doze1_pin);
    debelina_doze2 = digitalRead(debelina_doze2_pin);
    debelina_doze2 = digitalRead(debelina_doze2_pin);

    preklop_ciklov = digitalRead(preklop_ciklov_pin);
    start_cikla = digitalRead(start_cikla_pin);
    akd_no_fault = digitalRead(akd_no_fault_pin);
    driver_kolesa_ready = digitalRead(driver_kolesa_ready_pin);
    shranjen_obrat = false;
    shranjeno_tresenje = false;

    tlacni_senzor_vhod_stroja = digitalRead(tlacni_senzor_vhod_stroja_pin);
    stop_cikla = digitalRead(stop_cikla_pin);
    tlacni_senzor_gripper_zaprt = digitalRead(tlacni_senzor_gripper_zaprt_pin);
    loputa_zaprta = digitalRead(loputa_zaprta_pin);
    stop_total = digitalRead(stop_total_pin);
    home_position_tipka = digitalRead(home_position_tipka_pin);
    
    doze_check();
    check_start_cikla();
    home_position_manual();
    ///////////////////////////////////////////
    startTime = 0;    
    digitalWrite(enable_pin, LOW);
    state = -1;
    kolo_stevec = 0;
    gripper_stevec = 0;
    prepared=false;
    digitalWrite(praznenje_pin, LOW);
    digitalWrite(obracanje_dozirnega_kolesa_pin, LOW);
    digitalWrite(zapora_drce_pin, LOW);
    digitalWrite(zapora_vrat_pin, LOW);

    if(cikel2 == HIGH && varnostna_vrata_zaprta == HIGH && akd_no_fault == LOW)
    {
      digitalWrite(zapora_vrat_pin, HIGH);
      delay(100);
      if(varnostna_vrata_zaklenjena == HIGH)
      {
        cikel = 2;  
      } 
    }    
  }  
  ////////////////////////////////////// DELOVNI CIKEL - brez tresenja //////////////////////////////
  else if(cikel == 1 && preklop_ciklov == 1)
  {
    digitalWrite(zapora_vrat_pin, HIGH);
    delay(100);
    varnostna_vrata_zaklenjena = digitalRead(varnostna_vrata_zaklenjena_pin);
    if(varnostna_vrata_zaklenjena == HIGH)
    {
      akd_no_fault = digitalRead(akd_no_fault_pin);
      if(prepared != true)
      {
        go_home();
        preparation_no_shaking();
        prepared = true;
      }
      no_tresenje_obrat_kolesa();
      check_stop_cikla();
    }
  }
  //////////////////////////////// ROCNI NACIN  ///////////////////////////////////////////
  else if(cikel == 2)
  {
    
    if(varnostna_vrata_zaprta == HIGH)
    {
      digitalWrite(enable_pin, HIGH); 
      stop_cikla = digitalRead(stop_cikla_pin);
      home_position_tipka = digitalRead(home_position_tipka_pin);
      odpiranje_gripperja_tipka = digitalRead(odpiranje_gripperja_tipka_pin);
      zapiranje_gripperja_tipka = digitalRead(zapiranje_gripperja_tipka_pin);
      gripper_manual();
      check_stop_cikla();
      home_position_manual();
      //rocnoTresenje();
      
      state = -2;  
    } 
  }
  //////////////////////////// - error mode ///////////////////////////////////////////////////
  else if(cikel = 3)
  {
    stop_cikla = digitalRead(stop_cikla_pin);
    state = -3;
    check_stop_cikla();
  }
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// FUNKCIJE ////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////STATE 0//////////////////////////////////////////////////////////////////////////////
void pogoji_start()
// odstanil gripper odstevec
{ 
  //Serial.println("RAZLIKA IZPUST:");
  //Serial.println(razlika_tIzpust); 
  bool kolo_izhod_senzor = digitalReadFast(kolo_izhod_senzor_pin);
  if ((razlika_tIzpust > timer5 && startRun == false && kolo_stevec == gripper_stevec && kolo_stevec == doze_number && gripper_stevec == doze_number && kolo_izhod_senzor == LOW && gripper_doza_postavljena == HIGH) || (kolo_stevec == gripper_stevec && razlika_izhod > timer3 && ((kolo_stevec > 0) && (kolo_stevec < doze_number) && gripper_doza_postavljena == HIGH)   && kolo_stevec == gripper_stevec && kolo_izhod_senzor == LOW)) 
  {
    digitalWrite(praznenje_pin, LOW);
    startRun = false;
    shranjen_tIzpust = false;
    digitalWrite(obracanje_dozirnega_kolesa_pin, LOW);
    digitalWrite(enable_pin, HIGH);
    state = 1;
    
  }

  else if ((startRun == true && kolo_stevec == gripper_stevec && kolo_stevec == doze_number && gripper_stevec == doze_number && kolo_izhod_senzor == LOW && gripper_doza_postavljena == HIGH) || (kolo_stevec == gripper_stevec && razlika_izhod > timer3 && ((kolo_stevec > 0) && (kolo_stevec < doze_number) && gripper_doza_postavljena == HIGH)   && kolo_stevec == gripper_stevec && kolo_izhod_senzor == LOW)) 
  {
    startRun = false;
    shranjen_tIzpust = false;
    digitalWrite(obracanje_dozirnega_kolesa_pin, LOW);
    digitalWrite(enable_pin, HIGH);
    state = 1;  
  }   
}
//////////////////////////////////////////////////////////////////////////STATE 1//////////////////////////////////////////////////////////////////////////////////////
void predTresenje()
{
  t5 = startTime;
  shranjen_t5 = true;
  state = 2;
}
///////////////////////////////////////////////////////////////////STATE 2//////////////////////////////////////////////////////////////////////////////////////
void priprava_tresenje()   
{
  if(razlika_t5 > timer2)
  {
    digitalWrite(zapiranje_gripperja_pin, HIGH);
    delay(100);
    digitalWrite(zapiranje_gripperja_pin, LOW);
    go_home();
    delay(1000);      
    digitalWrite(zapora_drce_pin, HIGH);
    state = 3;
  }
}
///////////////////////////////////////////////////////////////////STATE 3//////////////////////////////////////////////////////////////////////////////////////
void zacne_tresti()   // gledam, ce so izpolnjeni pogoji za start tresenja. 1. prislo je 18 doz v gripper. 2. pretekel je cas. vsaj ena doza je v gripperju.
{
  bool gripper_stevni_senzor = digitalReadFast(gripper_stevni_senzor_pin);
  if(gripper_stevni_senzor == LOW)
  { 
    
    if(home_position_reached == HIGH)
    {
      
      digitalWrite(start_tresenja_pin, HIGH);
      delay(100);
      digitalWrite(start_tresenja_pin, LOW);
      
      state = 4;  
    } 
  }
  else if(gripper_stevni_senzor == HIGH)
  {
    cikel = 3;         
  }
}
//////////////////////////////////////////////////////////////////////////STATE 4///////////////////////////////////////////////////////////////////////////////
void prepis_vrednosti() // prepis vrednosti v vmesnem stanju med netresenjem in tresenjem
{  
  kolo_stevec = 0;
  gripper_stevec = 0;
  turn_count = 0;
  first_time = true;
  startRunPrihod = false;
  shranjen_tPrihod = false;
  
  
  state = 5;
}
//////////////////////////////////////////////////////////////////////////////STATE 5////////////////////////////////////////////////////////////////////////////
void stop_motion()    // izkljucim signal za tresenje, ko konca tresti
{
  bool gripper_stevni_senzor = digitalReadFast(gripper_stevni_senzor_pin);
  if(gripper_stevni_senzor == HIGH)
  {
    cikel = 3;         
  }
  
  if (konec_tresenja == 1)
  {
    digitalWrite(enable_pin, LOW);
    miza_polna = digitalRead(miza_polna_pin);
    if(miza_polna == LOW)
    {
      digitalWrite(zapora_drce_pin, LOW);
      digitalWrite(odpiranje_gripperja_pin, HIGH);
      delay(100);
      digitalWrite(odpiranje_gripperja_pin, LOW);
      delay(100);
  
      t_prihod = startTime;
      shranjen_tPrihod = true;
      
      t_izpust = startTime;
      shranjen_tIzpust = true;
      
      nazadnje_treseno = startTime;
      shranjeno_tresenje = true;
      
      state = 0;
    }
  }
}
///////////////////////////////////////////////////////////////////////////////////STATE 6//////////////////////////////////////////////////////////////////////////
void zapiranje_drce()
{
  if(razlika_tIzpust > timer5)
  {
    digitalWrite(zapora_drce_pin, HIGH);
  }
}
///////////////////////////////////////////////////////////////////////////////// STATE 7 ////////////////////////////////////////////////////////////////////////7
void resetStep()
{
  state = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////
void check_start_cikla()
{
  if(akd_no_fault == LOW)
  {
    if(start_cikla != last_start_cikla)
    {
      if(start_cikla == 1 && varnostna_vrata_zaprta == HIGH)
      {
        cikel = 1;
      }
    }
    last_start_cikla = start_cikla;      
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void check_stop_cikla()
{
  Serial.println("STOP CIKLA");
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
  bool kolo_izhod_senzor = digitalReadFast(kolo_izhod_senzor_pin);
  if (kolo_izhod_senzor != last_kolo_izhod_senzor)
  {
    if (kolo_izhod_senzor == 1 ) // && (kolo_stevec < doze_number)
    {
      kolo_stevec++;
      //Serial.println("#####################################################");
      //Serial.println("                                      KOLO STEVEC:");
      //Serial.println(kolo_stevec);
      //Serial.println("#####################################################");
      nazadnje_izhod = startTime;
      shranjen_izhod = true;
    }
  }
  last_kolo_izhod_senzor = kolo_izhod_senzor;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gripper_stetje()   // stejem doze, ki pridejo mimo vhodnega senzorja. senzor na gripperju.
{
  bool gripper_stevni_senzor = digitalReadFast(gripper_stevni_senzor_pin);
  if (gripper_stevni_senzor != last_gripper_stevni_senzor)
  {
    if (gripper_stevni_senzor == 1 && (gripper_stevec < doze_number))
    {
      gripper_stevec++;
      //Serial.println("                        GRIPPER STEVEC:");
      //Serial.println(gripper_stevec);
    }
  }
  last_gripper_stevni_senzor = gripper_stevni_senzor;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gripper_odstevanje() // stejem doze, katere grejo iz gripperja ven. po koncanem tresenju
{
  bool gripper_odstevalni_senzor = digitalReadFast(gripper_odstevalni_senzor_pin);
  
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
  delay(100);
  digitalWrite(home_position_pin, HIGH);
  delay(100);
  digitalWrite(home_position_pin, LOW); 
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
    delay(100);
    digitalWrite(kontroliran_stop_pin, LOW);
    cikel = 3;
  }
  if(max_position_reached == HIGH)
  {
    cikel = 3;  
  }
}
/////////////////////////////////////////////////////////////////////////////////
void home_position_manual()
{
  if(home_position_tipka == 1 && varnostna_vrata_zaprta == HIGH)
  {
    digitalWrite(enable_pin, HIGH);
    digitalWrite(home_position_pin, HIGH);
    
    delay(1000);
    digitalWrite(home_position_pin, LOW);
  }

}
///////////////////////////////////////////////////////////////////////////////////
void preparation_shaking()
{
  
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
///////////////////////////////////////////////////////////////////////////////
void obrat_kolesa()
{
  
  bool gripper_stevni_senzor = digitalReadFast(gripper_stevni_senzor_pin);
  bool kolo_izhod_senzor = digitalReadFast(kolo_izhod_senzor_pin);
  kolo_obrat_senzor = digitalRead(kolo_obrat_senzor_pin);
  
  //Serial.println("DOZE NUMBER:");
  //Serial.println(doze_number);
  //Serial.println("KOLO STEVEC:");
  //Serial.println(kolo_stevec);
  //Serial.println("KOLO VHOD PREDDOZE:");
  //Serial.println(kolo_vhod_pred_doze_senzor);
  //Serial.println("KOLO IZHOD:");
  //Serial.println(kolo_izhod_senzor);
  
  
  if(kolo_stevec < doze_number && kolo_izhod_senzor == LOW && kolo_vhod_pred_doze_senzor == HIGH) // pobral ven && kolo_vhod_senzor == HIGH
  {
    //Serial.println("OBRAT do doze_number");
    digitalWrite(obracanje_dozirnega_kolesa_pin, HIGH);        
    nazadnje_obrnjeno = startTime;
    shranjen_obrat = true;
    vrti = true;        
  }
  //Serial.println("DOZE NUMBER:");
  //Serial.println(doze_number);
  //Serial.println("VRTI:");
  //Serial.println(vrti);
  //Serial.println("kolo_obrat_senzor:");
  //Serial.println(kolo_obrat_senzor);
  
  //if(vrti == true && kolo_obrat_senzor == HIGH)
  //{
  //  Serial.println("STOP VRTENJA");
  //  digitalWrite(obracanje_dozirnega_kolesa_pin, LOW);  
  //  vrti = false;
  //}
  

  if(kolo_stevec == doze_number)
  {
    digitalWrite(obracanje_dozirnega_kolesa_pin, LOW);
  }
  
  if((kolo_stevec < doze_number) && (kolo_izhod_senzor == 0) && (razlika_tresenje > timer1))
  {
    
    if(kolo_stevec < doze_number)
    {
      Serial.println("TURN COUNT:");
      Serial.println(turn_count);
      if(turn_count < 14)
      {
        praznenje = true;
        t1 = startTime;
        digitalWrite(obracanje_dozirnega_kolesa_pin, HIGH); 
        digitalWrite(praznenje_pin, HIGH);
        
      }
      else if(turn_count > 14)
      {
        praznenje = false;
        digitalWrite(obracanje_dozirnega_kolesa_pin, LOW); 
        //digitalWrite(praznenje_pin, LOW);
      }
    }
  }

  if(praznenje == true)
  {
    if(!kolo_obrat_senzor == last_kolo_obrat_senzor){
      if(kolo_obrat_senzor == HIGH){
        turn_count++;
      }
    }
    last_kolo_obrat_senzor = kolo_obrat_senzor;
  }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void no_tresenje_obrat_kolesa()
{
  bool kolo_izhod_senzor = digitalReadFast(kolo_izhod_senzor_pin);
  kolo_vhod_pred_doze_senzor = digitalRead(kolo_vhod_pred_doze_senzor_pin);
  
  if(kolo_izhod_senzor == LOW && kolo_vhod_pred_doze_senzor == HIGH) // pobral ven && kolo_vhod_senzor == HIGH  
  {
    digitalWrite(obracanje_dozirnega_kolesa_pin, HIGH);    
  } 
}
//////////////////////////////////////////////////////////////////////////////
void rocnoTresenje()
{
  rocno_tresenje = digitalRead(rocno_tresenje_pin);
  if(rocno_tresenje == HIGH)
  {

    digitalWrite(zapiranje_gripperja_pin, HIGH);
    delay(100);
    digitalWrite(zapiranje_gripperja_pin, LOW);      
    delay(100);
    digitalWrite(zapora_drce_pin, HIGH);
    delay(100);
    digitalWrite(start_tresenja_pin, HIGH);
    delay(100);
    digitalWrite(start_tresenja_pin, LOW);    
  }
}
/////////////////////////////////////////////////////////////////////////////////
void napaka_stetje(){
  if(kolo_stevec > doze_number)
  {
    cikel = 3;
  }
}

/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
////state = -1 (preparation)
////state = 0 (waiting for start-shaking conditions to be fullfiled)
////state = 1 (conditions met)
////state = 2 (predtresenje)
////state = 3 (priprava tresenje)
////state = 4 (shaking and value copying)
////state = 5 (end of shaking)
////state = 6 (closing latch)
////state = 7 (last step)
///////////////////////////////////////////////////////////////////////////////////

