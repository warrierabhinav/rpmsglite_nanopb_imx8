/*
 * targetdevice.c
 *
 *  Created on: 06-Dec-2018
 *      Author: Abhinav@Nymble
 */
#include<ysizet.h>
#include "targetdevice.h"

#include "mcu.pb.h"
#include "..\NymbleLibrary\NanoPb\pb.h"
#include "..\NymbleLibrary\NanoPb\pb_common.h"
#include "..\NymbleLibrary\NanoPb\pb_encode.h"
#include "..\NymbleLibrary\NanoPb\pb_decode.h"
#include "..\NymbleLibrary\RpmsgNymble\rpmsgnymble.h"
#include "fsl_debug_console.h"

uint8_t commit_id_info_mcu[8] = "93665e7" ;


static void Mpu_packet_pack_send(uint8_t *payload, uint8_t length)
{
  
    uint8_t buffer[McuMpuMessage_size]; 
    uint8_t *p_packet = buffer;
    memcpy(p_packet, payload, length);

    rpmsg_send(buffer, (p_packet - buffer)) ;

}
void Mpu_packet_send(McuMpuMessage packet)
{
    uint8_t send_buff[McuMpuMessage_size];
    size_t send_buff_length = 0 ;
    bool status = false ;

    pb_ostream_t stream = pb_ostream_from_buffer(send_buff, sizeof(send_buff));

    switch (packet.which_val)
    {
        case McuMpuMessage_commit_info_tag:
        {
            for (uint8_t y = 0; y < 8; y++)
            {
                packet.val.commit_info.commit[y] = commit_id_info_mcu[y];
            }
        }
        break;

    
        default:
            break;


    }

    status = pb_encode(&stream, McuMpuMessage_fields, &packet);
    send_buff_length = stream.bytes_written;

    if (status == true)
    {
        Mpu_packet_pack_send(send_buff, send_buff_length);
    }
    else
    {
        PRINTF("\r\nNanoPb Encode failed\r\n");
    }


}

void send_commit_info(void)
{

    McuMpuMessage message = McuMpuMessage_init_zero;
    message.which_val = McuMpuMessage_commit_info_tag ;
    Mpu_packet_send(message);

}
