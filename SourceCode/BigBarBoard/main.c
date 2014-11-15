#include "p18f4550.h"

//#pragma config WDT=OFF, LVP=OFF, DEBUG=OFF, MCLRE = ON
 
//Internal oscillator, port function on RA6, EC used by USB 
//#pragma config FOSC = INTOSCIO_EC //1MHz oscillation



#include <spi.h>
#include <adc.h> 

#define INDICATOR PORTCbits.RC1



	 
 #define  GIVEBYTE(data)               \
	        WriteSPI(data);

	
unsigned char Brightness = 10;
unsigned char OrangeBrightness = 10;
float Sensitivity;
unsigned char Zero_Adj;
unsigned int  Hysto;




unsigned int safesub(unsigned int a, unsigned int b){
if(b>a)
  return 0;
return a-b;	
}

unsigned char SafeByte(unsigned int input){
   if(input > 255)
      return 255;
      return input;	
	
}
	
	


unsigned int channel[7] = {0,20,40,60,80,100,120};
unsigned char Hchannel[9] = {5,5,5,5,5,5,5,5,5};

unsigned char MaxIndex;
unsigned char MaxValue;
#define  RIGHT_ADC ADC_CH0
#define  LEFT_ADC  ADC_CH1

#define RESET PORTCbits.RC0

#define STROBE PORTCbits.RC2

unsigned int loadADCchannel(unsigned char channel){
	   SetChanADC (channel);
   	   Delay10TCYx(60);
       ConvertADC ();	// Start an A/D conversion				
       while(BusyADC()){	// Wait for the conversion to complete
       //maybe some code for while were waiting (could turn an led on maybe)
        }
        Delay10TCYx(60);
        return  ReadADC();
}
	

void LoadVolume(){
   unsigned char i,j;
   unsigned int temp;
   RESET = 1;
    Delay10TCYx(10);
   STROBE=1;
   Delay10TCYx(10);
   STROBE=0;
   Delay10TCYx(10); 
   
   RESET = 0;



 //STROBE=1;
  // Delay10TCYx(100); 
   
  
  
   for(i=0; i< 7; i++){
	  	STROBE=1;
   		Delay10TCYx(10);
        STROBE=0;
        Delay10TCYx(10); 
	    

        temp = safesub(((unsigned int)loadADCchannel(LEFT_ADC)*(unsigned int)Sensitivity)/(unsigned int)140,Zero_Adj);// + loadADCchannel(LEFT))/8;
  
  	
  if(Hysto > 0)
  	channel[i] = ((float)channel[i]*(float)Hysto + (float)temp * (float)(255-Hysto))/(float)255;
  else
  channel[i] = temp;
   	if(channel[i] > 255)
   		channel[i] = 255;
   }     
   
 //  channel[i] = channel[i] * (float)sqrt(i+1);
   
   
   Hchannel[0]  = channel[0];
   Hchannel[1]  = (channel[0]+channel[1])>>1;
   Hchannel[2]  = channel[1];
   Hchannel[3]  = (channel[1]+channel[2])>>1;
   Hchannel[4]  = channel[2];
   Hchannel[5]  = (channel[2]+channel[3])>>1;
   Hchannel[6]  = channel[3];
   Hchannel[7] = (channel[3]+channel[4])>>1;;
   Hchannel[8] = channel[4];

   
}


	



	




	
	

		





#define BRIGHTNESS  ADC_CH3
#define SENSITIVITY ADC_CH4
#define ZERO_ADJ    ADC_CH2
#define HYSTO       ADC_CH5


//green knob = brightness
//bluwe knob = sensitivity
//red knob = ZERO_ADJ
//white = hysto
void LoadOptions(){
	unsigned int temp;

	temp = loadADCchannel(BRIGHTNESS)/3;
	if(temp %2)
	 temp--;
	 if(temp > 254)
	 	temp = 254;
	Brightness = temp;
	OrangeBrightness = temp >>1;
	if(OrangeBrightness%2)
		OrangeBrightness --;
		
	
//	Hchannel[0] = temp;
	
	Sensitivity =  (loadADCchannel(SENSITIVITY)>>2);
    Zero_Adj =  loadADCchannel(ZERO_ADJ)>>4;
	Hysto =  (loadADCchannel(HYSTO)>>2);
			
	//Sensitivity = loadADCchannel(BRIGHTNESS) / 10;
	//Zero_Adj    = loadADCchannel(ZERO_ADJ)>>6;
//	Hysto       = loadADCchannel(HYSTO);
}
	

rom unsigned char coswave[400] = {
35, //0
35, //1
35, //2
35, //3
35, //4
35, //5
35, //6
35, //7
35, //8
35, //9
35, //10
35, //11
35, //12
35, //13
34, //14
34, //15
34, //16
34, //17
34, //18
34, //19
33, //20
33, //21
33, //22
33, //23
32, //24
32, //25
32, //26
32, //27
31, //28
31, //29
31, //30
31, //31
30, //32
30, //33
30, //34
29, //35
29, //36
29, //37
28, //38
28, //39
28, //40
27, //41
27, //42
26, //43
26, //44
26, //45
25, //46
25, //47
24, //48
24, //49
24, //50
23, //51
23, //52
22, //53
22, //54
21, //55
21, //56
21, //57
20, //58
20, //59
19, //60
19, //61
18, //62
18, //63
18, //64
17, //65
17, //66
16, //67
16, //68
15, //69
15, //70
14, //71
14, //72
14, //73
13, //74
13, //75
12, //76
12, //77
11, //78
11, //79
11, //80
10, //81
10, //82
9, //83
9, //84
9, //85
8, //86
8, //87
7, //88
7, //89
7, //90
6, //91
6, //92
6, //93
5, //94
5, //95
5, //96
4, //97
4, //98
4, //99
4, //100
3, //101
3, //102
3, //103
3, //104
2, //105
2, //106
2, //107
2, //108
1, //109
1, //110
1, //111
1, //112
1, //113
1, //114
0, //115
0, //116
0, //117
0, //118
0, //119
0, //120
0, //121
0, //122
0, //123
0, //124
0, //125
0, //126
0, //127
0, //128
0, //129
0, //130
0, //131
0, //132
0, //133
0, //134
0, //135
0, //136
0, //137
0, //138
0, //139
0, //140
0, //141
1, //142
1, //143
1, //144
1, //145
1, //146
1, //147
2, //148
2, //149
2, //150
2, //151
3, //152
3, //153
3, //154
3, //155
4, //156
4, //157
4, //158
4, //159
5, //160
5, //161
5, //162
6, //163
6, //164
6, //165
7, //166
7, //167
7, //168
8, //169
8, //170
9, //171
9, //172
9, //173
10, //174
10, //175
11, //176
11, //177
11, //178
12, //179
12, //180
13, //181
13, //182
14, //183
14, //184
14, //185
15, //186
15, //187
16, //188
16, //189
17, //190
17, //191
17, //192
18, //193
18, //194
19, //195
19, //196
20, //197
20, //198
21, //199
21, //200
21, //201
22, //202
22, //203
23, //204
23, //205
24, //206
24, //207
24, //208
25, //209
25, //210
26, //211
26, //212
26, //213
27, //214
27, //215
28, //216
28, //217
28, //218
29, //219
29, //220
29, //221
30, //222
30, //223
30, //224
31, //225
31, //226
31, //227
31, //228
32, //229
32, //230
32, //231
32, //232
33, //233
33, //234
33, //235
33, //236
34, //237
34, //238
34, //239
34, //240
34, //241
34, //242
35, //243
35, //244
35, //245
35, //246
35, //247
35, //248
35, //249
35, //250
35, //251
35, //252
35, //253
35, //254
35, //255
35, //256
};


 
	
unsigned char matrix[9][36];

	
void main(void){

    unsigned char coloumn, row,band,i,j,temp,counter,y;
    
    unsigned int a,b;
    

    
    

    
    TRISE = 0;
   	TRISD = 0;
	TRISC = 0;
	TRISB = 0b00000000;
    TRISA = 0b00111111;
    PORTD = 0;
	PORTC = 0;
	PORTB = 0b00000000;
	PORTA = 0b00111111;
	PORTE = 0;
	

	
	
   ADCON1= 0b00001001;
   UCON &= 0b11111011;
   UCFG =  0b00000100;


  
  	OSCTUNE = 0b00001111; 
	OSCCON  = 0b01110010;

	
	
	 OpenADC( ADC_FOSC_64 & // A/D clock source set to 32Tosc
   ADC_RIGHT_JUST& // write the Digital result(10bits) from right, into ADRESH:ADRESL(16bits).
   ADC_20_TAD, // A/D acquisition time: 20TAD (for 10bit conversion at least12TAD)
   ADC_CH0 & //choose input channel (11), AN11
   ADC_INT_OFF& //ADC interrupt off
   ADC_VREFPLUS_VDD& // choose the supply voltage VDD as reference voltage, V+
   ADC_VREFMINUS_VSS, // choose the supply voltage VSS as reference voltage, V-
    ADC_6ANA  // an0 input, all others digital
);
  
 
 
 
	while(1){
		
	counter+=10;
	if(counter >100)
		counter  = 0;
	
		OpenSPI(SPI_FOSC_4, MODE_01, SMPEND); //initialize SPI
		
		for(j=0; j< 9; j++){
			for(i=0; i< 36; i++){
				if(i< 18)
					y = 18-i;
				 else
				 	y = 54-i;
				 	
				 	//yellow = 1-14
				 	//orange = 15-22
				 	//red = 23-
				//GIVEBYTE( y == 36 ? 254 : 0);
				
				if(Hchannel[j] > y){
					if(y >= 15 && y<= 22){
						GIVEBYTE(OrangeBrightness);//orange LEDs are brighter than red and yellow, so sim in code.
					} else {	
						GIVEBYTE(Brightness);
					}	
				}	else {
					GIVEBYTE(0)
				}
				
//				GIVEBYTE(Hchannel[j] > y ? 254) : 0);
		 	}
		 
		 	
		}
		
		CloseSPI();
		
	
	//	INDICATOR=1;
		LoadOptions();
    	LoadVolume();
    
       
  //     for(i=0; i< 1; i++)
   //   	 Delay100TCYx(20);
      	
    //  	INDICATOR=0;
      	
      //	     for(i=0; i< 1; i++)
      //	 Delay100TCYx(20);
      	 
	}

}