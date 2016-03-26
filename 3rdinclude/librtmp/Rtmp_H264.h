/*
add by zongchunli
���ڽ�H264�����ݴ����RTMP Packet
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rtmp.h"
#include "rtmp_sys.h"
#include "rtmp_buffer.h"






/*
���ڽ��յ���H264�����ݴ����RTMP packet��������video buffer�У��ȴ�����
sVideoData:�����H264������
pVideoBuffer:video ������
*/
int RTMP_Packet_H264(RTMP* pRtmp,unsigned char * sVideoData,int size,uint32_t nFrameTime,RTMPBuffer *pVideoBuffer);

/*
��SPS֡���н������������Ҫ�Ĳ���
*/
int RTMP_Decode_SPS(unsigned char * sVideoData,RTMP_SPS_PPS *sSps);

/*
��SPS��PPS������װ��packetde data
*/
int RTMP_Packet_SPS_PPS(RTMP* pRtmp,RTMP_SPS_PPS sSPSPPS,uint32_t nTime,RTMPPacket *pPacket);


/*
��H264 NAL ��װ��packet data
*/
int RTMP_Packet_NAL(RTMP* pRtmp,unsigned char * sVideoData,int size,uint32_t nTime,int iKey,RTMPPacket *pPacket);




