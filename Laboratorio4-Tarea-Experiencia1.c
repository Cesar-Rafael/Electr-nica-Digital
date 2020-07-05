int DEBUG = 1;//Serial monitor encendido

//VARIABLES PARA EL BOTON:
int state_s1 = 0; //Estado actual de la maquina de estado
int state_prev_s1 = 0; //Estado previo
byte val_B_s1; //Defino una variable tipo byte para leer el botón del puertoD
unsigned long t_s1 = 0; //Tiempo actual 
unsigned long t_0_s1 = 0; //Tienpo inicial relativa
unsigned long bounce_delay_s1 = 7; //delay de 7ms
unsigned long t_2_boton = 1000; //Si el tiempo es mayor a 1s entonces se prenderá un segundo led

//VARIABLES PARA EL LED1:
int state_led1 = 0; //Estado actual del led
int state_prev_led1 = 0; //Estado previo
unsigned long t_led1 = 0; //Tiempo actual
unsigned long t_0_led1 = 0;	//Tiempo inicial relativo
//Como el periodo es de 2 segundos, y me piden un duty cicle de 50%
//El 50% de 2s es 1s, expresado en milisegundos sería 1000.
unsigned long on_delay_led1 = 1000;	//Tiempo encendido
unsigned long off_delay_led1 = 1000; //Tiempo apagado
int beep_count_led1 = 0; //Veces iniciales
int beep_numer_led1 = 15; //Veces maximas de blink

//VARIABLES PARA EL LED2:
int state_led2 = 0; //Estado actual del led
int state_prev_led2 = 0; //Estado previo
unsigned long t_led2 = 0; //Tiempo actual
unsigned long t_0_led2 = 0;	//Tiempo inicial relativo
//Como el periodo es de 2 segundos, y me piden un duty cicle de 75%
//El 75% de 2s es 1.5s, expresado en milisegundos sería 1500.
unsigned long on_delay_led2 = 1500;	//Tiempo encendido
unsigned long off_delay_led2 = 500; //Tiempo apagado
int beep_count_led2 = 0; //Veces iniciales
int beep_numer_led2 = 2; //Veces maximas de blink


void setup(){
    //En este caso, usaremos DDR para ver qué pines son entradas o salidas
    //Para las pines 0 y 1, les dejó el valor de 1 porque leí que se ponía así siempre :v.
    DDRD = B00000011;
    //Para esta configuración el pin 3 es una entrada.
    //Cabe resaltar que mi botón ya está armado como PULL UP en el circuito.

    DDRB = B00001000;
    //Para esta configuración el pin 11 es una salida.

    DDRC = B00000001;
    //Para esta configuración el pin A0 es salida
  
  	if (DEBUG){
        //Serial.begin(115200); //Velocidad 115200 baudics
        Serial.println("Debugging is ON");
	}
}

void loop(){
  //Llamo a cada función de estados:
  SM_s1();
  SM_led1();
  SM_led2();
  
  /*if (DEBUG){
    if (state_prev_s1 != state_s1 || state_prev_led1 != state_led1){
    	Serial.print("Switch State: ");Serial.println(state_s1);
    	Serial.print("LED State: ");Serial.println(state_led1);
    }
  }*/
}

//Estados del botón:
void SM_s1(){
	val_B_s1 = PIND; //Guarda en un byte
	state_prev_s1 = state_s1;
    switch(state_s1){
        case 0: //RESET
            state_s1 = 1; 
            break;

        case 1: //ESPERO HASTA QUE HAYA UN CAMBIO A LOW DEL BOTON
            val_B_s1 = PIND;
            //Ya que estamos con un botón Pull_up, el pin 2 tendrá valor de 1
            //Y queremos ver cuando está en low, o esa cuando sea 0, por tanto: 
            if( val_B_s1 == B00000000) {
              	state_s1 = 2;
            }//VA AL ESTADO 2 CUANDO IDENTIFICA LOW
            break;

        case 2: //INICIA EL CONTEO
            t_0_s1 = millis();
            state_s1 = 3;
            break;

        case 3:	//CUENTA Y VA AL ESTADO 4 SI LLLEGA AL TIEMPO MINIMO 
          //O VA AL ESTADO DE RESET SI APARECE UN HIGH 
            t_s1 = millis();
            val_B_s1 = PIND;
        	if( t_s1 - t_0_s1 > bounce_delay_s1 ){ 
          		state_s1 = 4;
            }
            if(val_B_s1 == B00001000){ 
              	state_s1 = 0;
            }//Si es HIGH regreso al estado 0
            break;

        case 4: //SI PASAS LOS 7ms LLEGAS AQUÍ
      		//Como ya pasó el tiempo solicitado, cambio de estado al led1
      		state_led1 = 2;
            t_s1 = millis();
            val_B_s1 = PIND;
			//SI MANTIENE PRESIONADO EL BOTÓN POR MÁS DE 1 SEGUNDO, PASA A OTRO ESTADO
            if( t_s1 - t_0_s1 >= t_2_boton) state_s1 = 5;
      		//EN CASO NO HAYA SIDO PULSADO DURANTE MÁS DE UN SEGUNDO Y VOLVIÓ A HIGH ENTONCES PASA AL ESTADO 6
      		else if(val_B_s1 == B00001000) state_s1 = 6;
            break;

        case 5: //ALCANZA UNA PULSACIÓN DEL BOTÓN MAYOR A 1s
            //En este estado, le damos un valor al estado del led2 para que empiece a prenderse
      		state_led2 = 2;
            state_s1 = 6;
            break; 

        case 6:
            if( DEBUG ){Serial.println("PULSADO!!");}
            state_s1 = 0;
            break; 
    }  	
}
 
void SM_led1(){ //MAQUINA DE ESTADO DEL LED 1
	state_prev_led1 = state_led1;
	switch(state_led1){
		case 0: //RESET
			beep_count_led1 = 0;
			state_led1 = 1;
			break;
		
		case 1: //ESPERA
			//Espera hasta que el state_s1 sea 6 
			break;
		
		case 2: //SE ENCIENDE EL LED Y GUARDA EL TIEMPO INICIAL
			PORTB = B00001000;
			t_0_led1 = millis();
			state_led1 = 3;
			break;
			
		case 3://ON
			t_led1 = millis();
     		if(t_led1 - t_0_led1 > on_delay_led1){ //delay de encendido del led
				state_led1 = 4;
            }
			break;
		
		case 4: //TURN OFF
			beep_count_led1++; //contador de blink
			t_0_led1 = millis();
      		PORTB = B00000000;
			state_led1 = 5;
			break;
		
		case 5: //OFF
			t_led1 = millis();
			if( t_led1 - t_0_led1 > off_delay_led1) state_led1 = 2;
			if(beep_count_led1 >= beep_numer_led1) state_led1 = 0;
			break;
	}    
}  

void SM_led2(){ //MAQUINA DE ESTADO DEL LED 2
	state_prev_led2 = state_led2;
	switch(state_led2){
		case 0: //RESET
			beep_count_led2 = 0;
			state_led2 = 1;
			break;
		
		case 1: //ESPERA
			//Espera hasta que el state_s1 sea 5
			break;
		
		case 2: //SE ENCIENDE EL LED Y GUARDA EL TIEMPO INICIAL
			PORTC = B00000001;
			t_0_led2 = millis();
			state_led2 = 3;
			break;
			
		case 3://ON
			t_led2 = millis();
      		if(t_led2 - t_0_led2 > on_delay_led2){ //delay de encendido del led
				state_led2 = 4;
            }
			break;
		
		case 4: //TURN OFF
			beep_count_led2++; //contador de blink
			t_0_led2 = millis();
      		PORTC = B00000000;
			state_led2 = 5;
			break;
		
		case 5: //OFF
			t_led2 = millis();
			if( t_led2 - t_0_led2 > off_delay_led2) state_led2 = 2;
			if(beep_count_led2 >= beep_numer_led2) state_led2 = 0;
			break;
	}    
} 