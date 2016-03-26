
/*

add by zongchunli@kankan.com
���ڽ�aac�����ݴ����RTMP Packet

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
���ڽ��յ���aac�����ݸ���rtmpЭ��ĸ�ʽ�����packet�У�����audio buffer��

sAudioData:aac������
pAudioBuffer: audio������
*/
int RTMP_Packet_AAC(RTMP* pRtmp,unsigned char * sAudioData,int size,uint32_t nFrameTime,RTMPBuffer *pAudioBuffer);

/*
��AAC ��sequence header ��װ��packet
*/

RTMPPacket* RTMP_AAC_SequenceHeader_Packet(RTMP* pRtmp,RTMP_AAC_ASC *pAacConfig);

int RTMP_AAC_SequenceHeader(RTMP* pRtmp,RTMP_AAC_ASC *pAacConfig,RTMPBuffer *pAudioBuffer);

/*
��sequence ���ó�Ĭ��ֵ
*/

int RTMP_Set_Default_Sequence(RTMP_AAC_ASC *pAacConfig);

void RTMP_Set_AAC_SequenceHeader(RTMP_AAC_ASC *pAacConfig,int channel , int FrequencyIndex, int samplesize,int profile);



