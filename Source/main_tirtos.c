/*
 * Derivative work ; Jacob Faseler 13Feb2022
 */

/*
 *  ======== main_tirtos.c ========
 */
/* Includes */
#include <stdint.h>

/* POSIX Header Files */
#include <pthread.h>
#include <mqueue.h>

/* RTOS Header Files */
#include <ti/sysbios/BIOS.h>

/* Example/Board Header Files */
#include <ti/drivers/Board.h>

/* Local Header Files */
#include "mem_man_tools.h"
#include "callbacks.h"
#include "uart_thread.h"
#include "executive.h"

/* Stack size in bytes */
#define THREADSTACKSIZE    1024

/*
 *  ======== main ========
 */
int main(void)
{
    pthread_attr_t      pAttrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

    mqd_t               bcmq_main;
    struct mq_attr      attr;

    /* Nuke the SRAM? */
//    sram_wipe ();

    /* Call driver init functions */
    Board_init();
    config_bc_uart();
    config_ow_uart();

    /* Message queue for posting to back channel UART */
    attr.mq_maxmsg = BC_MAX_MSGS;
    attr.mq_msgsize = sizeof(bc_msg);
    attr.mq_curmsgs = 0;
    attr.mq_flags = 0;
    bcmq_main = mq_open("bc_uart_mq", O_RDWR | O_CREAT, 0664, &attr);
    if (bcmq_main == (mqd_t)-1) {/* mq_open() failed*/ while (1) ;}



//    /*
//     *  Message Queue to send messages to LED thread.
//     *  It is non-blocking to allow it to be called by ISRs (e.g. GPIO and
//     *  Timer callbacks). Note: senders need to use semLED to actually
//     *  wake up the LED thread.
//     */
//
//    attr.mq_maxmsg = LED_MSG_NUM;
//    attr.mq_msgsize = LED_MSG_SIZE;
//    attr.mq_flags = 0;
//    mqLED = mq_open("led", O_RDWR | O_CREAT | O_NONBLOCK, 0664, &attr);
//    if (mqLED == (mqd_t)-1) {
//         mq_open() failed
//        while (1);
//    }
//
//    /* Semaphore to notify LED thread that msg is present. */
//    retc = sem_init(&semLED, 0, 0);
//    if (retc == -1) {
//        while (1);
//    }

    /* Set priority and stack size attributes*/
    pthread_attr_init(&pAttrs);

    detachState = PTHREAD_CREATE_DETACHED;

    retc = pthread_attr_setdetachstate(&pAttrs, detachState);
    if (retc != 0) {/*pthread_attr_setdetachstate() failed*/ while (1) ; }

    retc |= pthread_attr_setstacksize(&pAttrs, THREADSTACKSIZE);
    if (retc != 0) { /*pthread_attr_setstacksize() failed*/ while (1) ; }

    /* Create UART Read Thread with priority = 3 */
    priParam.sched_priority = 3;
    pthread_attr_setschedparam(&pAttrs, &priParam);
    retc = pthread_create(&bc_r_thr_handler, &pAttrs, bc_r_thr, NULL);
    if (retc != 0) { /*  pthread_create() failed */ while (1) ; }

    /* Create UART Write Thread with priority = 2 */
    priParam.sched_priority = 2;
    pthread_attr_setschedparam(&pAttrs, &priParam);
    retc = pthread_create(&bc_w_thr_handler, &pAttrs, bc_w_thr, NULL);
    if (retc != 0) { /* pthread_create() failed */ while (1) ; }

    /* Create executive thread with priority = 1 */
    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&pAttrs, &priParam);
    retc = pthread_create(&exec_thr_handler, &pAttrs, exec_thr, NULL);
    if (retc != 0) { /* pthread_create() failed */ while (1) ; }

    BIOS_start();

    return (0);
}
