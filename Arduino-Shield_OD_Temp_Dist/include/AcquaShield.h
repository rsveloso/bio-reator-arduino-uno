#ifndef ACQUASHIELD_H_INCLUDED
#define ACQUASHIELD_H_INCLUDED

#include <Arduino.h>

#define StartCommand '#'
#define EndCommand '#'

#define ShieldOD_addr 0xE7   //Endereço do Shield no protocolo UART (0xE7).

//enumera opções de improessao de OD
enum od_disp {mgl, saturacao};

//define variavel global para opção de impressao de OD
extern od_disp od_type;

class ODShield
{
    public:
        init(void);
        float temp(void);
        float OD(void);
        float ODsat(void);
        void DispWrite(char font, int x, int y, char str[]);  //Escreve uma string no display do pHShield.
        void DispBuffClear(void);                             //Apaga o buffer do display do pHShield.
        void DispClear(void);                                 //Apaga o display do pHShield.
        void DispShow(void);                                  //Envia os dados do fuffer para a tela do display do pHShield.
    
    private:
};

#endif