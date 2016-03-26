#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>


#include "rtmp.h"
#include "rtmp_sys.h"


#define FLV_CODECID_H264 7

#define FLV_CODECID_AAC  10


char * put_bytes(char *c,char *bytes,int len);

char * put_byte( char *output, uint8_t nVal )  ;


char * put_be16(char *output, uint16_t nVal ) ;

char * put_be24(char *output,uint32_t nVal );

char * put_be32(char *output, uint32_t nVal );

char *  put_be64( char *output, uint64_t nVal );


char * put_amf_bool(char *c, uint8_t b);

char * put_amf_string( char *c, const char *str ) ;

char * put_amf_double( char *c, double d ) ;

int SendMetadataPacket(RTMP *pRtmp ,RTMPMetadata* lpMetaData);







