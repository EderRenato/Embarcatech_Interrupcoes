#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/pio.h"
#include "ws2818b.pio.h"

#define LED_PIN 13          // Pino do LED simples
#define LED_COUNT 25        // Número de LEDs na matriz
#define MATRIX_PIN 7        // Pino da matriz de LEDs
#define BOTAO_A 5           // Pino do botão A
#define BOTAO_B 6           // Pino do botão B
#define DEBOUNCE_DELAY 50   // Tempo de debounce para os botões

// Estrutura para representar um pixel (LED)
struct pixel_t {
    uint8_t G, R, B;        // Componentes de cor: Verde, Vermelho e Azul
};

typedef struct pixel_t pixel_t; // Alias para a estrutura pixel_t
typedef pixel_t npLED_t;        // Alias para facilitar o uso no contexto de LEDs

npLED_t leds[LED_COUNT];        // Array para armazenar o estado de cada LED
PIO np_pio;                     // Variável para referenciar a instância PIO usada
uint sm;                        // Variável para armazenar o número do state machine usado

// Função para calcular o índice do LED na matriz
int getIndex(int x, int y) {
    x = 4 - x; // Inverte as colunas (0 -> 4, 1 -> 3, etc.)
    y = 4 - y; // Inverte as linhas (0 -> 4, 1 -> 3, etc.)
    if (y % 2 == 0) {
        return y * 5 + x;       // Linha par (esquerda para direita)
    } else {
        return y * 5 + (4 - x); // Linha ímpar (direita para esquerda)
    }
}

// Função para inicializar o PIO para controle dos LEDs
void npInit(uint pin) {
    uint offset = pio_add_program(pio0, &ws2818b_program); // Carregar o programa PIO
    np_pio = pio0;                                         // Usar o primeiro bloco PIO

    sm = pio_claim_unused_sm(np_pio, false);              // Tentar usar uma state machine do pio0
    if (sm < 0) {                                         // Se não houver disponível no pio0
        np_pio = pio1;                                    // Mudar para o pio1
        sm = pio_claim_unused_sm(np_pio, true);           // Usar uma state machine do pio1
    }

    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f); // Inicializar state machine para LEDs

    for (uint i = 0; i < LED_COUNT; ++i) {                // Inicializar todos os LEDs como apagados
        leds[i].R = 0;
        leds[i].G = 0;
        leds[i].B = 0;
    }
}

// Função para definir a cor de um LED específico
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[index].R = r;                                    // Definir componente vermelho
    leds[index].G = g;                                    // Definir componente verde
    leds[index].B = b;                                    // Definir componente azul
}

// Função para limpar (apagar) todos os LEDs
void npClear() {
    for (uint i = 0; i < LED_COUNT; ++i) {                // Iterar sobre todos os LEDs
        npSetLED(i, 0, 0, 0);                             // Definir cor como preta (apagado)
    }
    npWrite();                                            // Atualizar LEDs no hardware
}

// Função para atualizar os LEDs no hardware
void npWrite() {
    for (uint i = 0; i < LED_COUNT; ++i) {                // Iterar sobre todos os LEDs
        pio_sm_put_blocking(np_pio, sm, leds[i].G);       // Enviar componente verde
        pio_sm_put_blocking(np_pio, sm, leds[i].R);       // Enviar componente vermelho
        pio_sm_put_blocking(np_pio, sm, leds[i].B);       // Enviar componente azul
    }
}

// Função executada no Core 1
void core1_entry() {
    while (true) {
        gpio_put(LED_PIN, true);  // Liga o LED
        sleep_ms(200);
        gpio_put(LED_PIN, false); // Desliga o LED
        sleep_ms(200);
    }
}
void display_numerico(int frame) {
    int matriz[10][5][5][3] = {
                {
                    {{0, 0, 0}, {193, 192, 191}, {193, 192, 191}, {193, 192, 191}, {0, 0, 0}},
                    {{0, 0, 0}, {193, 192, 191}, {0, 0, 0}, {193, 192, 191}, {0, 0, 0}},
                    {{0, 0, 0}, {193, 192, 191}, {0, 0, 0}, {193, 192, 191}, {0, 0, 0}},
                    {{0, 0, 0}, {193, 192, 191}, {0, 0, 0}, {193, 192, 191}, {0, 0, 0}},
                    {{0, 0, 0}, {193, 192, 191}, {193, 192, 191}, {193, 192, 191}, {0, 0, 0}}
                },
                {
                    {{0, 0, 0}, {0, 101, 13}, {0, 101, 13}, {0, 0, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 0, 0}, {0, 101, 13}, {0, 0, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 0, 0}, {0, 101, 13}, {0, 0, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 0, 0}, {0, 101, 13}, {0, 0, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 101, 13}, {0, 101, 13}, {0, 101, 13}, {0, 0, 0}}
                },
                {
                    {{0, 0, 0}, {101, 0, 0}, {101, 0, 0}, {101, 0, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {101, 0, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {101, 0, 0}, {101, 0, 0}, {101, 0, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {101, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {101, 0, 0}, {101, 0, 0}, {101, 0, 0}, {0, 0, 0}}
                },
                {
                    {{0, 0, 0}, {0, 16, 101}, {0, 16, 101}, {0, 16, 101}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 16, 101}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 16, 101}, {0, 16, 101}, {0, 16, 101}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 16, 101}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 16, 101}, {0, 16, 101}, {0, 16, 101}, {0, 0, 0}}
                },
                {
                    {{0, 0, 0}, {97, 0, 90}, {0, 0, 0}, {97, 0, 90}, {0, 0, 0}},
                    {{0, 0, 0}, {97, 0, 90}, {0, 0, 0}, {97, 0, 90}, {0, 0, 0}},
                    {{0, 0, 0}, {97, 0, 90}, {97, 0, 90}, {97, 0, 90}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {97, 0, 90}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {97, 0, 90}, {0, 0, 0}}
                },
                {
                    {{0, 0, 0}, {0, 92, 97}, {0, 92, 97}, {0, 92, 97}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 92, 97}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 92, 97}, {0, 92, 97}, {0, 92, 97}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 92, 97}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 92, 97}, {0, 92, 97}, {0, 92, 97}, {0, 0, 0}}
                },
                {
                    {{0, 0, 0}, {101, 0, 80}, {101, 0, 80}, {101, 0, 80}, {0, 0, 0}},
                    {{0, 0, 0}, {101, 0, 80}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {101, 0, 80}, {101, 0, 80}, {101, 0, 80}, {0, 0, 0}},
                    {{0, 0, 0}, {101, 0, 80}, {0, 0, 0}, {101, 0, 80}, {0, 0, 0}},
                    {{0, 0, 0}, {101, 0, 80}, {101, 0, 80}, {101, 0, 80}, {0, 0, 0}}
                },
                {
                    {{0, 0, 0}, {248, 255, 0}, {248, 255, 0}, {248, 255, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {248, 255, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {248, 255, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {248, 255, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {248, 255, 0}, {0, 0, 0}}
                },
                {
                    {{0, 0, 0}, {34, 255, 0}, {34, 255, 0}, {34, 255, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {34, 255, 0}, {0, 0, 0}, {34, 255, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {34, 255, 0}, {34, 255, 0}, {34, 255, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {34, 255, 0}, {0, 0, 0}, {34, 255, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {34, 255, 0}, {34, 255, 0}, {34, 255, 0}, {0, 0, 0}}
                },
                {
                    {{0, 0, 0}, {255, 162, 0}, {255, 162, 0}, {255, 162, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {255, 162, 0}, {0, 0, 0}, {255, 162, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {255, 162, 0}, {255, 162, 0}, {255, 162, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 162, 0}, {0, 0, 0}},
                    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 162, 0}, {0, 0, 0}}
                }};
    
    // Desenhar o número na matriz de LEDs
    for (int linha = 0; linha < 5; linha++) {
        for (int coluna = 0; coluna < 5; coluna++) {
            int posicao = getIndex(linha, coluna);
            npSetLED(posicao, matriz[frame][coluna][linha][0], matriz[frame][coluna][linha][1], matriz[frame][coluna][linha][2]);
        }
    }
    npWrite(); // Atualizar LEDs no hardware
}

int main() {
    stdio_init_all(); // Inicializar stdio
    gpio_init(BOTAO_A);
    gpio_init(BOTAO_B);
    gpio_init(LED_PIN);

    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_pull_up(BOTAO_A);
    gpio_pull_up(BOTAO_B);

    npInit(MATRIX_PIN); // Inicializar o PIO para controle dos LEDs

    multicore_launch_core1(core1_entry); // Iniciar o Core 1

    int contador = 0;
    while (true) {
        display_numerico(contador); // Exibir o número atual na matriz de LEDs

        // Verificar botão A (incrementar contador)
        if (gpio_get(BOTAO_A) == 0) {
            sleep_ms(DEBOUNCE_DELAY);
            if (contador != 9) {
                contador++;
            } else {
                contador = 0;
            }
        }

        // Verificar botão B (decrementar contador)
        if (gpio_get(BOTAO_B) == 0) {
            sleep_ms(DEBOUNCE_DELAY);
            if (contador != 0) {
                contador--;
            } else {
                contador = 9;
            }
        }

        printf("Contador: %d\n", contador); // Exibir o valor do contador no console
        sleep_ms(100); // Pequena pausa para evitar sobrecarga
    }

    return 0;
}