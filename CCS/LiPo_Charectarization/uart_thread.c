/*
 * Derivative work by: Jacob Faseler ; 13Feb2022
 */

/*
 *  ======== uart_thread.c ========
 */

/* Includes */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* POSIX Header Files */
#include <semaphore.h>
#include <mqueue.h>

/* Driver Header Files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>

/* Board Header file */
#include "ti_drivers_config.h"

/* Local Header Files */
#include "callbacks.h"
#include "uart_thread.h"
#include "executive.h"
#include "heartbeat.h"

/* Global Variables */
pthread_t bc_w_thr_handler;
pthread_t bc_r_thr_handler;
UART_Handle uart_handle;

pthread_t ow_w_thr_handler;
pthread_t ow_r_thr_handler;
UART_Handle ow_handle;

char bc_r_buf[BC_BUF_LEN];
int str_ready = 0;
int in_str_len = 0;

/*
 *  ======== Peripheral Configuration ========
 */

void config_bc_uart()
{

    UART_Params uartParams;

    /* Call driver init functions */
    GPIO_init();
    UART_init();
//    Timer_init();

    /* Define UART parameters*/
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_ON;
    uartParams.baudRate = 115200;

    uart_handle = UART_open(CONFIG_UART0, &uartParams);

    if (uart_handle == NULL) {/* UART_open() failed */ while (1);}
}

void config_ow_uart()
{
    UART_Params owParams;

    /* Call driver init functions */
    GPIO_init();
    UART_init();

    /* Define UART parameters */
    UART_Params_init(&owParams);
    owParams.writeDataMode = UART_DATA_BINARY;
    owParams.readDataMode = UART_DATA_BINARY;
    owParams.readReturnMode = UART_RETURN_FULL;
    owParams.readEcho = UART_ECHO_OFF;
    owParams.baudRate = 115200;

    ow_handle = UART_open(CONFIG_UART1, &owParams);

    if (ow_handle == NULL) {/* UART_open() failed */ while(1);}
}

/*
 *  ======== 1-Wire Threads ========
 */
void *ow_w_thr(void *arg0)
{
    while (1)
    {
        ;
    }
}

void *ow_r_thr(void *arg0)
{
    while (1)
    {
        ;
    }
}

/*
 * Directive Parsing
 * Returns 1 if str starts with "DIR", else returns 0
 */
int parse_for_dir(char str_in[], int str_len)
{
    char dir_str[] = "DIR";
    int i;

    if (str_len < 3) {return 0;}
    for (i = 0 ; i < 3 ; i++ )
    {
        if (!(str_in[i] == dir_str[i])) {return 0;}
    }
    return 1;
}

int interpret_dir(char str_in[], int str_len)
{
    char stress_bc_str[] = "stressbc";
    int i;
    int is_phrase = 0;

    if (!(str_len < 12))
    {
        is_phrase = 1;
        for (i = 3 ; i < 12 ; i++){
            if (!(str_in[i] == stress_bc_str[i])) { is_phrase = 0; }
        }
        if (is_phrase) {return 1;}
        return -1;
    }
    return -1;
}

/*
 *  ======== BackChannelThreads ========
 */
void *bc_w_thr(void *arg0)
{
    char greeting[] = "Hello\n\r";
    char nl_cr[] = "\n\r";
    char parsed[] = "Recieved:";
    char detect[] = "Which is a directive";
    char int_err[] = "Issue interpreting Directive";
    char int_sbc[] = "Commence BC UART Stress Test";
    char def_str[] = "Directive not implemented";
    struct timespec uart_slp_rem, uart_slp_time = {
        .tv_sec = 1,
        .tv_nsec = 0
    };
    mqd_t bcmq_uart;
    struct mq_attr query;
    bc_msg mes_rec;                /* Received bcmq message buffer */
    int messages;
    ssize_t mes_len;


    /* Initialize Message Queue Instance */
    bcmq_uart = mq_open("bc_uart_mq", O_RDWR);

    /* Smart Echo + Post Messages */
    UART_write(uart_handle, &greeting, 8);
    while (1)
    {
        /* Check for carriage-returned user input */
        if (str_ready)
        {
            UART_write(uart_handle, &nl_cr, 2);
            UART_write(uart_handle, &parsed, sizeof(parsed)/sizeof(char));
            UART_write(uart_handle, &bc_r_buf, in_str_len);
            UART_write(uart_handle, &nl_cr, 2);
            /* Check input for Directives */
            if (parse_for_dir(bc_r_buf, in_str_len))
            {
                UART_write(uart_handle, &nl_cr, 2);
                UART_write(uart_handle, &detect, sizeof(detect)/sizeof(char));
                UART_write(uart_handle, &nl_cr, 2);
                switch (interpret_dir(bc_r_buf, in_str_len)) {
                    case -1 :
                        UART_write(uart_handle, &int_err, sizeof(int_err)/sizeof(char));
                        UART_write(uart_handle, &nl_cr, 2);
                        break;
                    case 1 :
                        UART_write(uart_handle, &int_sbc, sizeof(int_sbc)/sizeof(char));
                        UART_write(uart_handle, &nl_cr, 2);
                        break;
                    default :
                        UART_write(uart_handle, &def_str, sizeof(def_str)/sizeof(char));
                        UART_write(uart_handle, &nl_cr, 2);
                        break;
                }
            }
            str_ready = 0;
            in_str_len = 0;
        }
        /* Check for notifications from the executive */
        mq_getattr(bcmq_uart, &query);
        messages = query.mq_curmsgs;
        if ( messages >= 1)
        {
            while (messages >= 1)
            {
                mes_len = mq_receive(bcmq_uart, (char*)&mes_rec, sizeof(bc_msg), 0);
                UART_write(uart_handle, &nl_cr, 2);
                UART_write(uart_handle, (char*)mes_rec.mes, mes_rec.len);
                UART_write(uart_handle, &nl_cr, 2);
                --messages;
                mes_len++; /* Warning Aversion */
            }
        }
        nanosleep(&uart_slp_time, &uart_slp_rem);
    }
}

void *bc_r_thr(void *arg0)
{
    struct timespec bc_r_int, bc_r_int_rem = {
        .tv_sec = 0,
        .tv_nsec = 100000
    };
    char byte_in;

    while (1)
    {
        UART_read(uart_handle, &byte_in, 1);
        UART_write(uart_handle, &byte_in, 1);
        bc_r_buf[in_str_len++] = byte_in;
        if (byte_in == '\r') {str_ready = 1;}
        if (in_str_len >= (BC_BUF_LEN - BC_BUF_MARGIN)) {str_ready = 1;}
        nanosleep(&bc_r_int, &bc_r_int_rem);
    }

}
