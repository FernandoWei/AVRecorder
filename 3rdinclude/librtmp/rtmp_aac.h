
/*

add by zongchunli@kankan.com
用于将aac裸数据打包成RTMP Packet

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "rtmp.h"
#include "rtmp_sys.h"
#include "rtmp_buffer.h"



int Detach_Head_Aac(RTMP_ADTS_HEADER * adtsheader,char * Adts_Headr_Buf);

/*
用于将收到的aac裸数据根据rtmp协议的格式打包进packet中，放入audio buffer中

sAudioData:aac裸数据
pAudioBuffer: audio缓冲区
*/
int RTMP_Packet_AAC(RTMP* pRtmp,unsigned char * sAudioData,int size,uint32_t nFrameTime,RTMPBuffer *pAudioBuffer);

/*
将AAC 的sequence header 封装进packet
*/

RTMPPacket* RTMP_AAC_SequenceHeader_Packet(RTMP* pRtmp,RTMP_AAC_ASC *pAacConfig);

int RTMP_AAC_SequenceHeader(RTMP* pRtmp,RTMP_AAC_ASC *pAacConfig,RTMPBuffer *pAudioBuffer);

/*
将sequence 设置成默认值
*/

int RTMP_Set_Default_Sequence(RTMP_AAC_ASC *pAacConfig);

void RTMP_Set_AAC_SequenceHeader(RTMP_AAC_ASC *pAacConfig,int channel , int FrequencyIndex, int samplesize,int profile);



