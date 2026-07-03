#include <zephyr.h>
#include <sys/printk.h>
#include "padaria.h"

#define STACK_SIZE 1024
#define PRIORIDADE_PADEIRO 5
#define PRIORIDADE_CLIENTE 5

static int saldo_vitrine = 0;

K_THREAD_STACK_DEFINE(stack_padeiro, STACK_SIZE);
K_THREAD_STACK_DEFINE(stack_cliente, STACK_SIZE);

static struct k_thread thread_padeiro;
static struct k_thread thread_cliente;

static void padeiro_thread(void *arg1, void *arg2, void *arg3)
{
    while (1)
    {
        saldo_vitrine++;

        printk("[PADEIRO] Produziu 1 pão | Vitrine = %d\n",
               saldo_vitrine);

        k_msleep(1000);
    }
}

static void cliente_thread(void *arg1, void *arg2, void *arg3)
{
    while (1)
    {
        if (saldo_vitrine > 0)
        {
            saldo_vitrine--;

            printk("[CLIENTE] Retirou 1 pão | Vitrine = %d\n",
                   saldo_vitrine);
        }
        else
        {
            printk("[CLIENTE] Tentou retirar pão, mas a vitrine está vazia\n");
        }

        k_msleep(1500);
    }
}

void padaria_init(void)
{
    printk("=== Atividade 7 - Parte 1: Sem sincronização ===\n");

    k_thread_create(&thread_padeiro,
                    stack_padeiro,
                    STACK_SIZE,
                    padeiro_thread,
                    NULL, NULL, NULL,
                    PRIORIDADE_PADEIRO,
                    0,
                    K_NO_WAIT);

    k_thread_create(&thread_cliente,
                    stack_cliente,
                    STACK_SIZE,
                    cliente_thread,
                    NULL, NULL, NULL,
                    PRIORIDADE_CLIENTE,
                    0,
                    K_NO_WAIT);
}