/*
 * rpmsgnymble.h
 *
 *  Created on: 20-Dec-2018
 *      Author: Abhinav@Nymble
 */



#ifndef RPMSGNYMBLE_H_
#define RPMSGNYMBLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rpmsg_lite.h"
#include "rpmsgnymble.h"
#include "rpmsg_queue.h"
#include "rpmsg_ns.h"
#include "board.h"
#include "fsl_debug_console.h"
#include<stdint.h>
#include<ysizet.h>

#define RPMSG_LITE_SHMEM_BASE (0xB8000000U)
#define RPMSG_LITE_LINK_ID (RL_PLATFORM_IMX8MQ_M4_USER_LINK_ID)
#define RPMSG_LITE_NS_ANNOUNCE_STRING "rpmsg-virtual-tty-channel-1"
#define APP_TASK_STACK_SIZE (256)
#define IMX_A53_EPT_ADDR (1024)
#ifndef LOCAL_EPT_ADDR
#define LOCAL_EPT_ADDR (30)
#endif
#define LOCAL_EPT_ADDR_2 (50)

void rpmsg_config(void);
void rpmsg_process(void);
void rpmsg_deinit(void);
void rpmsg_send(const uint8_t *data, size_t length);
void rpmsg_transmit(void);








#endif /* RPMSGNYMBLE_H_ */
