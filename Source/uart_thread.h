/* Derivative work ; Jacob Faseler 13Feb2022 */

/*
 *  ======== uart_thread.h ========
 */

/* POSIX Header files */
#include <pthread.h>
#include <mqueue.h>

/* Driver Header Files */
#include <ti/drivers/UART.h>

#ifndef UART_THREAD_H_
#define UART_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#define BC_MAX_MSGS 6
#define BC_BUF_LEN 100
#define BC_BUF_MARGIN 20


void config_bc_uart();
void config_ow_uart();

extern pthread_t           bc_w_thr_handler;
extern void *bc_w_thr(void *arg0);
extern pthread_t           bc_r_thr_handler;
extern void *bc_r_thr(void *arg0);

#endif /* UART_THREAD_H_ */
