
/*
add by zongchunli@kankan.com

*/

#include <stdio.h>
#include <stdlib.h>

#include "rtmp.h"
#include "rtmp_aac.h"
#include "rtmp_buffer.h"
#include "Rtmp_H264.h"
#include "amf.h"

#include "thread.h"

//#define MAX_INT 1000000
//#define WAIT_TIME 5000




TFTYPE BufferThread(void *arg);


int Buffer_Control(BufferConfig *pConfig);

void Set_Max_Delay(BufferConfig *pConfig,uint32_t iDelayTime);

void Set_Wait_Time(BufferConfig *pConfig,uint32_t iWaitTime);


