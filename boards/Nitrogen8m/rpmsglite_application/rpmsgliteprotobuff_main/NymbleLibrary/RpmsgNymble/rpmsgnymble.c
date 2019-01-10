/*
 * rpmsgnymble.c
 *
 *  Created on: 20-Dec-2018
 *      Author: Abhinav@Nymble
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rpmsg_lite.h"
#include "rpmsgnymble.h"
#include "rpmsg_queue.h"
#include "rpmsg_ns.h"
#include "board.h"
#include "fsl_debug_console.h"


/* Globals */
volatile unsigned long remote_addr = 0 ;
volatile int has_received = 0 , has_received2 = 0 ;
struct rpmsg_lite_ept_static_context my_ept_context;
struct rpmsg_lite_ept_static_context my_ept2_context;
rpmsg_ns_static_context my_ns_context;
struct rpmsg_lite_endpoint *my_ept;
struct rpmsg_lite_endpoint *my_ept2;
struct rpmsg_lite_instance rpmsg_ctxt;
struct rpmsg_lite_instance *my_rpmsg;
void *rx_buf;
volatile bool handshake_hello_linux_bootup_message = false ;

volatile int count_isr = 1 ;

typedef struct the_message_rx
{
    uint8_t rec_DATA;
} THE_MESSAGE_RX, *THE_MESSAGE_RX_PTR;
THE_MESSAGE_RX volatile msg_rx = {0};


typedef struct the_message_tx
{
    uint8_t tx_DATA;
} THE_MESSAGE_TX, *THE_MESSAGE_TX_PTR;
THE_MESSAGE_TX volatile msg_tx = {0};

void app_nameservice_isr_cb(unsigned int new_ept, const char *new_ept_name, unsigned long flags, void *user_data)
{
    PRINTF("\r\napp nameservice isr \r\n");
}

static int my_ept_read_cb(void *payload, int payload_len, unsigned long src, void *priv)
{
    if (false == handshake_hello_linux_bootup_message)
    {
        handshake_hello_linux_bootup_message = true ;
        int *has_received = priv;
        remote_addr = src;
        *has_received = 1;

    }

    if (payload_len <= sizeof(THE_MESSAGE_RX))
    {
        memcpy((void *)&msg_rx, payload, payload_len);
        PRINTF("receive-Data:  %u \r\n", msg_rx.rec_DATA);
        int *has_received = priv;
        remote_addr = src;
        *has_received = 1;
    }

    return RL_RELEASE;
}


void rpmsg_config(void)
{

    my_rpmsg = rpmsg_lite_remote_init((void *)RPMSG_LITE_SHMEM_BASE, RPMSG_LITE_LINK_ID, RL_NO_FLAGS, &rpmsg_ctxt);
    while (!rpmsg_lite_is_link_up(my_rpmsg))       ;
    PRINTF("Link is up!\r\n");
    my_ept = rpmsg_lite_create_ept(my_rpmsg, LOCAL_EPT_ADDR, my_ept_read_cb, (void *)&has_received, &my_ept_context);
    rpmsg_ns_bind(my_rpmsg, app_nameservice_isr_cb, NULL, &my_ns_context);
    rpmsg_ns_announce(my_rpmsg, my_ept, RPMSG_LITE_NS_ANNOUNCE_STRING, RL_NS_CREATE);
    has_received = 0;
    PRINTF("\r\nNameservice sent, ready for incoming messages...\r\n");
    PRINTF("\r\nWaiting for response...\r\n");
}


void rpmsg_process(void)
{

    if (has_received)
    {
        has_received = 0;
        PRINTF("receive-Data:  %u \r\n", msg_rx.rec_DATA);

    }



}

void rpmsg_deinit(void)
{

    PRINTF("Ping pong done, deinitializing...\r\n");
    rpmsg_lite_destroy_ept(my_rpmsg, my_ept);
    my_ept = NULL;
    rpmsg_lite_deinit(my_rpmsg);
    msg_rx.rec_DATA = 0;
    PRINTF("Looping forever...\r\n");



}

void rpmsg_send(const uint8_t *data, size_t length)
{
    if (true == handshake_hello_linux_bootup_message)
    {
        while (length--)
        {

            msg_tx.tx_DATA = *(data++) ;
            rpmsg_lite_send(my_rpmsg, my_ept, IMX_A53_EPT_ADDR, (uint8_t *)&msg_tx, sizeof(THE_MESSAGE_TX), RL_DONT_BLOCK);
            PRINTF("Sent-Data:  %u \r\n", msg_tx.tx_DATA);
        }

    }

}

void rpmsg_transmit(void)
{

    rpmsg_lite_send(my_rpmsg, my_ept, IMX_A53_EPT_ADDR, (uint8_t *)&msg_tx, sizeof(THE_MESSAGE_TX), RL_DONT_BLOCK);
    msg_tx.tx_DATA = msg_tx.tx_DATA + 1 ;
    if (msg_tx.tx_DATA >= 255)
    {
        msg_tx.tx_DATA = 0 ;
    }
    PRINTF("Sent-Data:  %u \r\n", msg_tx.tx_DATA);







}