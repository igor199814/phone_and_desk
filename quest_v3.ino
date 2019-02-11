#include <math.h>
#include <Bounce.h>
          //телефон
          //disk_dialer
          //красный, желтый, синий - через R=10k к земле     
          int tmp;          
          int disk_k1 = 4; //контакт импульсов номеронабирателя - желтый
          int disk_k2 = 6; //шунт, контакт сигнала начала ввода - синий
          byte count = 0;
          Bounce num = Bounce(disk_k1, 5);
          Bounce flag = Bounce(disk_k2, 5);
          //disk_dialer
          #define brekers_tube 2
          #define ring_pin 3 
          bool state_ring=false, relay1_state=false;
          //телефон
          String dialed_number="";
          
          String key_number="5094";
          
          int short_pause=0,long_pause=3000, short_ring=30,long_ring=80;//pause-перерва після (number_ring) дзвінків, ring-перерва між ударами
          byte number_ring=50,count_ring=number_ring;
          unsigned long last_ring=0;
          unsigned long last_ring_pause=0;
          bool flag_ring=false;

          //реле
          #define relay1 9
          #define relay2 10
          //реле
          #define connect_wire1 7
          #define connect_wire2 8
          //буззер
          bool flag_buzzer[3];
          int buze = 11; 
          unsigned long last_high_buzzer =0;
          //буззер
void setup() 
{         
          //телефон
          //disk_dialer
          pinMode(disk_k1, INPUT);
          pinMode(disk_k2, INPUT);
          Serial.begin(57600);
          //disk_dialer
          pinMode(brekers_tube, INPUT);
          pinMode(ring_pin,OUTPUT);
          digitalWrite(ring_pin,LOW);
          //телефон            

          //реле
          pinMode(relay1,OUTPUT);
          digitalWrite(relay1,relay1_state);
          pinMode(relay2,OUTPUT);
          digitalWrite(relay2,LOW);
          //реле
          
          //буззер
          pinMode(buze,OUTPUT);
          digitalWrite(buze,LOW);
          //буззер
}

void loop() 
{
 disk_dialer();
 if (digitalRead(connect_wire1)==1)//connect_wire1)==1
 {   flag_buzzer[0]=buzzer(flag_buzzer[0]);  
     if ((key_number==dialed_number)&&(flag_ring==false))
     {
         
          for (int i=0; i <= 1000; i++)
          {
               //if (digitalRead(brekers_tube)==0)              
               ringer(long_ring,short_pause);
               Serial.println("Ring");
          }
          flag_ring=true;
          digitalWrite(ring_pin, LOW);
          relay1_state=true;
          digitalWrite(relay1,relay1_state);
          flag_buzzer[1]=buzzer(flag_buzzer[1]);   
     }
     else if ((digitalRead(brekers_tube)==0)&&(flag_ring==false))
     {
        ringer(short_ring,long_pause);
     }
     else 
     {
        digitalWrite(ring_pin, LOW);
     }
 }
 if ((digitalRead(connect_wire2)==1)&&(relay1_state==true)) //connect_wire2)==1
 {
    digitalWrite(relay2,HIGH);
    flag_buzzer[2]=buzzer(flag_buzzer[2]); 
    
 }
 Serial.print("digitalRead(brekers_tube)="); 
 Serial.println(digitalRead(brekers_tube));
 Serial.print("dialed_number-"); 
 Serial.println(dialed_number);  

//Serial.print("digitalRead(connect_wire1)"); 
 //Serial.println(digitalRead(connect_wire1));
}

void disk_dialer()
{
   if (digitalRead(brekers_tube)==1)
   {   
      if ( num.update() ) 
      {
          if (num.read() == HIGH) 
          {
            count++;
          }
      }
      
      if ( flag.update() ) 
      {
      
        if (flag.read() == HIGH) 
        {
            count=0;
        }
        else
        {
          if ((count)!=0)
          {
          if ((count)==10) count=0;
          dialed_number+=String(count);
          }
        }
        
      }
    }
    else
    {
      dialed_number="";
    }

}
void ringer(int interval_ring,int interval_pause)//pause-перерва після (number_ring) дзвінків, ring-перерва між ударами
{
      if (number_ring>0)
      {
        if (abs(millis()-last_ring)>=interval_ring)
        {
          state_ring=!state_ring;
          digitalWrite(ring_pin, state_ring);
          last_ring=millis();
          number_ring-=1;
        }   
      }
      else if(abs(millis()-last_ring_pause)>=interval_pause)//interval
      {
      /*Serial.print(millis());
        Serial.print("-");
        Serial.print(last_ring_pause);
        Serial.print("=");
        Serial.println(abs(millis()-last_ring_pause));
      */
        number_ring=count_ring;
        last_ring_pause=millis();
        Serial.println("number_ring=count_ring;");
      }
}

bool buzzer(bool flag_buzzer)
{
 /* if (abs(millis()-last_high_buzzer)>=100)
  {
    digitalWrite(buze,HIGH);
    flag_buzzer=true;
  /*Serial.print(millis());
    Serial.print("-");
    Serial.print(last_high_buzzer);
    Serial.print("=");
    Serial.println(abs(millis()-last_high_buzzer));
  */
  //}
  /*
  else if (flag_buzzer==false)
  {
    digitalWrite(buze,LOW);
    flag_buzzer=true;
    last_high_buzzer=millis();
     //Serial.print("=");
    //Serial.println(flag_buzzer);

  }*/
  if (flag_buzzer==false)
  {
  tone(buze,2000,500);
  flag_buzzer=true;
  }
return flag_buzzer;
}
