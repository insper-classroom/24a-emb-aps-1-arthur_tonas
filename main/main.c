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
#include "funcoes.h"

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

    int sequencia_player[TAM_SEQUENCIA];
    int etapa_player = 0;

    gera_sequencia(sequencia);

    int erro = 0;

    while (true) {
        for (int i = 0; i < TAM_SEQUENCIA; i ++){
            // TOCA SEQUÊNCIA ATÉ O NÍVEL
            printf("i: %d\n", i);
            for (int j = 0; j <= i; j++){
                som_e_led(sequencia[j], 500, BUZ_PIN);
                sleep_ms(500);
            }
            etapa_player = 0;
            
            while(etapa_player <= i && !erro){
                // ESPERA O JOGADOR APERTAR ANDO
                if (callback_flag){

                    if (btn_r_flag){
                        som_e_led(1, 500, BUZ_PIN);
                        sequencia_player[etapa_player] = 1;
                        etapa_player += 1;
                        btn_r_flag = 0;
                    }
                    if (btn_g_flag){
                        som_e_led(2, 500, BUZ_PIN);
                        sequencia_player[etapa_player] = 2;
                        etapa_player += 1;
                        btn_g_flag = 0;
                    }
                    if (btn_b_flag){
                        som_e_led(3, 500, BUZ_PIN);
                        sequencia_player[etapa_player] = 3;
                        etapa_player += 1;
                        btn_b_flag = 0;
                    }
                    if (btn_y_flag){
                        som_e_led(4, 500, BUZ_PIN);
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
                erro = 0;
                script_erro(i);
                gera_sequencia(sequencia);
                break; // NÃO TENHO CERTEZA SE ESSE BREAK TA FUNCIONANDO COMO DEVERIA
            } else{
                sleep_ms(1000); // PODE AJUSTAR ESSE SLEEP SE NECESSÁRIO
                //script acerto
            }
        }
    }
}
