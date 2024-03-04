/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "time.h"

const int BTN_PIN_R = 18;
const int BTN_PIN_G = 19;
const int BTN_PIN_B = 20;
const int BTN_PIN_Y = 21;

volatile int btn_r_flag = 0;
volatile int btn_g_flag = 0;
volatile int btn_b_flag = 0;
volatile int btn_y_flag = 0;
volatile int callback_flag = 0;

const int LED_PIN_R = 10;
const int LED_PIN_G = 11;
const int LED_PIN_B = 12;
const int LED_PIN_Y = 13;

const int BUZ_PIN = 16;

const int TAM_SEQUENCIA = 10;

void btn_callback(uint gpio, uint32_t events){
    callback_flag = 1;
    if (gpio == BTN_PIN_R) btn_r_flag = 1;
    if (gpio == BTN_PIN_G) btn_g_flag = 1;
    if (gpio == BTN_PIN_B) btn_b_flag = 1;
    if (gpio == BTN_PIN_Y) btn_y_flag = 1;
}

void buzzer(int codigo, int tempo, int pin){
    int freq;

    if (codigo == 1){
        freq = 200;
    } else if (codigo == 2){
        freq = 300;
    } else if (codigo == 3){
        freq = 400;
    } else if (codigo == 4){
        freq = 500;
    }

    int tempo_us = tempo * 1000000;
    int periodo_us = 1000000 / freq;
    int ciclos = tempo_us / periodo_us;

    for (int i = 0; i < ciclos; i++){
        gpio_put(pin, 1);
        sleep_us(freq/2);
        gpio_put(pin, 0);
        sleep_us(freq/2);
    }
}

void led(int codigo, int t){
    int pino;

    if (codigo == 1){
        pino = 200;
    } else if (codigo == 2){
        pino = 300;
    } else if (codigo == 3){
        pino = 400;
    } else if (codigo == 4){
        pino = 500;
    }

    gpio_put(pino, 1);
    sleep_ms(t);
    gpio_put(pino, 0);
}

void gera_sequencia(int *psequencia) {
    for (int i =  0; i < TAM_SEQUENCIA; i++){
        psequencia[i] = (rand() % 4) + 1;
    }
}

int main() {
    stdio_init_all();
    srand(time(NULL));

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);
    gpio_set_irq_enabled_with_callback(
        BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);
    gpio_set_irq_enabled(
        BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);

    gpio_init(BTN_PIN_B);
    gpio_set_dir(BTN_PIN_B, GPIO_IN);
    gpio_pull_up(BTN_PIN_B);
    gpio_set_irq_enabled(
        BTN_PIN_B, GPIO_IRQ_EDGE_FALL, true);

    gpio_init(BTN_PIN_Y);
    gpio_set_dir(BTN_PIN_Y, GPIO_IN);
    gpio_pull_up(BTN_PIN_Y);
    gpio_set_irq_enabled(
        BTN_PIN_Y, GPIO_IRQ_EDGE_FALL, true);

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);

    gpio_init(LED_PIN_B);
    gpio_set_dir(LED_PIN_B, GPIO_OUT);

    gpio_init(LED_PIN_Y);
    gpio_set_dir(LED_PIN_Y, GPIO_OUT);

    gpio_init(BUZ_PIN);
    gpio_set_dir(BUZ_PIN, GPIO_OUT);

    int sequencia[TAM_SEQUENCIA];
    // sequencia[0] = 1;
    // sequencia[1] = 2;
    // sequencia[2] = 3;
    // sequencia[3] = 4;

    int sequencia_player[TAM_SEQUENCIA];
    int etapa_player = 0;

    gera_sequencia(sequencia);

    int erro = 0;

    while (true) {
        for (int i = 0; i < TAM_SEQUENCIA; i ++){
            //Toca a sequencia
            printf("i: %d\n", i);
            for (int j = 0; j <= i; j++){
                buzzer(sequencia[j], 1, BUZ_PIN);
                //led(sequencia[j], 500);
                sleep_ms(500);
            }
            etapa_player = 0;
            
            while(etapa_player <= i && !erro){

                if (callback_flag){

                    if (btn_r_flag){
                        buzzer(1, 1, BUZ_PIN);
                        //led(1, 500);
                        sequencia_player[etapa_player] = 1;
                        etapa_player += 1;
                        btn_r_flag = 0;
                    }
                    if (btn_g_flag){
                        buzzer(2, 1, BUZ_PIN);
                        //led(2, 500);
                        sequencia_player[etapa_player] = 2;
                        etapa_player += 1;
                        btn_g_flag = 0;
                    }
                    if (btn_b_flag){
                        buzzer(3, 1, BUZ_PIN);
                        //led(3, 500);
                        sequencia_player[etapa_player] = 3;
                        etapa_player += 1;
                        btn_b_flag = 0;
                    }
                    if (btn_y_flag){
                        buzzer(4, 1, BUZ_PIN);
                        //led(4, 500);
                        sequencia_player[etapa_player] = 4;
                        etapa_player += 1;
                        btn_y_flag = 0;
                    }
                    if (sequencia_player[etapa_player-1] != sequencia[etapa_player-1]){
                        erro = 1;
                        callback_flag = 0;
                    }
                    callback_flag = 0;
                }
            }

            if (erro){
                gera_sequencia(sequencia);
                break;
            } else{
                sleep_ms(500);
                //script acerto
            }
            
            

        }
    }
}
