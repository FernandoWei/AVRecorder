/*
add by zongchunli
用于将H264裸数据打包成RTMP Packet
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rtmp.h"
#include "rtmp_sys.h"
#include "rtmp_buffer.h"






/*
用于将收到的H264裸数据打包成RTMP packet，并放入video buffer中，等待发送
sVideoData:输入的H264裸数据
pVideoBuffer:video 缓冲区
*/
int RTMP_Packet_H264(RTMP* pRtmp,unsigned char * sVideoData,int size,uint32_t nFrameTime,RTMPBuffer *pVideoBuffer);

/*
对SPS帧进行解析，获得所需要的参数
*/
int RTMP_Decode_SPS(unsigned char * sVideoData,RTMP_SPS_PPS *sSps);

/*
将SPS，PPS数据组装成packetde data
*/
int RTMP_Packet_SPS_PPS(RTMP* pRtmp,RTMP_SPS_PPS sSPSPPS,uint32_t nTime,RTMPPacket *pPacket);


/*
将H264 NAL 组装成packet data
*/
int RTMP_Packet_NAL(RTMP* pRtmp,unsigned char * sVideoData,int size,uint32_t nTime,int iKey,RTMPPacket *pPacket);




