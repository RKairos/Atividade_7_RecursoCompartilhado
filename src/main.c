#include <zephyr.h>
#include <sys/printk.h>
#include <console/console.h>
#include "padaria.h"

void main(void)
{
    char escolha;

    console_init();

    printk("\n===== ATIVIDADE 7 =====\n");
    printk("1 - Parte 1 (Sem sincronizacao)\n");
    printk("2 - Parte 2 (Mutex)\n");
    printk("3 - Parte 3 (Semaforos)\n");

    while (1)
    {
        escolha = console_getchar();

        switch(escolha)
        {
            case '1':
                padaria_parte1_init();
                return;

            case '2':
                padaria_parte2_init();
                return;

            case '3':
                padaria_parte3_init();
                return;

            default:
                printk("Digite apenas 1, 2 ou 3.\n");
        }
    }
}