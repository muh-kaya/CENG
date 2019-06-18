/*
 ***GROUP 10***
 * 2099166 Muhammet KAYA
 * 2099174 İbrahim Atacan KERPİÇ
 */

#include <p18cxxx.h>
#include <p18f8722.h>
#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#define _XTAL_FREQ   40000000

#include "Includes.h"
#include "LCD.h"




unsigned int t250_count = 0, t100_count = 0,t500_count=0,oldvalue=-1, value, convalue,att=2,t1_sec=0;
unsigned int pins = 0 , index = 11, pindex=0, toggle_500 = 0;
int time=120;
char fset[16] = " Set a pin:";
char enter[16] = " Enter pin:";
char attempt[16] = "  Attempts:";//5
char pinset[16] = "   ---    ---   ";
int pin[4], enterpin[4];
int rb6flag = 0, rb7flag = 0, rb6_flag = 0 , sec3flag=1, rb6_flag_2=0,flag_sec1=0;
int finish=0;
int oldtime=0;
int newtime=0;
int endtime=0;
unsigned char SD7[4], iter;


void beginLCD();
void setLCD(char *fset);
int  convert(unsigned int value);
void init();
void pinsetLCD();
void enterLCD(char* enter,char* attempt);
void finishLCD();
void pinleribasLCD();
void wrong_pinLCD();
void clear7SD();
void update_SD7();
unsigned char getDisp(unsigned char c);
void gettime();

void interrupt high_isr(void)//interrupt function
{
    update_SD7();

    if(RBIE && RBIF)//rb change interrupt
    {
        if(!RB6 && rb6_flag)//RB6 setting pin interrupt
        {
            pin[pindex] = convert(ADRES);//save current ad value
            //fset[index] = pin[pindex] + '0';
            pindex++;//increased pindex
            index++;//increased index
            rb6flag=1;//set rb6flag 1
            rb6_flag = 0;
        }
        else if(!RB6 && rb6_flag_2 )//RB6 entering pin interrupt
        {
            enterpin[pindex] = convert(ADRES);//save current ad value
            //enter[index] = enterpin[pindex] + '0';
            pindex++;//increased pindex
            index++;//increased index
            rb6flag=1;//set rb6flag 1
            rb6_flag_2 = 0;
        }
        else if(!RB7)//if rb7 change interrupt
        {
            rb7flag = 1;//set RB7 flag
        }
        RBIF = 0;//rb interrupt 0
    }

    if(T0IE && T0IF)//timer0 interrupt
    {
        t250_count++;//increase 250ms counterc
        t100_count++;//increase 100ms counter
        t500_count++;//increase 500ms counter
        if(t250_count == 50)//when 250 ms passed
        {
            if(pins == 0)//changed pins flag for blinkng
            {
                pins = 1;
            }
            else
            {
                pins = 0;
            }
            t250_count = 0;//clear 250ms counter
        }
        if(t100_count==20)//when 100 ms passed
        {
            t100_count=0;//clear 100ms counter
            GO = 1;//adcon interrupt
        }

        if(t500_count == 100)//when 500 ms passed
        {
            if(toggle_500 == 0)//for bilinking setting pin screen
            {
                toggle_500 = 1;
            }
            else
            {
                toggle_500 = 0;
            }
            sec3flag--;//decrease flag for count 3 sec
            t500_count=0;//clear 500ms counter

        }

        TMR0 = 61;//for 50ms
        T0IF = 0;//tmr0 flag = 0
    }

    if(TMR1IE && TMR1IF)//timer1 interrupt
    {
        t1_sec++;//timer1 counter
        if(t1_sec == 20)//when 1sec passed
        {
            if(time==0)//if time is over
                endtime=1;//endtimeflag = 1 for restart

            time--;//decerase time
            t1_sec = 0;//clear timer1counter
            flag_sec1 = 1;//1 sec flag
        }
        TMR1L = 220;//initial value for 50ms
        TMR1H = 11;
        TMR1IF = 0;//cleartimer1 flag
    }

    if(ADIE && ADIF)//check adcon interrupt
    {
        ADIF = 0;//clear int adcon flag
        value = ADRES;//take value
        convalue = convert(value);//convert value into 0-9
    }
    update_SD7();

    return;

}
void init()
{
    GIE = 0; //disable global interrupt

    TRISE1 = 1; // take RE1 as input

    INTCON = 0; // clear INTCON

    TRISB = 0;  // clear TRISB
    TRISB6 = 1; //take RB6 as input
    TRISB7 = 1; //take RB7 as input
    PORTB = 0;  //clear PORTB
    LATB = 0;   //clear PORTB
    RBIE = 1;   //enable rb interrupts
    RBIF = 0;   //clear rb interrupt flag
    RBPU = 0;   //enable pull-ups

    TMR0IE = 1; // enable timer0
    T0CON = 0;  // clear t0con
    T08BIT = 1; // make t0 8bit
    PSA = 0;    // enable prescaler
    T0PS0 = T0PS1 = T0PS2 = 1; //(count until 256)
    TMR0 = 61; //start tmr0 from 61 for 50ms
    T0IF = 0; //tmr0 interrupt flag=0
    TMR0ON = 1; //tmr0 is on

    TMR1IE = 1; //enable tmr1
    TMR1L = 220; //for 50ms interrupt(timer1)
    TMR1H = 11;  //for 50ms
    T1CKPS1 = T1CKPS0 = 1; //prescaler is 8


    CHS3 = CHS2 = 1; //set channel 12
    ADIE = 1;       //enable ADCON interrupt
    ACQT2 = 1;      //8*1+2*1=10  (potentiometer is used between 0 and 9)
    ACQT1 = 0;
    ACQT0 = 1;
    ADIF = 0;      //ADCON INT. FLAG=0
    ADFM = 1;      //right justify
    ADON = 1;      //ADCON is on


    TRISH4 = 1;   //output for led segment and lcd disp

    //Restarting values of all variables
    t250_count = 0, t100_count = 0,t500_count=0,t1_sec=0;
    pins = 0 , index = 11, pindex=0, toggle_500 = 0;
    rb6flag = 0, rb7flag = 0, rb6_flag = 0 , sec3flag=1, rb6_flag_2=0,flag_sec1=0;
    finish=0;
    oldtime=0;
    endtime=0;
    pin[0]=pin[1]=pin[2]=pin[3]=0;
    enterpin[0]=enterpin[1]=enterpin[2]=enterpin[3]=0;
    att = 2;


}

// Main Function
void main(void)
{

    while(!finish)//if the program is finish then this while will be ended(finish flag will be 1 if the program have to end)
    {

        InitLCD();            // Initialize LCD in 4bit mode

        init();                    //Initializing

        ClearLCDScreen();           // Clear LCD screen

        beginLCD();//call begin LCD which display first screen of the program to LCD

    while(1)//check for RE1 button
    {

            if(RE1 == 0)//if RE1 is pressed
            {
                while(1)//wait for released
                {
                    if(RE1 == 1)//if RE1 is released
                    {
                        for(int i=0; i<200; i++)//when released wait 3 sec.
                            __delay_ms(15);
                        break;
                    }
                }
                break;
            }
    }
    clear7SD();//clear 7 segment display
    GIE = 1;//enable global interrupt

    ClearLCDScreen();// Clear LCD screen
    setLCD(fset);//write string part of setting screen to LCD
    WriteCommandToLCD(0x8B);//write led to 4 # charachter between 11-15 indexes
    WriteStringToLCD("####");

    oldvalue = convert(ADRES);

    while(1)// for setting 4 bit pin after waiting 3 second
    {


        update_SD7();//update 7 segment display (----)

        if(pindex <= 3)//check for bound of pin
        {
            if(oldvalue != convalue)//check for potentimeter changes if changes write to LED
            {

                if(index == 11)//control which index will write
                {
                    WriteCommandToLCD(0x8B);  //if index==11 then display converted ADC value
                    WriteDataToLCD(convalue+'0');
                }
                if(index == 12)
                {
                    WriteCommandToLCD(0x8C);   //if index==12 then display converted ADC value
                    WriteDataToLCD(convalue+'0');
                }
                if(index == 13)
                {
                    WriteCommandToLCD(0x8D);   //if index==13 then display converted ADC value
                    WriteDataToLCD(convalue+'0');
                }
                if(index == 14)
                {
                    WriteCommandToLCD(0x8E);   //if index==14 then display converted ADC value
                    WriteDataToLCD(convalue+'0');
                }

                if(rb6flag == 0)//check for RB6 button interrupt  if RB6 is not pushed
                {
                    rb6_flag = 1;//
                    oldvalue = -1;//oldvalue = -1 then # charac. is not blinking #
                }
                else//// if RB6 is pushed then 
                {
                    oldvalue = convalue;// if RB6 is pushed then oldvalue = convalue then after index changes # charachter blinking
                    rb6flag = 0;//RB6 interrupt flag=0
                    rb6_flag = 0;
                }
            }
            else
            {
                update_SD7();//update 7 segment dispplay
                if(pins == 1)//pins used for # charachter blinking if 1 then display '#' else ' '
                {
                    if(index == 11)
                    {
                        WriteCommandToLCD(0x8B);   //if index==11 then display '#'
                        WriteDataToLCD('#');
                    }
                    if(index == 12)
                    {
                        WriteCommandToLCD(0x8C);   //if index==12 then display '#'
                        WriteDataToLCD('#');
                    }
                    if(index == 13)
                    {
                        WriteCommandToLCD(0x8D);   //if index==13 then display '#'
                        WriteDataToLCD('#');
                    }
                    if(index == 14)
                    {
                        WriteCommandToLCD(0x8E);  //if index==14 then display '#'
                        WriteDataToLCD('#');
                    }

                }
                else if(pins == 0)//pins = 0 so display ' '
                {
                    if(index == 11)
                    {
                        WriteCommandToLCD(0x8B);   //if index==11 then display ' '
                        WriteDataToLCD(' ');
                    }
                    if(index == 12)
                    {
                        WriteCommandToLCD(0x8C);   //if index==12 then display ' '
                        WriteDataToLCD(' ');
                    }
                    if(index == 13)
                    {
                        WriteCommandToLCD(0x8D);   //if index==13 then display ' '
                        WriteDataToLCD(' ');
                    }
                    if(index == 14)
                    {
                        WriteCommandToLCD(0x8E);   //if index==14 then display ' '
                        WriteDataToLCD(' ');
                    }
                }
                update_SD7();//update 7 segment display

            }
            rb7flag = 0;//RB7 interrupt flag=0;if the user press RB7 before all pins not set up
            update_SD7();//update 7 segment display
        }
        if(rb7flag == 1 && pindex == 4)//if RB7 button is pressed and 4 bit pin is setting up
        {
            //pin[pindex] = convert(ADRES);
            index = 11;//clear index
            pindex = 0;//clear pindex
            rb7flag = 0;//clear RB7 flag
            break;
        }
    }
    clear7SD(); // clear 7 segment display
    update_SD7();//update 7 segment display

    ClearLCDScreen();
    sec3flag=6;//flag for 3 sec
    TMR0=61;//set TMR0 to its initial value
    toggle_500==0;//500ms blinking flag
    while(sec3flag)
    {
            update_SD7();//update 7 segment display
            if(toggle_500==1)//if 1 clearLCD
                ClearLCDScreen();

            else if(toggle_500 == 0)//else display setted pin
                pinsetLCD();

            update_SD7();//update 7 segment display


    }
    rb7flag = 0;//RB7flag=0
    ClearLCDScreen();
    oldvalue = convert(ADRES);//take potentiometer for oldvalue for checking # blinking

    enterLCD(enter,attempt);//display LED to string part of entering pin
    WriteCommandToLCD(0x8B);//display 4 #
    WriteStringToLCD("####");

    SD7[3]=0;
    TMR1ON = 1;            //tmr1 is on
    time=120;
    gettime();//set initial time(120) to 4 bit SD7 array
    while(1)// for taking 4 bit
    {
        if(endtime==1)//check for time over(120sec)
        {
            break;
        }
        gettime();//get current time 
        update_SD7();//update 7 segment display

        if(att==0)//attempt ==0 then wait 20 sec and restart if the time is not over 
        {
            oldtime=time;//save current time
            att=2;//set initial attempt value
            ClearLCDScreen();
            while(1)
            {
                ADIE=0;//disable ADCON interrupt
                RBIE=0;//disable RB change interrupt

                if(endtime==1)//if time is over break for restarting
                    break;

                if(time==oldtime-20)//check 20 sec passed
                {
                    WriteCommandToLCD(0x8B);//display 4 # and attempt number
                    WriteStringToLCD("####");
                    WriteCommandToLCD(0xC0);
                    WriteStringToLCD("  Attempts:");
                    WriteCommandToLCD(0xCB);
                    WriteDataToLCD(att+'0');
                    WriteCommandToLCD(0xCC);
                    WriteStringToLCD("    ");
                    ADIE=1;//enable ADCON interrupt
                    RBIE=1;//enable RB change interrupt
                    break;
                }
                gettime();//get current time
                update_SD7();//update 7 segment display

                wrong_pinLCD();//display wrong screen
            }

        }
        if(endtime==1)//check for time is over
                    break;

        if(pindex <= 3)//check for bound of pin index(4 bit)
        {
            if(oldvalue != convalue)//check for potentimeter changes if changes write to LED
            {

                if(index == 11)
                {
                    WriteCommandToLCD(0x8B);   //if index==11 then display converted ADC value
                    WriteDataToLCD(convalue+'0');
                }
                if(index == 12)
                {
                    WriteCommandToLCD(0x8C);   //if index==12 then display converted ADC value
                    WriteDataToLCD(convalue+'0');
                }
                if(index == 13)
                {
                    WriteCommandToLCD(0x8D);   //if index==13 then display converted ADC value
                    WriteDataToLCD(convalue+'0');
                }
                if(index == 14)
                {
                    WriteCommandToLCD(0x8E);   //if index==14 then display converted ADC value
                    WriteDataToLCD(convalue+'0');
                }

                if(rb6flag == 0)//check for RB6 button interrupt  if RB6 is not pushed
                {
                    rb6_flag_2 = 1;
                    oldvalue = -1;//oldvalue = -1 then # charac. is not blinking #
                }
                else//if RB6 is pushed then
                {
                    oldvalue = convalue;// if RB6 is pushed then oldvalue = convalue then after index changes # charachter blinking
                    rb6flag = 0;
                    rb6_flag_2 = 0;
                }
            }
            else
            {
                if(pins == 1)//check for blinking 
                {

                    if(index == 11)
                    {
                        WriteCommandToLCD(0x8B);   //if index==11 then display '#'
                        WriteDataToLCD('#');
                    }
                    if(index == 12)
                    {
                        WriteCommandToLCD(0x8C);   //if index==12 then display '#'
                        WriteDataToLCD('#');
                    }
                    if(index == 13)
                    {
                        WriteCommandToLCD(0x8D);   //if index==13 then display '#'
                        WriteDataToLCD('#');
                    }
                    if(index == 14)
                    {
                        WriteCommandToLCD(0x8E);   //if index==14 then display '#'
                        WriteDataToLCD('#');
                    }

                //enter[index] = '#';
                    enterLCD(enter,attempt);
                }
                else if(pins == 0)
                {
                    if(index == 11)
                    {
                        WriteCommandToLCD(0x8B);  //if index==11 then display ' '
                        WriteDataToLCD(' ');
                    }
                    if(index == 12)
                    {
                        WriteCommandToLCD(0x8C);   //if index==12 then display ' '
                        WriteDataToLCD(' ');
                    }
                    if(index == 13)
                    {
                        WriteCommandToLCD(0x8D);   //if index==13 then display ' '
                        WriteDataToLCD(' ');
                    }
                    if(index == 14)
                    {
                        WriteCommandToLCD(0x8E);   //if index==14 then display ' '
                        WriteDataToLCD(' ');
                    }
                    //enter[index] = ' ';
                    enterLCD(enter,attempt);
                }
            }
            rb7flag = 0;
        }

        if(rb7flag == 1 && pindex == 4)//if RB7 is pressed and 4 bit pin entered
        {
            index = 11;//clear index
            pindex = 0;//clear pindex
            rb7flag = 0;//clear RB7 interrupt flag


            if(pin[0]==enterpin[0] && pin[1]==enterpin[1] && pin[2]==enterpin[2] && pin[3]==enterpin[3])//if entered pin and setted pin is equal
            {
                GIE=0;//disable global interrupt
                finish=1;//finish flag == 1
                finishLCD();//finish screen is displayed
                break;
            }
            else//if it is not equal
            {
                att--;//attempt--
                WriteCommandToLCD(0x8B);//write 4  '#'
                WriteStringToLCD("####");
            }
        }

    }
    while(endtime==0)//if time is over
    {
         update_SD7();//update 7segment display
      //  finishLCD();
    }
    endtime=0;


    }
}


void beginLCD() { //It is used beginning of the program

    WriteCommandToLCD(0x80);   // Goto to the beginning of the first line
    WriteStringToLCD(" $>Very  Safe<$ "); 

    WriteCommandToLCD(0xC0);    // Goto to the beginning of the second line
    WriteStringToLCD(" $$$$$$$$$$$$$$ ");
    //__delay_ms(2);
}

void finishLCD() { //It is used end of the program
    update_SD7();

    WriteCommandToLCD(0x80);   // Goto to the beginning of the first line
    WriteStringToLCD("Safe is opening!");   

    WriteCommandToLCD(0xC0);    // Goto to the beginning of the second line
    WriteStringToLCD("$$$$$$$$$$$$$$$$");
    //__delay_ms(1);
    update_SD7();

}

void setLCD(char *fset) {   //It is used while the user setting the PIN

    WriteCommandToLCD(0x80);   // Goto to the beginning of the first line
    WriteStringToLCD(fset);    

    WriteCommandToLCD(0xC0);  // Goto to the beginning of the second line
    WriteStringToLCD("                ");

    //__delay_ms(2);
}

void enterLCD(char *enter,char* attempt)//It is used while the user entering the PIN
{


    WriteCommandToLCD(0x80);   //Goto to the beginning of the first line
    WriteStringToLCD(enter);   


    WriteCommandToLCD(0xC0);    //Goto to the beginning of the second line
    WriteStringToLCD("  Attempts:");
    WriteCommandToLCD(0xCB);
    WriteDataToLCD(att+'0');
    WriteCommandToLCD(0xCC);
    WriteStringToLCD("    ");


   // __delay_ms(2);
}

void pinsetLCD() {  //It is used for writing  The new pin is  and -pin-
    WriteCommandToLCD(0x80);   // Goto to the beginning of the first line
    WriteStringToLCD(" The new pin is ");    // Write "The new pin is" on LCD

//"   ---    ---   ";
    WriteCommandToLCD(0xC3);
    WriteStringToLCD("---");
    WriteCommandToLCD(0xC6);
    WriteDataToLCD(pin[0]+'0');
    WriteCommandToLCD(0xC7);
    WriteDataToLCD(pin[1]+'0');
    WriteCommandToLCD(0xC8);
    WriteDataToLCD(pin[2]+'0');
    WriteCommandToLCD(0xC9);
    WriteDataToLCD(pin[3]+'0');
    WriteCommandToLCD(0xCA);
    WriteStringToLCD("---");
   // __delay_ms(2);
}

void wrong_pinLCD()//it is used when the user enter wrong pin 2 times
{
    WriteCommandToLCD(0x80);        // Goto to the beginning of the first line
    WriteStringToLCD(" Enter pin:XXXX ");    // Write " Enter pin:XXXX " on LCD
    WriteCommandToLCD(0xC0);   
    WriteStringToLCD("Try after ");    // Write "Try after" on LCD
    WriteCommandToLCD(0xCA);
    WriteDataToLCD(2+'0');              // Write '2' on LCD(from 20 sec)
    WriteCommandToLCD(0xCB);
    WriteDataToLCD(0+'0');              // Write '0' on LCD(from 20 sec)

    WriteCommandToLCD(0xCC);
    WriteStringToLCD(" sec.");          // Write "sec." on LCD(from 20 sec)

}

void pinleribasLCD()/*We used this funct. for debugging.*/
{
    WriteCommandToLCD(0x80);
    WriteStringToLCD("PIN=");
    WriteCommandToLCD(0x83);
    WriteDataToLCD(pin[0]+'0');
    WriteCommandToLCD(0x84);
    WriteDataToLCD(pin[1]+'0');
    WriteCommandToLCD(0x85);
    WriteDataToLCD(pin[2]+'0');
    WriteCommandToLCD(0x86);
    WriteDataToLCD(pin[3]+'0');

    WriteCommandToLCD(0xC0);
    WriteStringToLCD("ent=");
    WriteCommandToLCD(0xC3);
    WriteDataToLCD(enterpin[0]+'0');
    WriteCommandToLCD(0xC4);
    WriteDataToLCD(enterpin[1]+'0');
    WriteCommandToLCD(0xC5);
    WriteDataToLCD(enterpin[2]+'0');
    WriteCommandToLCD(0xC6);
    WriteDataToLCD(enterpin[3]+'0');
}

int  convert(unsigned int value)/*It is used for converting of ADC Values to numbers.*/
{
    if(0<= value && value<= 99)
        return 0;
    if(100<= value && value<= 199)
        return 1;
    if(200<= value && value<= 299)
        return 2;
    if(300<= value && value<= 399)
        return 3;
    if(400<= value && value<= 499)
        return 4;
    if(500<= value && value<= 599)
        return 5;
    if(600<= value && value<= 699)
        return 6;
    if(700<= value && value<= 799)
        return 7;
    if(800<= value && value<= 899)
        return 8;
    if(900<= value && value<= 1024)
        return 9;
}

void clear7SD()//set all segments to dash character
{
    for(int i=0;i<4;i++)
        SD7[i]='-';
}

unsigned char getDisp(unsigned char c)/*it is used for displaying elements of SD7 */
{
    switch(c)
    {
        case 0: return 0b00111111;  // 7-Segment = 0
        case 1: return 0b00000110;  // 7-Segment = 1
        case 2: return 0b01011011;  // 7-Segment = 2
        case 3: return 0b01001111;  // 7-Segment = 3
        case 4: return 0b01100110;  // 7-Segment = 4
        case 5: return 0b01101101;  // 7-Segment = 5
        case 6: return 0b01111101;  // 7-Segment = 6
        case 7: return 0b00000111;  // 7-Segment = 7
        case 8: return 0b01111111;  // 7-Segment = 8
        case 9: return 0b01100111;  // 7-Segment = 9
        case '-': return 1<<6;      // only the dash segment

    }
    return 0;   // all segments off
}

void update_SD7()//update every character of 7segment
{
    iter=1;//iter is used for iterating over SD7 array
    for(int i=0;i<4;i++)
    {
        PORTJ = 0;  //clear PORTJ
        PORTH |= (iter&0x0f); //then select appropriate bit in PORTH
        PORTJ = getDisp(SD7[3-i]);    //set portj to appropriate representation
        __delay_us(500);            //wait for 500microsec
        PORTH &= 0xf0;  //clear porth
        iter <<= 1;   // go to next bit
    }
}

void gettime()/*It changes the SD7 with respect to current time*/
{
    if(time<100)
    {
        SD7[0] = time%10; //right most bit according to base 10(birler basamagı)
        SD7[1] = time/10; //second right most bit according to base 10(onlar)
        SD7[2] = 0;       //second left most bit according to base 10(yuzler)
    }
    else if(time>=100 )
    {
        SD7[0] = time%10;
        SD7[1] = (time-100)/10;
        SD7[2] = 1;
    }
}