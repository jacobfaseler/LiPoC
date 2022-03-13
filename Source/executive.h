/*
 * executive.h
 *
 *  Created on: Feb 19, 2022
 *      Author: JacobFaseler
 */

/* POSIX Header Files */
#include <pthread.h>
#include <mqueue.h>

/* Local Header Files */
#include "uart_thread.h"

#ifndef EXECUTIVE_H_
#define EXECUTIVE_H_

//#define STRESS_BC_UART

typedef struct bc_mes {
    char mes[BC_BUF_LEN - BC_BUF_MARGIN];
    int  len;
} bc_msg;

extern pthread_t exec_thr_handler;
extern void *exec_thr(void *arg0);


#endif /* EXECUTIVE_H_ */
