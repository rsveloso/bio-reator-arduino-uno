/**************************************************************************************************************************************************************************
Exemplo de utilização da library AcquaShield com o Shield de OD, este exemplo mostra como realizar a leitura de OD e temperatura do Shield, realizar o procedimento
de calbração do eletrodo de OD, fazer a leitura dos parâmetros de calibração e mostrar dados no display do Shield de OD.

******** Procedimento de calibração do eletrodo de OD: ********
** Abra o "Serial Monitor" do Arduino.
-> Envie o comando #ODCal#
** O Shield retornará "Procedimento de calibração iniciado...".
** O Shield começará a enviar as leituras de tensão do circuito.
** Retire o eletrodo de OD do conector do Shield e coloque um "Jumper" entre os terminais. 
-> Envie o comando #Cal,cc#
<- O Shield deve retornar *OK
** Este comando faz a correção da tensão de offset do circuito eletrônico.
** Retire o jumper do conector do Shield e conecte o eletrodo de OD.
** Separe a solução de calibração (0 mg/L).

** Coloque o eletrodo, inicialmente, na solução de calibração.
** O Shield começará a enviar as leituras de OD, aguarde até que a leitura de OD se estabilize.
-> Envie o comando #Cal,00#
<- O Shield deverá retornar *OK.
** Lave o eletrodo em águua corrente.
** Posteriormente, deixe o eletrodo em ar atmosférico e aguarde a leitura de OD, no terminal serial, se estabilizar.
-> Envie o comando #Cal,ar#
<- O Shield deve retornar *OK
** Ao finalizar os procedimentos de calibração...

-> Envie o comando F
** O Shield retornará "Rotina de calibração finalizada, reinicie o sistema..."
** Reinicie todo sistema (Arduino + Shield).
** Pronto, seu Shield de OD está calibrado.

******** Comandos de configuracao e vizualização de parâmetros ********
-> Comandos para alterar a indicação de OD:
   #ODMgl#     -> Imprime o valor de OD em mg/L
   #ODSat#     -> Imprime o valor de OD em % de saturação

-> Comando para imprimir a tensão de offset do circuito A/D:
   #ODOft#   -> Imprime no terminal o valor de tensão de offset em mV

-> Comando para imprimir a tensão de calibração do eletrodo em solução 0 mg/L:
   #ODV00#   -> Imprime no terminal o valor de tensão de em mV

-> Comando para imprimir a tensão de calibração do eletrodo em contato com o Ar:
   #ODVar#   -> Imprime no terminal o valor de tensão de em mV

-> Comando para apagar a calibração do Shield
   #ODCeras# -> Apaga a ultima calibração realizada e coloca os valores de calibração default.


** Firmware desenvolvido para Arduino UNO **
** Acquanativa **
** 18/08/2022 **
***************************************************************************************************************************************************************************/

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "include/AcquaShield.h"
#include "include/Ultrassom.h"   // ➜ novo include
#include "include/PHSensor.h"    // pH sensor (local include)

ODShield Shield_OD; //Instancia a variável "ODShield".
Ultrassom sensorUltrassom(13, 12);  // trig, echo
PHSensor sensorPH(A0); // sensor pH (A0)
const uint8_t PINO_TEMP2 = 4; // DS18B20 conectado diretamente ao pino D4
OneWire oneWireTemp2(PINO_TEMP2);
DallasTemperature sensorTemp2(&oneWireTemp2);

void setup() 
{
    Serial.begin(9600);   //Inicializa a UART do Arduino.
    Shield_OD.init();     //Inicializa o Shield de OD.
    sensorTemp2.begin();
    sensorTemp2.setResolution(10); // resolucao de 0,25 C para conversao mais rapida

    od_type = saturacao;        //iicializa variável externa para indicar tipo de impressão de OD

    delay(1000);
    Shield_OD.DispBuffClear();                      //Limpa o buffer do display do Shield de OD.
    Shield_OD.DispWrite(3, 13, 21, "Acquanativa");  //Envia a string para o buffer do display do Shield de OD.
    Shield_OD.DispShow();                           //Mostra a string no display do Shield de OD.
    delay(1000); 
}

void loop() 
{
    static char vetor_temp[6];             //Vetor para armazenar a string de temperatura do Shield de EC.
    static char vetor_OD[12];               //Vetor para armazenar a string de EC do Shield de EC.
      
    static unsigned int previous_mod = 0;  //Variável para contagem do tempo de loop.

    float od = 0.0;

    
    if(millis() - previous_mod >= 1000)
    {   
        //Sensor Ultrassônico
        float distancia = sensorUltrassom.lerDistancia();

        //Serial.print("Distância Ultrassom: ");
        //Serial.print(distancia);
        //Serial.println(" cm");
        //Fim sensor Ultrasônico

        Shield_OD.DispBuffClear();      //Limpa o buffer do display do Shield de OD.
        
        if(od_type == mgl)
        {
            od = Shield_OD.OD();                       //Faz a leitura do valor de OD do Shield.

            if(od < 10.0)
            {
                dtostrf(od, 2, 1, vetor_OD);               //Faz a leitura do OD e converte a variável float "OD" em string.
                strcpy(vetor_OD, vetor_OD);                //Deixa a String no formato correto para ser mostrada no display do Shield de OD.     
                Shield_OD.DispWrite(4, 6, 16, "OD:");      //.DispWrite(Fonte(1 a 4), x, y, string); -> Envia a string para o buffer do display do Shield de OD. 
                Shield_OD.DispWrite(4, 49, 16, vetor_OD);  //.DispWrite(Fonte(1 a 4), x, y, string); -> Envia o vetor de string, com o valor de OD, para o buffer do display do Shield de OD. 
                Shield_OD.DispWrite(2, 96, 16, "mg/L");    //.DispWrite(Fonte(1 a 4), x, y, string); -> Envia a string para o buffer do display do Shield de OD.  
            }
            else
            {

                dtostrf(od, 2, 1, vetor_OD);               //Faz a leitura do OD e converte a variável float "OD" em string.
                strcpy(vetor_OD, vetor_OD);                //Deixa a String no formato correto para ser mostrada no display do Shield de OD.     
                Shield_OD.DispWrite(4, 6, 16, "OD:");      //.DispWrite(Fonte(1 a 4), x, y, string); -> Envia a string para o buffer do display do Shield de OD. 
                Shield_OD.DispWrite(4, 49, 16, vetor_OD);  //.DispWrite(Fonte(1 a 4), x, y, string); -> Envia o vetor de string, com o valor de OD, para o buffer do display do Shield de OD. 
                Shield_OD.DispWrite(2, 105, 16, "mg/L");    //.DispWrite(Fonte(1 a 4), x, y, string); -> Envia a string para o buffer do display do Shield de OD.  
            }

        } 
        else
        {
            od = Shield_OD.ODsat();  

            Shield_OD.DispWrite(4, 6, 16, "OD:");           //.DispWrite(Fonte(1 a 4), x, y, string); -> Envia a string para o buffer do display do Shield de OD.  
   
            if(od < 10.0)
            {
                dtostrf(od, 2, 1, vetor_OD);               //Faz a leitura do OD e converte a variável float "OD" em string.
                strcpy(vetor_OD, vetor_OD);
                Shield_OD.DispWrite(4, 49, 16, vetor_OD);
                Shield_OD.DispWrite(2, 100, 16, "%");            //.DispWrite(Fonte(1 a 4), x, y, string); -> Envia a string para o buffer do display do Shield de OD.  

            }
            else if((od >= 10.0) & (od <= 99.9))
            {
                dtostrf(od, 2, 1, vetor_OD);               //Faz a leitura do OD e converte a variável float "OD" em string.
                strcpy(vetor_OD, vetor_OD);
                Shield_OD.DispWrite(4, 49, 16, vetor_OD);
                Shield_OD.DispWrite(2, 110, 16, "%");            //.DispWrite(Fonte(1 a 4), x, y, string); -> Envia a string para o buffer do display do Shield de OD.  
  
            }
            else
            {
                dtostrf(od, 3, 0, vetor_OD);               //Faz a leitura do OD e converte a variável float "OD" em string.
                strcpy(vetor_OD, vetor_OD);
                Shield_OD.DispWrite(4, 49, 16, vetor_OD);
                Shield_OD.DispWrite(2, 105, 16, "%");            //.DispWrite(Fonte(1 a 4), x, y, string); -> Envia a string para o buffer do display do Shield de OD.   
      
            } 

        }      

        // ----------- LEITURA DE TEMPERATURA -----------
        float temp = Shield_OD.temp();
        sensorTemp2.requestTemperatures();
        float temp2 = sensorTemp2.getTempCByIndex(0);
        float ph = sensorPH.readPH();

        // ----------- PRINT NO SERIAL MONITOR -----------
        // ----------- SAÍDA SERIAL PADRONIZADA ----------------
        // Formato: Temp=xx.xx; Temp2=xx.xx; OD=yy.yy; pH=zz.zz; Dist=aa.aa;

        Serial.print("Temp=");
        Serial.print(temp);
        Serial.print("; ");

        Serial.print("Temp2=");
        if (temp2 == DEVICE_DISCONNECTED_C) Serial.print("-127.00");
        else Serial.print(temp2, 2);
        Serial.print("; ");

        Serial.print("OD=");
        Serial.print(od);
        Serial.print("; ");

        Serial.print("pH=");
        Serial.print(ph, 2);
        Serial.print("; ");

        Serial.print("Dist=");
        Serial.print(distancia);
        Serial.println(";");

        // ----------- DISPLAY -----------
        //dtostrf(Shield_OD.temp(), 2, 1, vetor_temp);  //Faz a leitura da temperatura e converte a variável float "temp" em string.
        dtostrf(temp, 2, 1, vetor_temp);  //ajuste com a linha de cima.
        strcpy(vetor_temp, vetor_temp);               //Deixa a String no formato correto para ser mostrada no display do Shield de OD.
        Shield_OD.DispWrite(1, 7, 32, "Temp:");       //.DispWrite(Fonte(1 a 4), x, y, string); -> Envia a string para o buffer do display do Shield de OD.
        Shield_OD.DispWrite(1, 40, 32, vetor_temp);   //.DispWrite(Fonte(1 a 4), x, y, string); -> Envia o vetor de string, com o valor de temperatura, para o buffer do display do Shield de OD.
    
        Shield_OD.DispShow();                         //Mostra a string no display do Shield de OD.   
       
        previous_mod = millis();
    }
}
