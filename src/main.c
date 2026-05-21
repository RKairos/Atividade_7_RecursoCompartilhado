#include <zephyr/kernel.h>             // Funções básicas do Zephyr (ex: k_msleep, k_thread, etc.)
#include <zephyr/device.h>             // API para obter e utilizar dispositivos do sistema
#include <zephyr/drivers/gpio.h>       // API para controle de pinos de entrada/saída (GPIO)
#include <zephyr/drivers/uart.h>
#include <stdlib.h>
#include <stdio.h>
#include <pwm_z42.h>

// Define o valor do registrador MOD do TPM para configurar o período do PWM
#define TPM_MODULE 1000         // Define a frequência do PWM fpwm = (TPM_CLK / (TPM_MODULE * PS))
// Valores de duty cycle correspondentes a diferentes larguras de pulso
// Duty cycle dos LEDs
#define RED_BASE    (100)   // vermelho 100%
#define GREEN_BASE (35) // verde 36%


uint16_t duty_red;         
uint16_t duty_green;   
uint16_t duty_blue;


// Obtém automaticamente a UART configurada no Device Tree como console,
// permitindo comunicação entre o monitor serial do PC e a placa
const struct device *uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

//cria função de leitura de integer
int ler_int_uart(void)
{
    char buffer[4]; // aceita 0 até 100(999)
    int i = 0; //serve como contador
    unsigned char c; //caracter de valor c

    while(1) //cria um loop infinito para tempo infinito de espera de entrada
    {
        if(uart_poll_in(uart_dev, &c) == 0) //se conseguir ler um caractere entra nessa caso
        {
            // Se recebeu número, guarda no buffer
            if(c >= '0' && c <= '9')
            {
                if(i < 3) //garante que não vai estourar tamanho do buffer
                {
                    buffer[i] = c;
                    i++;
                    uart_poll_out(uart_dev, c); // mostra o que foi digitado
                }
            }

            // Se recebeu Enter e já tem algo digitado, encerra
            if((c == '\r' || c == '\n') && i > 0)
            {
                break;
            }
        }

        k_msleep(10);
    }

    buffer[i] = '\0'; //colocar fator null no final para finalizar o string

    return atoi(buffer);
}




int intensidade;

uint16_t calcula_duty(uint16_t base, int intensidade)
{
    uint16_t brilho;

    // Mantém a proporção da cor e aplica a intensidade geral
    brilho = (TPM_MODULE * base * intensidade) / (100 * 100);

    // Inverte porque o LED da FRDM-KL25Z é active low
    return TPM_MODULE - brilho;
}



int main(void)
{
    // Inicializa TPM2
    pwm_tpm_Init(TPM2, TPM_OSCERCLK, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);
    // Inicializa TPM3
    pwm_tpm_Init(TPM0, TPM_OSCERCLK, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);

    /*
      FRDM-KL25Z RGB LED:
      PTB18 → Vermelho
      PTB19 → Verde
      PTD1  → Azul
    */

    // Inicializa canais PWM
    pwm_tpm_Ch_Init(TPM2,0,TPM_PWM_H,GPIOB,18); // Vermelho
    pwm_tpm_Ch_Init(TPM2,1,TPM_PWM_H,GPIOB,19); // Verde
    pwm_tpm_Ch_Init(TPM0,1,TPM_PWM_H,GPIOD,1);  // Azul


    while(1)
    {
        printf("\r\nDigite intensidade (0-100): ");

        intensidade = ler_int_uart();

    // Limites
        if(intensidade < 0)
            intensidade = 0;

        if(intensidade > 100)
            intensidade = 100;
        

        duty_red   = calcula_duty(RED_BASE, intensidade);
        duty_green = calcula_duty(GREEN_BASE, intensidade);
        duty_blue  = TPM_MODULE;
        pwm_tpm_CnV(TPM2,0,duty_red);
        pwm_tpm_CnV(TPM2,1,duty_green);
        pwm_tpm_CnV(TPM0,1,duty_blue);


        printf("\r\nIntensidade aplicada: %d%%\r\n", intensidade);
    
    }

    return 0;
}