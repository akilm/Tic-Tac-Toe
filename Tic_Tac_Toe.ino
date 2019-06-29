/* Created On May 20,2018 by Akil M*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int a=0,b=0;                                     // a and b are used to represent x and y coordinates respectively
int p1[3][3],p2[3][3];                           // p1 and p2 are to keep track of the leds of the respective players
int T[3][3];                                     // T is used to keep track of the total number of leds used p1 and p2 combined
void init_players()
{
    for(int i=0; i<3; i++)                       //to initialize all values as zero. 1 will be written in p1 or p2 depending on the player and 1 will be written in T also to keep track of the leds used
    {
        for(int j=0; j<3; j++)
        {
            p1[i][j]=0;
            p2[i][j]=0;
            T[i][j]=0;
        }
    }
}

void newpos()                            //to determine the new position after a player's turn . this loop aims to find the lowest unused coordinate (not selected by X or O)
{
    for(int i=3; i>=0; i--)
        for(int j=3; j>=0; j--)
        {
            if(T[i][j]==0)
            {
                a=i;
                b=j;
            }
        }
}


void glow(int c,int d)                                  //used to turn on the led when it is player one's turn 
{                                                       // the pins corresponding to each led is given in the description
    if(d==0)
    {
        if(c==0)
            PORTB|=(1<<0);
        else if(c==1)
            PORTB|=(1<<1);
        else if(c==2)
            PORTB|=(1<<2);
    }
    else if(d==1)
    {
        if(c==0)
            PORTB|=(1<<3);
        else if(c==1)
            PORTB|=(1<<4);
        else if(c==2)
            PORTB|=(1<<5);
    }
    else if(d==2)
    {
        if(c==0)
            PORTC|=(1<<0);
        else if(c==1)
            PORTC|=(1<<1);
        else if(c==2)
            PORTC|=(1<<2);
    }

}

void turnoff(int c,int d)          // used to turn off the previously glowing led , when player one switches to another led
{
    if(d==0)
    {
        if(c==0)
            PORTB&=~(1<<0);
        else if(c==1)
            PORTB&=~(1<<1);
        else if(c==2)
            PORTB&=~(1<<2);
    }
    if(d==1)
    {
        if(c==0)
            PORTB&=~(1<<3);
        else if(c==1)
            PORTB&=~(1<<4);
        else if(c==2)
            PORTB&=~(1<<5);
    }
    if(d==2)
    {
        if(c==0)
            PORTC&=~(1<<0);
        else if(c==1)
            PORTC&=~(1<<1);
        else if(c==2)
            PORTC&=~(1<<2);
    }
}


void timer1_init()
{
    TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10);         // set up timer with prescaler = 64 and CTC mode
    TCNT1 = 0;                                              // initialize counter
    OCR1A = 50000;                                          // initialize compare value
    TIMSK1 |= (1 << OCIE1A);                                // enable compare interrupt
    sei();                                                  // enable global interrupts
}
ISR (TIMER1_COMPA_vect)                              //this interrupt service routine is called regularly  when the timer reaches the value 50000
{

    for(int i=0; i<3; i++)                           //used to toggle all the leds which are selected by player p2.
    {
        for(int j=0; j<3; j++)
        {
            if(p2[i][j]==1)
            {
                if(j==0)
                {
                    if(i==0)
                        PORTB^=(1<<0);
                    else if(i==1)
                        PORTB^=(1<<1);
                    else if(i==2)
                        PORTB^=(1<<2);
                }
                if(j==1)
                {
                    if(i==0)
                        PORTB^=(1<<3);
                    else if(i==1)
                        PORTB^=(1<<4);
                    else if(i==2)
                        PORTB^=(1<<5);
                }
                if(j==2)
                {
                    if(i==0)
                        PORTC^=(1<<0);
                    else if(i==1)
                        PORTC^=(1<<1);
                    else if(i==2)
                        PORTC^=(1<<2);
                }

            }
        }
    }

}

void winner_1()                            //called when player 1 wins
{
    for(int i=0; i<3; i++)                  //setting the values of all the elements of p2 to zero so that no led will blink
        for(int j=0; j<3; j++)
        {
            p2[i][j]=0;
        }
    for(int i=0; i<3; i++)                 //glowing the remaining leds which were either blinking or in the off state
        for(int j=0; j<3; j++)
        {
            if(p1[i][j]==0)
            {
                p1[i][j]=1;
                glow(i,j);

            }


        }
}
void winner_2()                             //called when player 2 wins
{
    for(int i=0; i<3; i++)                  //turning off the leds which belonged to player 1
        for(int j=0; j<3; j++)
        {
            if(p1[i][j]==1)
                turnoff(i,j);
        }
    for(int i=0; i<3; i++)                  //making all the values of p2 as 1 so that all the leds continuosly blink
        for(int j=0; j<3; j++)
        {
            if(p2[i][j]==0)
            {
                p2[i][j]=1;
            }
        }
}
int winner1check()                          //checking whether player 1 has won or not
{                                           //all the 8 possible conditions are checked
    if(p1[0][0]==1)
    {
        if(( p1[1][0]==1 )&& (p1[2][0]==1))
        {
            winner_1();
            return 1;
        }
        else if(( p1[0][1]==1 )&& (p1[0][2]==1))
        {
            winner_1();
            return 1;
        }
        else if( ( p1[1][1]==1 )&& (p1[2][2]==1))
        {
            winner_1();
            return 1;
        }
    }
    if(p1[1][0]==1)
    {
        if((p1[1][1]==1)&&(p1[1][2]==1))
        {
            winner_1();
            return 1;
        }
    }
    if(p1[2][0]==1)
    {
        if( ( p1[2][1]==1 )&& (p1[2][2]==1))
        {
            winner_1();
            return 1;
        }
        if( ( p1[1][1]==1 )&& (p1[0][2]==1))
        {
            winner_1();
            return 1;
        }
    }
    if(p1[2][1]==1)
    {
        if( ( p1[1][1]==1 )&& (p1[0][1]==1))
        {
            winner_1();
            return 1;
        }
    }
    if(p1[2][2]==1)
    {
        if( ( p1[1][2]==1 )&& (p1[0][2]==1))
        {
            winner_1();
            return 1;
        }
    }
    return 0;
}


int winner2check()                                   //checking whether player 2 has won or not
{                                                    //all 8 possibilities are considered
    if(p2[0][0]==1)
    {
        if(( p2[1][0]==1 )&& (p2[2][0]==1))
        {
            winner_2();
            return 2;
        }
        else if(( p2[0][1]==1 )&& (p2[0][2]==1))
        {
            winner_2();
            return 2;
        }
        else if( ( p2[1][1]==1 )&& (p2[2][2]==1) )
        {
            winner_2();
            return 2;
        }
    }
    if(p2[1][0]==1)
    {
        if((p2[1][1]==1)&&(p2[1][2]==1))
        {
            winner_2();
            return 2;
        }
    }
    if(p2[2][0]==1)
    {
        if( ( p2[2][1]==1 )&& (p2[2][2]==1))
        {
            winner_2();
            return 2;
        }
        if( ( p2[1][1]==1 )&& (p2[0][2]==1))
        {
            winner_2();
            return 2;
        }
    }
    if(p2[2][1]==1)
    {
        if( ( p2[1][1]==1 )&& (p2[0][1]==1))
        {
            winner_2();
            return 2;
        }
    }
    if(p2[2][2]==1)
    {
        if( ( p2[1][2]==1 )&& (p2[0][2]==1))
        {
            winner_2();
            return 2;
        }
    }
    return 0;
}
void selection1()
{
    int x=a;
    int y=b;
    glow(x,y);
    while((PIND&(1<<2))==0)                 //initally the button is in off position , checking whether the led i
    {
        if(PIND&(1<<3))                     //if x traverse button is pressed then the x coordinate is changed
        {
            if(x==0)
            {
                if(T[1][y]==0)              //checking whether the next coordinate is unused
                {
                    turnoff(x,y);
                    x=1;
                    glow(x,y);
                    _delay_ms(200);
                }
                else if(T[2][y]==0)         //checking the next available possibility
                {
                    turnoff(x,y);
                    x=2;
                    glow(x,y);
                    _delay_ms(200);

                }
            }
            else
            if(x==1)
            {
                if(T[2][y]==0)
                {
                    turnoff(x,y);
                    x=2;
                    glow(x,y);
                    _delay_ms(200);
                }
                else if(T[0][y]==0)
                {
                    turnoff(x,y);
                    x=0;
                    glow(x,y);
                    _delay_ms(200);
                }
            }
            else
            if(x==2)
            {
                if(T[0][y]==0)
                {
                    turnoff(x,y);
                    x=0;
                    glow(x,y);
                    _delay_ms(200);
                }
                else if(T[1][y]==0)
                {
                    turnoff(x,y);
                    x=1;
                    glow(x,y);
                    _delay_ms(200);

                }
            }

        }
        else
        if(PIND&(1<<4))                      //if y traverse button is pressed then the x coordinate is changed
        {
            if(y==0)
            {
                if(T[x][1]==0)
                {
                    turnoff(x,y);
                    y=1;
                    glow(x,y);
                    _delay_ms(200);
                }
                else if(T[x][2]==0)
                {
                    turnoff(x,y);
                    y=2;
                    glow(x,y);
                    _delay_ms(200);

                }
            }
            else
            if(y==1)
            {
                if(T[x][2]==0)
                {
                    turnoff(x,y);
                    y=2;
                    glow(x,y);
                    _delay_ms(200);
                }
                else if(T[x][0]==0)
                {
                    turnoff(x,y);
                    y=0;
                    glow(x,y);
                    _delay_ms(200);
                }
            }
            else
            if(y==2)
            {
                if(T[x][0]==0)
                {
                    turnoff(x,y);
                    y=0;
                    glow(x,y);
                    _delay_ms(200);
                }
                else if(T[x][1]==0)
                {
                    turnoff(x,y);
                    y=1;
                    glow(x,y);
                    _delay_ms(200);
                }

            }
        }


    }
    _delay_ms(1000);
    T[x][y]=1;
    p1[x][y]=1;

}
void selection2()
{
    int x=a;
    int y=b;
    p2[x][y]=1;
    while((PIND&(1<<2))==0)                 //initally the button is in off position
    {
        if(PIND&(1<<3))                       //same as the selection 1  process and instead of glowing the index numbers of the led p2[x][y] is set to one and the blinking is taken care of by the interrupt service routine
        {
            if(x==0)
            {
                if(T[1][y]==0)
                {
                    p2[x][y]=0;
                    turnoff(x,y);
                    x=1;
                    p2[x][y]=1;
                    _delay_ms(200);
                }
                else if(T[2][y]==0)
                {
                    p2[x][y]=0;
                    turnoff(x,y);
                    x=2;
                    p2[x][y]=1;
                    _delay_ms(200);

                }
            }
            else if(x==1)
            {
                if(T[2][y]==0)
                {
                    p2[x][y]=0;
                    turnoff(x,y);
                    x=2;
                    p2[x][y]=1;
                    _delay_ms(200);
                }
                else if(T[0][y]==0)
                {
                    p2[x][y]=0;
                    turnoff(x,y);
                    x=0;
                    p2[x][y]=1;
                    _delay_ms(200);
                }
            }
            else if(x==2)
            {
                if(T[0][y]==0)
                {
                    p2[x][y]=0;
                    turnoff(x,y);
                    x=0;
                    p2[x][y]=1;
                    _delay_ms(200);
                }
                else if(T[1][y]==0)
                {
                    p2[x][y]=0;
                    turnoff(x,y);
                    x=1;
                    p2[x][y]=1;
                    _delay_ms(200);

                }
            }

        }

        if(PIND&(1<<4))
        {
            if(y==0)
            {
                if(T[x][1]==0)
                {
                    p2[x][y]=0;
                    turnoff(x,y);
                    y=1;
                    p2[x][y]=1;
                    _delay_ms(200);
                }
                else if(T[x][2]==0)
                {
                    p2[x][y]=0;
                    turnoff(x,y);
                    y=2;
                    p2[x][y]=1;
                    _delay_ms(200);

                }
            }
            else if(y==1)
            {
                if(T[x][2]==0)
                {
                    p2[x][y]=0;
                    turnoff(x,y);
                    y=2;
                    p2[x][y]=1;
                    _delay_ms(200);
                }
                else if(T[x][0]==0)
                {
                    p2[x][y]=0;
                    turnoff(x,y);
                    y=0;
                    p2[x][y]=1;
                    _delay_ms(200);
                }
            }
            else
            if(y==2)
            {
                if(T[x][0]==0)
                {
                    p2[x][y]=0;
                    turnoff(x,y);
                    y=0;
                    p2[x][y]=1;
                    _delay_ms(200);
                }
                else if(T[x][1]==0)
                {
                    p2[x][y]=0;
                    turnoff(x,y);
                    y=1;
                    p2[x][y]=1;
                    _delay_ms(200);
                }

            }
        }
    }
    _delay_ms(1000);
    T[x][y]=1;

}
int main(void)
{
    DDRB = 0b00111111;   //pins PB0 PB1 PB2 PB3 PB4 PB5 PC0 PC1 PC2 are connected to leds
    DDRC = 0b00000111;
    DDRD = 0b11100000;   //pins PB2 PB3 PB4 connected to buttons
    PORTB=0x00;          //turnoff all leds initially
    PORTC=0x00;
    PORTD |=(1<<5)|(1<<6)|(1<<7);  //current source for the buttons
    init_players();
    timer1_init();
    int winner=0;
    while(1)
    {
        if(winner==0)
        {
            newpos();
            selection1();
            winner=winner1check();
        }
        if(winner==0)
        {
            newpos();
            selection2();
            winner=winner2check();
        }
    }
    return 0;
}
