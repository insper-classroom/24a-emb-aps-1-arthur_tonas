#ifndef FUNCOES_H
#define FUNCOES_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Constantes
extern const int BTN_PIN_R;
extern const int BTN_PIN_G;
extern const int BTN_PIN_B;
extern const int BTN_PIN_Y;

extern const int LED_PIN_R;
extern const int LED_PIN_G;
extern const int LED_PIN_B;
extern const int LED_PIN_Y;

extern const int BUZ_PIN;
extern const int TAM_SEQUENCIA;

extern volatile int btn_r_flag;
extern volatile int btn_g_flag;
extern volatile int btn_b_flag;
extern volatile int btn_y_flag;
extern volatile int callback_flag;



// Protótipos de funções
void btn_callback(uint gpio, uint32_t events);
void buzzer(int freq, int tempo);
void som_e_led(int codigo, int tempo, int pin);
void script_erro(int pontos);
void gera_sequencia(int *psequencia);

#endif
