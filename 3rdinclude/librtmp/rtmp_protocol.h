#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rtmp.h"
#include "rtmp_sys.h"
#include "rtmp_buffer.h"


/*
 建立与服务器端的rtmp连接
*/


int RTMP_Connect_kk(RTMP *pRtmp,const char * url);

/** 
* 断开连接，释放相关的资源。 *
*/   

void RTMP_Close_kk(RTMP *pRtmp);


