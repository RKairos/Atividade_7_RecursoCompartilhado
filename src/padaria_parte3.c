#include <zephyr.h>
#include <sys/printk.h>
#include "padaria.h"

#define STACK_SIZE 1024
#define PRIORIDADE 5

#define CAPACIDADE_VITRINE 10

static int saldo_vitrine = 0;

/*
 * Semáforo que representa os espaços livres na vitrine.
 * Começa com 10, pois a vitrine começa vazia.
 */
K_SEM_DEFINE(sem_espacos_livres, CAPACIDADE_VITRINE, CAPACIDADE_VITRINE);

/*
 * Semáforo que representa os pães disponíveis.
 * Começa com 0, pois inicialmente não há pães na vitrine.
 */
K_SEM_DEFINE(sem_paes_disponiveis, 0, CAPACIDADE_VITRINE);

K_THREAD_STACK_DEFINE(stack_padeiro_3, STACK_SIZE);
K_THREAD_STACK_DEFINE(stack_cliente_3, STACK_SIZE);

static struct k_thread thread_padeiro_3;
static struct k_thread thread_cliente_3;

static void padeiro_thread(void *a, void *b, void *c)
{
    while (1)
    {
        /*
         * O padeiro só pode produzir se houver espaço livre.
         * Se a vitrine estiver cheia, ele fica bloqueado aqui.
         */
        k_sem_take(&sem_espacos_livres, K_FOREVER);

        saldo_vitrine++;

        printk("[P3 PADEIRO] Produziu 1 pao | Vitrine = %d\n",
               saldo_vitrine);

        /*
         * Após produzir, há mais um pão disponível.
         */
        k_sem_give(&sem_paes_disponiveis);

        k_msleep(1000);
    }
}

static void cliente_thread(void *a, void *b, void *c)
{
    while (1)
    {
        /*
         * O cliente só pode retirar se houver pão disponível.
         * Se a vitrine estiver vazia, ele fica bloqueado aqui.
         */
        k_sem_take(&sem_paes_disponiveis, K_FOREVER);

        saldo_vitrine--;

        printk("[P3 CLIENTE] Retirou 1 pao | Vitrine = %d\n",
               saldo_vitrine);

        /*
         * Após retirar, há mais um espaço livre.
         */
        k_sem_give(&sem_espacos_livres);

        k_msleep(1500);
    }
}

void padaria_parte3_init(void)
{
    printk("=== Parte 3: Utilizando Semaforos ===\n");
    printk("Capacidade maxima da vitrine: %d paes\n", CAPACIDADE_VITRINE);

    k_thread_create(&thread_padeiro_3,
                    stack_padeiro_3,
                    STACK_SIZE,
                    padeiro_thread,
                    NULL, NULL, NULL,
                    PRIORIDADE,
                    0,
                    K_NO_WAIT);

    k_thread_create(&thread_cliente_3,
                    stack_cliente_3,
                    STACK_SIZE,
                    cliente_thread,
                    NULL, NULL, NULL,
                    PRIORIDADE,
                    0,
                    K_NO_WAIT);
}