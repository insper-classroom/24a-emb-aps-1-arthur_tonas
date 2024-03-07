#include "funcoes.h"
#include "time.h"
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"


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

const int TEMPO = 140;

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


void playMelody(){
    int melody[] = {
        // Cantina BAnd - Star wars 
        // Score available at https://musescore.com/user/6795541/scores/1606876
        NOTE_B4,-4, NOTE_E5,-4, NOTE_B4,-4, NOTE_E5,-4, 
        NOTE_B4,8,  NOTE_E5,-4, NOTE_B4,8, REST,8,  NOTE_AS4,8, NOTE_B4,8, 
        NOTE_B4,8,  NOTE_AS4,8, NOTE_B4,8, NOTE_A4,8, REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_G4,8,
        NOTE_G4,4,  NOTE_E4,-2, 
        NOTE_B4,-4, NOTE_E5,-4, NOTE_B4,-4, NOTE_E5,-4, 
        NOTE_B4,8,  NOTE_E5,-4, NOTE_B4,8, REST,8,  NOTE_AS4,8, NOTE_B4,8,

        NOTE_A4,-4, NOTE_A4,-4, NOTE_GS4,8, NOTE_A4,-4,
        NOTE_D5,8,  NOTE_C5,-4, NOTE_B4,-4, NOTE_A4,-4,
        NOTE_B4,-4, NOTE_E5,-4, NOTE_B4,-4, NOTE_E5,-4, 
        NOTE_B4,8,  NOTE_E5,-4, NOTE_B4,8, REST,8,  NOTE_AS4,8, NOTE_B4,8,
        NOTE_D5,4, NOTE_D5,-4, NOTE_B4,8, NOTE_A4,-4,
        NOTE_G4,-4, NOTE_E4,-2,
        NOTE_E4, 2, NOTE_G4,2,
        NOTE_B4, 2, NOTE_D5,2,

        NOTE_F5, -4, NOTE_E5,-4, NOTE_AS4,8, NOTE_AS4,8, NOTE_B4,4, NOTE_G4,4, 
    };

    int notes = 64;

    int wholenote = (60000 * 2) / TEMPO;

    int divider = 0, noteDuration = 0;

    for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

        // calculates the duration of each note
        gpio_put(LED_PIN_R, 1);
        gpio_put(LED_PIN_G, 1);
        gpio_put(LED_PIN_B, 1);
        gpio_put(LED_PIN_Y, 1);

        divider = melody[thisNote + 1];
        if (divider > 0) {
        // regular note, just proceed
        noteDuration = (wholenote) / divider;
        } else if (divider < 0) {
        // dotted notes are represented with negative durations!!
        noteDuration = (wholenote) / abs(divider);
        noteDuration *= 1.5; // increases the duration in half for dotted notes
        }

        // we only play the note for 90% of the duration, leaving 10% as a pause
        buzzer(melody[thisNote], noteDuration*0.9);
        
        gpio_put(LED_PIN_R, 0);
        gpio_put(LED_PIN_G, 0);
        gpio_put(LED_PIN_B, 0);
        gpio_put(LED_PIN_Y, 0);

        sleep_ms(noteDuration*0.1);

        if (callback_flag == 1) {
            break; // Sai do loop se callback_flag for igual a 1
        }

        
  }
}
