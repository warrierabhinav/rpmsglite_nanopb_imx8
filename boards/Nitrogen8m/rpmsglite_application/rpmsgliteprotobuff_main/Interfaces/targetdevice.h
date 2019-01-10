/*
 * targetdevice.h
 *
 *  Created on: 06-Decs-2018
 *      Author: Abhinav@Nymble
 */

#include<stdint.h>
#include<ysizet.h>
#include "mcu.pb.h"
#ifndef TARGETDEVICE_H_
#define TARGETDEVICE_H_




void Mpu_packet_send(McuMpuMessage packet);
void send_commit_info(void);






























#endif /* TARGETDEVICE_H_ */
