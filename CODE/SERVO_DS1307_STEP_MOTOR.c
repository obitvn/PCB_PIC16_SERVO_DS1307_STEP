#include <SERVO_DS1307_STEP_MOTOR.h>
#include <ds1307.c>
#use delay(clock=20000000)

#define LCD_ENABLE_PIN PIN_B3
#define LCD_RS_PIN PIN_B5
#define LCD_RW_PIN PIN_B4
#define LCD_DATA4 PIN_B2
#define LCD_DATA5 PIN_B1
#define LCD_DATA6 PIN_D7
#define LCD_DATA7 PIN_D6

#include <lcd.c>


int sec;
int min;
int hrs;


int mode, set_up_value, set_hour, set_minute;

int count, on_time;


/*

void TIMER0_isr(void) 
{
set_timer0(252);
count++;
  if (count >= on_time)
    {
        output_bit(Pin_B0,1);  // complement the value for blinking the LEDs
    }
    
    if (count >= (on_time+(200-on_time)))
    {
        output_bit(Pin_B0,0);
        count=0;
    }
}

void RunServo(int goc)
{
 on_time = (170-goc);
}
*/

void servoRotate0() //0 Degree
{
  unsigned int i;
  for(i=0;i<5;i++)
  {
    output_bit(Pin_B0,1);
    Delay_us(800);
    output_bit(Pin_B0,0);
    Delay_us(19200);
  }
}

void servoRotate45() //90 Degree
{
  unsigned int i;
  for(i=0;i<5;i++)
  {
    output_bit(Pin_B0,1);
    Delay_us(1300);
    output_bit(Pin_B0,0);
    Delay_us(18700);
  }
}



void ScanButton()
{
 if(input(Pin_A4) == 0)
      {
      while(input(Pin_A4)== 0){}
      delay_ms(10);
      mode++;
      if(mode>5) mode = 0;
      }
 if(mode==0)
 {
   if(input(Pin_E0) == 0)
      {
      while(input(Pin_E0) == 0){}
      mode=3;
      }
 }
 if(mode>2)
 {
 if(input(Pin_E0) == 0)
      {
      while(input(Pin_E0) == 0){}
      set_up_value++;
      }
 else if(input(Pin_E1) == 0)
      {
      while(input(Pin_E1) == 0){}
      set_up_value--;
      }
 }
}

/*

void Run_Step(int dir)
{
  
  output_bit(Pin_C0,0);
  if(dir==1) output_bit(Pin_C2,1);
   else output_bit(Pin_C2,0);
  
  while(1)
  {
   output_bit(Pin_C1,1);
   delay_ms(10);
   output_bit(Pin_C1,0);
   delay_ms(10);
  }
}
*/

void Mode_Servo()
{
         servoRotate45();
         delay_ms(500);
         servoRotate0();
}

void Mode_Step_Servo()
{
         output_bit(Pin_C2,1);
         while((input(Pin_D5)==1)&&(min % 2 == 1)) //sw1
         {
          ds1307_get_time (hrs, min, sec);
          lcd_gotoxy(1,2);
          printf(lcd_putc,"TIME: %02u:%02u:%02u",hrs, min, sec);
          output_bit(Pin_C1,1);
          delay_ms(20);
          output_bit(Pin_C1,0);
          delay_ms(20);
         }
         servoRotate45();
         delay_ms(500);
         servoRotate0();
         delay_ms(100);
         output_bit(Pin_C2,0);
         while((input(Pin_D4)==1)&&(min % 2 == 1)) //sw1
         {
          ds1307_get_time (hrs, min, sec);
          lcd_gotoxy(1,2);
          printf(lcd_putc,"TIME: %02u:%02u:%02u",hrs, min, sec);
          output_bit(Pin_C1,1);
          delay_ms(20);
          output_bit(Pin_C1,0);
          delay_ms(20);
         }
}

void main()
{

   
   lcd_init();
   lcd_gotoxy(1,1);
   lcd_putc("INIT HARDWARE");
   output_float(Pin_A5);
   delay_ms(500);
   servoRotate0();
   mode=0;
   while(TRUE)
   {
      //TODO: User Code
      //ds1307_get_time (hrs, min, sec);   /// get time from rtc ds1307
      ScanButton(); // 
      if(mode == 0 )
      {
        ds1307_get_time (hrs, min, sec);
        lcd_gotoxy(1,1);
        lcd_putc("HOME           ");
        lcd_gotoxy(1,2);
        printf(lcd_putc,"TIME: %02u:%02u:%02u",hrs, min, sec);
      }
      else if(mode == 1 ) //HAND_MODE
       {
        ds1307_get_time (hrs, min, sec);
        lcd_gotoxy(1,1);
        lcd_putc("MANUAL MODE       ");
        lcd_gotoxy(1,2);
        printf(lcd_putc,"TIME: %02u:%02u:%02u",hrs, min, sec);
        if(input(Pin_E0) == 0)
        {
         Mode_Servo();
        }
        else if(input(Pin_E1) == 0)
        {
         while(input(Pin_E1) == 0){}
         output_bit(Pin_C2,1);
         while(input(Pin_D5)==1) //sw1
         {
          output_bit(Pin_C1,1);
          delay_ms(20);
          output_bit(Pin_C1,0);
          delay_ms(20);
         }
         servoRotate45();
         delay_ms(500);
         servoRotate0();
         delay_ms(100);
         output_bit(Pin_C2,0);
         while(input(Pin_D4)==1) //sw1
         {
          output_bit(Pin_C1,1);
          delay_ms(20);
          output_bit(Pin_C1,0);
          delay_ms(20);
         }
        }
       }
      else if(mode == 2) // AUTO
      {
       
        lcd_gotoxy(1,1);
        lcd_putc("AUTO MODE         ");
        lcd_gotoxy(1,2);
        printf(lcd_putc,"TIME: %02u:%02u:%02u",hrs, min, sec);
        ds1307_get_time (hrs, min, sec);
        if(min % 2 == 0) // chan
        {
         Mode_Servo();
         while((min % 2 == 0))
         {
          ds1307_get_time (hrs, min, sec);
          lcd_gotoxy(1,2);
          printf(lcd_putc,"TIME: %02u:%02u:%02u",hrs, min, sec);
         }
        }
        else // le
        {
         Mode_Step_Servo();
         while((min % 2 == 1))
         {
          ds1307_get_time (hrs, min, sec);
          lcd_gotoxy(1,2);
          printf(lcd_putc,"TIME: %02u:%02u:%02u",hrs, min, sec);
         }
        }
        
      }
      else if(mode == 3) //set time
      {
       set_hour = set_up_value;
       lcd_gotoxy(1,1);
       printf (lcd_putc, "SET HOUR: %d     ", set_hour);
       if(set_hour>12)
       {
       set_hour=0;
       set_up_value =0;
       }
      }
      else if(mode == 4) //set minute
      {
       set_minute = set_up_value;
       lcd_gotoxy(1,1);
       printf (lcd_putc, "SET MINUTE: %d     ", set_minute);
       if(set_minute>59)
       {
       set_minute=0;
       set_up_value =0;
       }
      }
      else if(mode == 5)
      {
       lcd_gotoxy(1,1);
       lcd_putc("SAVE SETTING ..");
       ds1307_set_date_time(18,2,16,0,set_hour,set_minute,0); // set time and day
       mode=0;
      }

   }

}
