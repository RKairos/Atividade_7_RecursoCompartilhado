#include <zephyr.h>
#include <sys/printk.h>
#include <console/console.h>
#include "padaria.h"

void main(void)
{
    char escolha;

    console_init();

    printk("=== Atividade 7 - Padaria ===\n");
    printk("Escolha o modo:\n");
    printk("1 - Parte 1: Sem sincronizacao\n");
    printk("2 - Parte 2: Com mutex\n");

    while (1)
    {
        escolha = console_getchar();

        if (escolha == '1')
        {
            printk("\nModo escolhido: Parte 1\n");
            padaria_parte1_init();
            break;
        }
        else if (escolha == '2')
        {
            printk("\nModo escolhido: Parte 2\n");
            padaria_parte2_init();
            break;
        }
        else
        {
            printk("Digite 1 ou 2.\n");
        }
    }
}