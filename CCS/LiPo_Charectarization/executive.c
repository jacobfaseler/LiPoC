/*
 * executive.c
 *
 *  Created on: Feb 19, 2022
 *      Author: JacobFaseler
 */

/* Includes */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* POSIX Header Files */
#include <mqueue.h>
#include <pthread.h>
#include <time.h>

/* Local Header Files */
#include "uart_thread.h"

/* Local Header Files */
#include "executive.h"

/* Global Variables */
pthread_t exec_thr_handler;
mqd_t bcmq_exec;
struct timespec exec_lat, exec_lat_rem =
{
    .tv_sec = 20,
    .tv_nsec = 0
};
struct timespec bcmq_lat =
{
    .tv_sec = 1,
    .tv_nsec = 0
};

/*
 * ======== Finite State Machine Definitions ========
 */

/* Fundamental State Type */
typedef enum {
    S_cleanup,
    S_startup,
    S_setup,
    S_run_test,
    S_signal_complete,
    S_stress_bc,
    S_states_count
} State;

/* Finite States */
State cleanup(void);
State startup(void);
State setup(void);
State run_test(void);
State signal_complete(void);
State stress_bc(void);

/* Function Pointer Directory */
State (*states[7])(void) =
        {
         cleanup,
         startup,
         setup,
         run_test,
         signal_complete,
         stress_bc
        };

/* State Functions */

State cleanup(void)
{
    State ret_state_cu = S_cleanup;
    ;
    return ret_state_cu;
}

State startup(void)
{
    State ret_state_stu = S_cleanup;
    ;
    return ret_state_stu;
}

State setup(void)
{
    State ret_state_seu = S_cleanup;
    ;
    return ret_state_seu;
}

State run_test(void)
{
    State ret_state_rt = S_cleanup;
    ;
    return ret_state_rt;
}

State signal_complete(void)
{
    State ret_state_sc = S_cleanup;
    ;
    return ret_state_sc;
}

State stress_bc(void)
{
    State ret_state_sb = S_cleanup;
    ;
    return ret_state_sb;
}

/*
 * ======== Executive Thread ========
 */

void *exec_thr(void *arg0)
{
    bcmq_exec = mq_open("bc_uart_mq", O_RDWR);
    State next_state = S_cleanup;

    /* Wakeup Message */
    bc_msg dmymsg = {
           .mes = "Executive Startup",
           .len = 18
    };
    mq_timedsend(bcmq_exec, (char*)&dmymsg, sizeof(bc_msg), 0, &bcmq_lat);

    /* State Machine */
    while(1)
    {
        nanosleep(&exec_lat, &exec_lat_rem);
        next_state = (*states[next_state])();
    }

#ifdef STRESS_BC_UART
    bc_msg dmymsg = {
           .mes = "THIS IS A TEST",
           .len = 16
    };
    int test_it;

    mq_timedsend(bcmq_exec, (char*)&dmymsg, sizeof(bc_msg),0, &bcmq_lat);
    while (1)
    {
        nanosleep(&exec_lat, &exec_lat_rem);
        dmymsg.mes[14] = ' ';
        for (test_it = '0' ; test_it <='6' ; test_it++){
            dmymsg.mes[15] = (char)test_it;
            mq_timedsend(bcmq_exec, (char*)&dmymsg, sizeof(bc_msg), 0, &bcmq_lat);
        }
    }
#endif

#ifndef STRESS_BC_UART

#endif

}
