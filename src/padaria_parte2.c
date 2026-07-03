#include <zephyr.h>
#include <sys/printk.h>
#include "padaria.h"

#define STACK_SIZE 1024
#define PRIORIDADE 5

static int saldo_vitrine = 0;

K_MUTEX_DEFINE(mutex_vitrine);

K_THREAD_STACK_DEFINE(stack_padeiro_2, STACK_SIZE);
K_THREAD_STACK_DEFINE(stack_cliente_2, STACK_SIZE);

static struct k_thread thread_padeiro_2;
static struct k_thread thread_cliente_2;

static void padeiro_thread(void *a, void *b, void *c)
{
    while (1)
    {
        k_mutex_lock(&mutex_vitrine, K_FOREVER);

        saldo_vitrine++;
        printk("[P2 PADEIRO] Produziu 1 pao | Vitrine = %d\n", saldo_vitrine);

        k_mutex_unlock(&mutex_vitrine);

        k_msleep(1000);
    }
}

static void cliente_thread(void *a, void *b, void *c)
{
    while (1)
    {
        k_mutex_lock(&mutex_vitrine, K_FOREVER);

        if (saldo_vitrine > 0)
        {
            saldo_vitrine--;
            printk("[P2 CLIENTE] Retirou 1 pao | Vitrine = %d\n", saldo_vitrine);
        }
        else
        {
            printk("[P2 CLIENTE] Vitrine vazia\n");
        }

        k_mutex_unlock(&mutex_vitrine);

        k_msleep(1500);
    }
}

void padaria_parte2_init(void)
{
    printk("=== Parte 2: Utilizando Mutex ===\n");

    k_thread_create(&thread_padeiro_2, stack_padeiro_2, STACK_SIZE,
                    padeiro_thread, NULL, NULL, NULL,
                    PRIORIDADE, 0, K_NO_WAIT);

    k_thread_create(&thread_cliente_2, stack_cliente_2, STACK_SIZE,
                    cliente_thread, NULL, NULL, NULL,
                    PRIORIDADE, 0, K_NO_WAIT);
}