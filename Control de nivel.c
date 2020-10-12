/*
PRACTICA 6 - "CONTROL DE NIVEL"
EQUIPO V 
8H1 - ELECTRONICA DIGITAL V
DAVID MADRID NAPOLES 17100201
CARLOS CAMACHO MATA 17100057
HECTOR SAMUEL RIVERA RUIZ 17100287
06/10/2020

DEBE DE MOSTAR LOS NOMBRES Y REGISTROS DE LOS ALUMNOS Y EL NOMBRE DE LA PRACTICA, 
AL PRESIONAR UN BOTON DEBE DE ENTAR A UN MENU DONDE TE PIDO SELECCIONAR UN SP 
QUE SE DEBERA DE CONFIRMAR CON EL MISMO BOTON, AL CONFIRMAR DEBERA DE MOSTRAR 
SI SE ESTA LLENANDO, VACIANDO, O SI EL NIVEL ESTA ESTABLE. EN ESTE MOMENTO SE 
DEBERA DE PODER REGRESAR A CONFIRMAR EL SP

*/
#INCLUDE <16F887.H>
#DEVICE ADC=8 
#USE DELAY(CLOCK=4000000)
#FUSES INTRC_IO,NOWDT,NOPUT,MCLR,NOPROTECT,NOCPD,NOBROWNOUT
#FUSES NOIESO,NOFCMEN,NOLVP,NODEBUG,NOWRT,BORV21

#USE FAST_IO(A)
#USE FAST_IO(B)
#USE FAST_IO(C)
#USE FAST_IO(D)

#INCLUDE <LCD1.C>

INT I = 0, AUX = 0, CUENTA = 0, SP = 0, V = 0;
FLOAT D = 0, K = 0.01961;

CHAR KEYPAD[16]={'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
INT REAL[6] = {1, 2, 3, 100, 4, 5};

VOID READ_US (){
   D = READ_ADC();
   V=D*K;
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "NIVEL = %1u L       "V);
}
VOID TECLADO (){ //SE ENCARGA DE DETECTAR PULSACIONES EN EL TECLADO
   AUX = 0XEF;
   CUENTA = 0; //LLEVARA LA CUENTA, DETECTANDO LA CUENTA PRESIONADO
   FOR(I=0;I<4;I++){//EN CADA ITERACION REVISA UNA FILA DIFERENTE
         
      AUX=AUX>>1; //DESPLAZA UN CERO A LA DERECHA
      OUTPUT_A((INPUT_A()&0XF0)|(AUX&0X0F));
         
      IF(INPUT(PIN_A7)==0) //REVISDA ESTADO COLUMNA A7
      BREAK;
      CUENTA++;
      IF(INPUT(PIN_A6)==0) //REVISDA ESTADO COLUMNA A6
      BREAK;
      CUENTA++;
      IF(INPUT(PIN_A5)==0) //REVISDA ESTADO COLUMNA A5
      BREAK;
      CUENTA++;
      IF(INPUT(PIN_A4)==0) //REVISDA ESTADO COLUMNA A4
      BREAK;
      CUENTA++;
         
      DELAY_MS(20); //TIEMPO DE ESPERA NECESARIO PARA DETECTAR CAMBIOS
   }
}

VOID MAIN (){ //FUNCION PRINCIPAL

   SET_TRIS_A(0XF0); //DECLARAR SALIDAS
   SET_TRIS_C(0X80); //DECLARAR 
   SET_TRIS_B(0X01); //DECLARAR ENTRADA ULTRASONICO
   OUTPUT_A(INPUT_A()&0XF0); //LIMPIAR BITS DE SALIDA
   LCD_INIT(); //INICIAR LCD
   SETUP_ADC(ADC_CLOCK_INTERNAL);
   SETUP_ADC_PORTS(sAN12);
   SET_ADC_CHANNEL(12);   
    
   WHILE (TRUE){ //CICLO INFINITO
   
   FOR(I = 0; I <= 2; I++){ // EN CADA ITERACION MUESTRA EL R Y N DE UN ALM
      SWITCH (I){
         CASE 0:
            LCD_GOTOXY(1,1);
            PRINTF(LCD_PUTC, "DAVID MADRID     \n17100201            ");
            BREAK;
               
         CASE 1:
            LCD_GOTOXY(1,1);
            PRINTF(LCD_PUTC, "HECTOR RUIZ     \n17100287          ");
            BREAK;
                     
         CASE 2:
            LCD_GOTOXY(1,1);
            PRINTF(LCD_PUTC, "CAMACHO MATA     \n17100057         ");
            BREAK;
            }
         DELAY_MS(1000);
         }  
     
      IF(INPUT(PIN_C7)==1){ //ENTRA AL CONTROL DE NIVEL 
         WHILE(INPUT(PIN_C7)==1) //MIENTRAS SE PRESIONE C7 NO HACE NADA
         {LCD_GOTOXY(1,1);
         PRINTF(LCD_PUTC, "SUELTE EL BOTON \nSELECT SP 1-5   ");}
         WHILE(INPUT(PIN_C7)==0){ //AL SOLTAR C7 LEE EL ADC Y PIDE SP
            SP:
            READ_US (); //LEE ULTRA SONICO
            TECLADO (); //LEE TECLADO MATRICIAL            
            IF(CUENTA<16){ //SI SE PRESIONA UNA TECLA
               IF(CUENTA<6){
                  LCD_GOTOXY(1,2);
                  PRINTF(LCD_PUTC, "SP = %c          ",KEYPAD[CUENTA]);
                  SP = REAL[CUENTA];
               }ELSE{
                  LCD_GOTOXY(1,2);
                  PRINTF(LCD_PUTC, "SP INVALIDO 1-5 ");
               }              
            } 
            WHILE(INPUT(PIN_C7)==1){ //CONFIRMAMOS SP
               WHILE(INPUT(PIN_C7)==1){}
               WHILE(INPUT(PIN_C7)==0){
                  READ_US ();
                  IF(SP == V){
                     LCD_GOTOXY(1,2);
                     PRINTF(LCD_PUTC, "SP = %u         E"SP); //ESTABLE
                  }ELSE IF(SP < V){
                     LCD_GOTOXY(1,2);
                     PRINTF(LCD_PUTC, "SP = %u         V"SP); //SE ESTA VACIANDO
                  }ELSE IF(SP > V){
                     LCD_GOTOXY(1,2);
                     PRINTF(LCD_PUTC, "SP = %u         L"SP); //SE ESTA LLENANDO
                  }
               }
               WHILE(INPUT(PIN_C7)==1){
                  WHILE(INPUT(PIN_C7)==1){}
                  WHILE(INPUT(PIN_C7)==0){
                  GOTO CAMBIAR; //VA AL MAIN A IMPRIMIR ALGO EN PANTALLA
                  }
               }
            }
            
         }
      }
     
      
      IF(CUENTA<16){ //SI SE PRESIONA UNA TECLA
                  
         WHILE(CUENTA<16){ //PRESIONADA LA TECLA
            TECLADO (); //SIGUE DETECTANDOLA Y RESPONDE SIN MOSTAR EL MENU
         }
      }
   } 
   CAMBIAR: //INDICA COLOCAR SP
   LCD_GOTOXY(1,2);
   PRINTF(LCD_PUTC, "SELECT SP 1-5   ");
   DELAY_MS(400);
   GOTO SP; //VA A LA PARTE DONDE SELECCIONAMOS SP
}


