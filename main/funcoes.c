#include "funcoes.h"
#include "time.h"


const int BTN_PIN_R = 18;
const int BTN_PIN_G = 19;
const int BTN_PIN_B = 20;
const int BTN_PIN_Y = 21;

const int LED_PIN_R = 10;
const int LED_PIN_G = 11;
const int LED_PIN_B = 12;
const int LED_PIN_Y = 13;

const int BUZ_PIN = 16;

const int TAM_SEQUENCIA = 100;

volatile int btn_r_flag = 0;
volatile int btn_g_flag = 0;
volatile int btn_b_flag = 0;
volatile int btn_y_flag = 0;
volatile int callback_flag = 0;


void btn_callback(uint gpio, uint32_t events){
    callback_flag = 1;
    if (gpio == BTN_PIN_R) btn_r_flag = 1;
    if (gpio == BTN_PIN_G) btn_g_flag = 1;
    if (gpio == BTN_PIN_B) btn_b_flag = 1;
    if (gpio == BTN_PIN_Y) btn_y_flag = 1;
}

void buzzer(int freq, int tempo){
    int tempo_us = tempo * 1000;
    int periodo_us = 1000000 / freq;
    int ciclos = tempo_us / periodo_us;

    for (int i = 0; i < ciclos; i++){
        gpio_put(BUZ_PIN, 1);
        sleep_us(periodo_us/2);
        gpio_put(BUZ_PIN, 0);
        sleep_us(periodo_us/2);
    }
}

void som_e_led(int codigo, int tempo, int pin){
    int freq;
    int led_pin;

    if (codigo == 1){
        freq = 523;
        led_pin = LED_PIN_R;
    } else if (codigo == 2){
        freq = 587;
        led_pin = LED_PIN_G;
    } else if (codigo == 3){
        freq = 659;
        led_pin = LED_PIN_B;
    } else if (codigo == 4){
        freq = 698;
        led_pin = LED_PIN_Y;
    }

    int tempo_us = tempo * 1000;
    int periodo_us = 1000000 / freq;
    int ciclos = tempo_us / periodo_us;

    gpio_put(led_pin, 1);
    for (int i = 0; i < ciclos; i++){
        gpio_put(pin, 1);
        sleep_us(periodo_us/2);
        gpio_put(pin, 0);
        sleep_us(periodo_us/2);
    }
    gpio_put(led_pin, 0);
}

void script_erro(int pontos){
    for (int i = 0; i < pontos; i ++){
        gpio_put(LED_PIN_R, 1);
        gpio_put(LED_PIN_G, 1);
        gpio_put(LED_PIN_B, 1);
        gpio_put(LED_PIN_Y, 1);
        buzzer(1000, 500);
        gpio_put(LED_PIN_R, 0);
        gpio_put(LED_PIN_G, 0);
        gpio_put(LED_PIN_B, 0);
        gpio_put(LED_PIN_Y, 0);
        sleep_ms(500);

    }
}

void gera_sequencia(int *psequencia) {
    for (int i =  0; i < TAM_SEQUENCIA; i++){
        psequencia[i] = (rand() % 4) + 1;
    }
}