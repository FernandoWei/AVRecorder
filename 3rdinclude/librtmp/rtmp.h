#ifndef __RTMP_H__
#define __RTMP_H__
/*
 *      Copyright (C) 2005-2008 Team XBMC
 *      http://www.xbmc.org
 *      Copyright (C) 2008-2009 Andrej Stepanchuk
 *      Copyright (C) 2009-2010 Howard Chu
 *
 *  This file is part of librtmp.
 *
 *  librtmp is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 2.1,
 *  or (at your option) any later version.
 *
 *  librtmp is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with librtmp see the file COPYING.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 *  http://www.gnu.org/copyleft/lgpl.html
 */

#if !defined(NO_CRYPTO) && !defined(CRYPTO)
#define CRYPTO
#endif

#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <pthread.h>

#include "http.h"


#include "amf.h"
#include "cJSON.h"


#ifdef __cplusplus
extern "C"
{
#endif

#define RTMP_LIB_VERSION	0x020300	/* 2.3 */

#define RTMP_FEATURE_HTTP	0x01
#define RTMP_FEATURE_ENC	0x02
#define RTMP_FEATURE_SSL	0x04
#define RTMP_FEATURE_MFP	0x08	/* not yet supported */
#define RTMP_FEATURE_WRITE	0x10	/* publish, not play */
#define RTMP_FEATURE_HTTP2	0x20	/* server-side rtmpt */
#define RTMP_FEATURE_UDT	0x30    /* UDT rtmp*/

#define RTMP_PROTOCOL_UNDEFINED	-1
#define RTMP_PROTOCOL_RTMP      0
#define RTMP_PROTOCOL_RTMPE     RTMP_FEATURE_ENC
#define RTMP_PROTOCOL_RTMPT     RTMP_FEATURE_HTTP
#define RTMP_PROTOCOL_RTMPS     RTMP_FEATURE_SSL
#define RTMP_PROTOCOL_RTMPTE    (RTMP_FEATURE_HTTP|RTMP_FEATURE_ENC)
#define RTMP_PROTOCOL_RTMPTS    (RTMP_FEATURE_HTTP|RTMP_FEATURE_SSL)
#define RTMP_PROTOCOL_RTMFP     RTMP_FEATURE_MFP


#define RTMP_DEFAULT_CHUNKSIZE	128

/* needs to fit largest number of bytes recv() may return */
#define RTMP_BUFFER_CACHE_SIZE (16*1024)

#define	RTMP_CHANNELS	65600

  extern const char RTMPProtocolStringsLower[][7];
  extern const AVal RTMP_DefaultFlashVer;
  extern int RTMP_ctrlC;

  uint32_t RTMP_GetTime(void);

/*      RTMP_PACKET_TYPE_...                0x00 */
#define RTMP_PACKET_TYPE_CHUNK_SIZE         0x01
/*      RTMP_PACKET_TYPE_...                0x02 */
#define RTMP_PACKET_TYPE_BYTES_READ_REPORT  0x03
#define RTMP_PACKET_TYPE_CONTROL            0x04
#define RTMP_PACKET_TYPE_SERVER_BW          0x05
#define RTMP_PACKET_TYPE_CLIENT_BW          0x06
/*      RTMP_PACKET_TYPE_...                0x07 */
#define RTMP_PACKET_TYPE_AUDIO              0x08
#define RTMP_PACKET_TYPE_VIDEO              0x09
/*      RTMP_PACKET_TYPE_...                0x0A */
/*      RTMP_PACKET_TYPE_...                0x0B */
/*      RTMP_PACKET_TYPE_...                0x0C */
/*      RTMP_PACKET_TYPE_...                0x0D */
/*      RTMP_PACKET_TYPE_...                0x0E */
#define RTMP_PACKET_TYPE_FLEX_STREAM_SEND   0x0F
#define RTMP_PACKET_TYPE_FLEX_SHARED_OBJECT 0x10
#define RTMP_PACKET_TYPE_FLEX_MESSAGE       0x11
#define RTMP_PACKET_TYPE_INFO               0x12
#define RTMP_PACKET_TYPE_SHARED_OBJECT      0x13
#define RTMP_PACKET_TYPE_INVOKE             0x14
/*      RTMP_PACKET_TYPE_...                0x15 */
#define RTMP_PACKET_TYPE_FLASH_VIDEO        0x16

#define RTMP_MAX_HEADER_SIZE 18

#define RTMP_PACKET_SIZE_LARGE    0
#define RTMP_PACKET_SIZE_MEDIUM   1
#define RTMP_PACKET_SIZE_SMALL    2
#define RTMP_PACKET_SIZE_MINIMUM  3

  typedef struct RTMPChunk
  {
    int c_headerSize;
    int c_chunkSize;
    char *c_chunk;
    char c_header[RTMP_MAX_HEADER_SIZE];
  } RTMPChunk;

  typedef struct RTMPPacket
  {
    uint8_t m_headerType;
    uint8_t m_packetType;
    uint8_t m_hasAbsTimestamp;	/* timestamp absolute or relative? */
    int m_nChannel;
    uint32_t m_nTimeStamp;	/* timestamp */
    int32_t m_nInfoField2;	/* last 4 bytes in a long header */
    uint32_t m_nBodySize;
    uint32_t m_nBytesRead;
    RTMPChunk *m_chunk;
    char *m_body;
  } RTMPPacket;

  typedef struct RTMPSockBuf
  {
    int sb_socket;
    int sb_size;		/* number of unprocessed bytes in buffer */
    char *sb_start;		/* pointer into sb_pBuffer of next byte to process */
    char sb_buf[RTMP_BUFFER_CACHE_SIZE];	/* data read from socket */
    int sb_timedout;
    void *sb_ssl;
  } RTMPSockBuf;
/////////////////////////////////////////////////////add begin
//add by zongchunli for rtmp buffer control

/*
sps pps½âÂë³öµÄÒ»Ð©ÐÅÏ¢
*/


//¶¨Òå°üÍ·³¤¶È£¬RTMP_MAX_HEADER_SIZE=18
#define RTMP_HEAD_SIZE   (sizeof(RTMPPacket)+RTMP_MAX_HEADER_SIZE)
//#define RTMP_HEAD_SIZE   (RTMP_MAX_HEADER_SIZE)



typedef struct RTMP_SPS_PPS
{
 //unsigned int nWidth;
 //unsigned int nHeight;
 //unsigned int nFrameRate;
 unsigned int nSPSLen;
 unsigned char * sSPS;
 unsigned int nPPSLen;
 unsigned char * sPPS;
}RTMP_SPS_PPS;


typedef struct RTMPMetadata  
{  
	// video, must be h264 type   
	uint8_t			bHasVideo;
	unsigned int    nWidth;  
	unsigned int    nHeight;  
	unsigned int    nFrameRate;     // fps   
	unsigned int    nVideoDataRate; // bps   
 

	// audio, must be aac type   
	uint8_t            bHasAudio;  
	unsigned int    nAudioSampleRate;  
	unsigned int    nAudioSampleSize;  
	unsigned int    nAudioChannels;  
	char            pAudioSpecCfg;  
	unsigned int    nAudioSpecCfgLen;  
	unsigned int	nAudioObjType;
}RTMPMetadata; 



typedef struct RTMPBuffer_Node
{
 	RTMPPacket *pPacket;
	int nFrame_type;//µ±Ç°packetÀàÐÍ¡£0: p frame.  1: I frame
 	struct RTMPBuffer_Node *next;

}RTMPBuffer_Node;

typedef struct RTMPBuffer
{
	RTMPBuffer_Node *pBufferNode;//packet buffer
	RTMPBuffer_Node *pLastNode;
	RTMP_SPS_PPS pSPSPPSData;
	uint8_t pAacConfig;
	uint32_t iLast_i_frame_time;   //the last i frame time
	
	RTMPPacket *pPacketConfig;   //sequence config packet
	
	uint32_t iBeginTime;          //¿ªÊ¼Ê±¼ä,Ãë
	
	struct timeval t_start;       //¿ªÊ¼Ê±¼ä£¬¾«È·µ½ºÁÃë
	
	pthread_mutex_t lock;         //¿ØÖÆbufferÊý¾ÝµÄ¶ÁÈ¡

	

	//ºóÃæËÄ¸ö²ÎÊýÓÃÓÚ×Ô¶¯½µµÍÂëÂÊÊ¹ÓÃ
	uint32_t max_bitrate;  //¸Ã buffer ¶ÔÓ¦µÄÁ÷ÔÊÐíµÄ×î´óÂëÂÊ

	uint32_t min_bitrate;  // ¸Ã buffer ¶ÔÓ¦µÄÁ÷ÔÊÐíµÄ×îÐ¡ÂëÂÊ

	uint32_t want_bitrate; //¸Ã buffer ¶ÔÓ¦µÄÁ÷Ï£ÍûµÄÂëÂÊ

	uint32_t delay_count; //¸Ãstream ·¢ÉúdelayµÄ´ÎÊý

	int packet_count;
 	
}RTMPBuffer;




typedef struct RTMP_ADTS_HEADER
{
	unsigned int syncword; 
	unsigned int id;       
	unsigned int layer;    
	unsigned int protection_absent; 
	unsigned int profile;          
	unsigned int sf_index;         
	unsigned int private_bit;      
	unsigned int channel_configuration;  
	unsigned int original;             
	unsigned int home;                  
	
	unsigned int copyright_identification_bit;  
	unsigned int copyright_identification_start; 
	unsigned int aac_frame_length;               
	unsigned int adts_buffer_fullness;  
	unsigned int no_raw_data_blocks_in_frame;
	
}RTMP_ADTS_HEADER;
typedef struct RTMP_AAC_ASC
{
	unsigned int nSoundFormat;
	unsigned int nSoundRate;
	unsigned int nSoundSize;
	unsigned int nSoundType;
	unsigned int nSoundProfile;
	unsigned int nSoundFrequencyIndex;
	unsigned int nChannelConfiguration;
	
}RTMP_AAC_ASC;


//portal
#define	AGENT_RTMP	"Mozilla/5.0"
#define CENTER_PORTAL_FAIL_COUNT 1
#define RTMP_URL_CENTER_PORTAL "192.168.64.234:1939"



typedef struct CDN_NODE{
  char name[100];
  char key[100];
  int use;
  int fail_num;
}CDN_NODE;



typedef struct RTMP_PORTAL{
 char url[200];
 char url_center_portal[200];
 int time_useful;//urlÓÐÐ§Ê±¼ä
 int begin_time;
 int icdn_num;
 CDN_NODE **cdn;
 int center_request_num;//µÚ¼¸´ÎÇëÇóÖÐÐÄportal
 char area_fail_info[50];//ÇøÓòportalÊ§°ÜÐÅÏ¢·¢ËÍ¸øÖÐÐÄportal
 int cdn_type;
 
}RTMP_PORTAL;

typedef struct RTMP_Static
  {
      uint64_t i_request_portal_time;
      uint64_t i_recv_portal_time;
      uint64_t i_begin_play_time;
      uint64_t i_stop_play_time;
      int i_reconnect_rtmp_count;
      int i_buffer_count;
      int i_liveid;
      int i_channel_id;
      char s_peerid[17];
      char s_portal_url[500];
      char s_rtmp_url[500];
      int i_request_portal_count;
      char s_guid[33];
      char i_userid;
      int i_play_type;// p2p or p2s
      int i_definition;//清晰度
      int i_send_type;//0:begin 1:end
      int i_live_mobile;//0:android  3:ios
      int i_reconnect_portal_count;



  }RTMP_Static;
  //RTMP_Static rtmp_static;
  void Rtmp_Static_Init(RTMP_Static *rtmp_static);
  int Send_Static(RTMP_Static *rtmp_static);
  void Get_guid(char * guid);
  uint64_t RTMP_GetTime_mill();

void init_portal(RTMP_PORTAL *rtmp_url);



HTTPResult RTMP_HTTP_get(struct HTTP_ctx *http, const char *url);


int request_portal(char *url,RTMP_PORTAL *rtmp_url);

int request_url( char *url,RTMP_PORTAL *center_portal,RTMP_PORTAL *area_portal,RTMP_Static *rtmp_static);




int RTMP_ParseURL_kk(char * url,AVal *real_url, int *tmode,int *cmode);





//portal end

uint32_t RTMP_GetTime_m(struct timeval t_start);
uint32_t RTMP_GetTime_print();

void RTMP_InitMetadata(RTMPMetadata *lpMetaData,int hasaudio,int hasvideo,int audio_channel,int audio_sample,int audio_samplesize,
			int video_framerate,int video_width,int video_height,int video_bitrate,int audio_bitrate);

#define LOGE(format, ...)  printf("(>_<) " format "\n", ##__VA_ARGS__)
#define LOGI(format, ...)  printf("(^_^) " format "\n", ##__VA_ARGS__)


///////////////////////////////////////////////////////add end
  void RTMPPacket_Reset(RTMPPacket *p);
  void RTMPPacket_Dump(RTMPPacket *p);
  int RTMPPacket_Alloc(RTMPPacket *p, int nSize);
  void RTMPPacket_Free(RTMPPacket *p);

  void RTMP_Packet_Free(RTMPPacket *p);

#define RTMPPacket_IsReady(a)	((a)->m_nBytesRead == (a)->m_nBodySize)

  typedef struct RTMP_LNK
  {
    AVal hostname;
    AVal sockshost;

    AVal playpath0;	/* parsed from URL */
    AVal playpath;	/* passed in explicitly */
    AVal tcUrl;
    AVal swfUrl;
    AVal pageUrl;
    AVal app;
    AVal auth;
    AVal flashVer;
    AVal subscribepath;
    AVal usherToken;
    AVal token;
    AVal pubUser;
    AVal pubPasswd;
    AMFObject extras;
    int edepth;

    int seekTime;
    int stopTime;

#define RTMP_LF_AUTH	0x0001	/* using auth param */
#define RTMP_LF_LIVE	0x0002	/* stream is live */
#define RTMP_LF_SWFV	0x0004	/* do SWF verification */
#define RTMP_LF_PLST	0x0008	/* send playlist before play */
#define RTMP_LF_BUFX	0x0010	/* toggle stream on BufferEmpty msg */
#define RTMP_LF_FTCU	0x0020	/* free tcUrl on close */
#define RTMP_LF_FAPU	0x0040	/* free app on close */
    int lFlags;

    int swfAge;

    int protocol;
    int timeout;		/* connection timeout in seconds */

    int pFlags;			/* unused, but kept to avoid breaking ABI */

    unsigned short socksport;
    unsigned short port;

#ifdef CRYPTO
#define RTMP_SWF_HASHLEN	32
    void *dh;			/* for encryption */
    void *rc4keyIn;
    void *rc4keyOut;

    uint32_t SWFSize;
    uint8_t SWFHash[RTMP_SWF_HASHLEN];
    char SWFVerificationResponse[RTMP_SWF_HASHLEN+10];
#endif
  } RTMP_LNK;

  /* state for read() wrapper */
  typedef struct RTMP_READ
  {
    char *buf;
    char *bufpos;
    unsigned int buflen;
    uint32_t timestamp;
    uint8_t dataType;
    uint8_t flags;
#define RTMP_READ_HEADER	0x01
#define RTMP_READ_RESUME	0x02
#define RTMP_READ_NO_IGNORE	0x04
#define RTMP_READ_GOTKF		0x08
#define RTMP_READ_GOTFLVK	0x10
#define RTMP_READ_SEEKING	0x20
    int8_t status;
#define RTMP_READ_COMPLETE	-3
#define RTMP_READ_ERROR	-2
#define RTMP_READ_EOF	-1
#define RTMP_READ_IGNORE	0

    /* if bResume == TRUE */
    uint8_t initialFrameType;
    uint32_t nResumeTS;
    char *metaHeader;
    char *initialFrame;
    uint32_t nMetaHeaderSize;
    uint32_t nInitialFrameSize;
    uint32_t nIgnoredFrameCounter;
    uint32_t nIgnoredFlvFrameCounter;
  } RTMP_READ;

  typedef struct RTMP_METHOD
  {
    AVal name;
    int num;
  } RTMP_METHOD;

  typedef struct RTMP_AVIOInterruptCB {
	  int (*callback)(void*);
	  void *opaque;
  } RTMP_AVIOInterruptCB;

  int ff_check_interrupt_rtmp(RTMP_AVIOInterruptCB *cb);
  

  typedef struct RTMP
  {
    int m_inChunkSize;
    int m_outChunkSize;
    int m_nBWCheckCounter;
    int m_nBytesIn;
    int m_nBytesInSent;
    int m_nBufferMS;
    int m_stream_id;		/* returned in _result from createStream */
    int m_mediaChannel;
    uint32_t m_mediaStamp;
    uint32_t m_pauseStamp;
    int m_pausing;
    int m_nServerBW;
    int m_nClientBW;
    uint8_t m_nClientBW2;
    uint8_t m_bPlaying;
    uint8_t m_bSendEncoding;
    uint8_t m_bSendCounter;

    int m_numInvokes;
    int m_numCalls;
    RTMP_METHOD *m_methodCalls;	/* remote method calls queue */

    int m_channelsAllocatedIn;
    int m_channelsAllocatedOut;
    RTMPPacket **m_vecChannelsIn;
    RTMPPacket **m_vecChannelsOut;
    int *m_channelTimestamp;	/* abs timestamp of last packet */

    double m_fAudioCodecs;	/* audioCodecs for the connect packet */
    double m_fVideoCodecs;	/* videoCodecs for the connect packet */
    double m_fEncoding;		/* AMF0 or AMF3 */

    double m_fDuration;		/* duration of stream in seconds */

    int m_msgCounter;		/* RTMPT stuff */
    int m_polling;
    int m_resplen;
    int m_unackd;
    AVal m_clientID;

    RTMP_READ m_read;
    RTMPPacket m_write;
    RTMPSockBuf m_sb;
    RTMP_LNK Link;

	//protocl UDT or TCP,add by zongchunli
	int m_udt_tcp; // 0:TCP  1:UDT
	int m_portal;//ÊÇ·ñ²ÉÓÃportal
	RTMP_PORTAL *center_portal;
	RTMP_PORTAL *area_portal;
	RTMP_AVIOInterruptCB interrupt_callback;
  RTMP_Static rtmp_static;
 	
  } RTMP;
/*
 * add by zongchunli
*/
 void RTMP_SetUDT(RTMP *r,int iUdt);
  typedef struct BufferConfig
  {
	  RTMP * pRTMP;
	  RTMPBuffer *pVideoBuffer;
	  RTMPBuffer *pAudioBuffer;
	  int state;//ÓÃÓÚ¿ØÖÆ³ÌÐò½áÊøµÄ±êÖ¾Î»
	  uint32_t iMaxDelay;//×î´óÑÓ³ÙÊ±¼ä
	  uint32_t iWaitTime;//bufferÎª¿ÕÊ±µÄµÈ´ýÊ±¼ä
	  int send_video_type;//ÊÓÆµ·¢ËÍ·½Ê½¡£Ä¬ÈÏÎª1. 1:ËùÓÐµÄÖ¡¶¼·¢ËÍ¡£0:²»·¢ËÍvideo packet
	  uint32_t iReSend_Window_Time;//ÊÓÆµÖÕ¶Ë¿ªÊ¼ÖØÐÂ·¢ËÍvideoÊ±µÄÅÐ¶Ï´°¿Ú
	 
	  
  }BufferConfig;

////////////////////////////////////////////

  int RTMP_ParseURL(const char *url, int *protocol, AVal *host,
		     unsigned int *port, AVal *playpath, AVal *app);

  void RTMP_ParsePlaypath(AVal *in, AVal *out);
  void RTMP_SetBufferMS(RTMP *r, int size);
  void RTMP_UpdateBufferMS(RTMP *r);

  int RTMP_SetOpt(RTMP *r, const AVal *opt, AVal *arg);
  int RTMP_SetupURL(RTMP *r, char *url);
  void RTMP_SetupStream(RTMP *r, int protocol,
			AVal *hostname,
			unsigned int port,
			AVal *sockshost,
			AVal *playpath,
			AVal *tcUrl,
			AVal *swfUrl,
			AVal *pageUrl,
			AVal *app,
			AVal *auth,
			AVal *swfSHA256Hash,
			uint32_t swfSize,
			AVal *flashVer,
			AVal *subscribepath,
			AVal *usherToken,
			int dStart,
			int dStop, int bLiveStream, long int timeout);

  int RTMP_Connect(RTMP *r, RTMPPacket *cp);
  struct sockaddr;
  int RTMP_Connect0(RTMP *r, struct sockaddr *svc);
  int RTMP_Connect1(RTMP *r, RTMPPacket *cp);
  int RTMP_Serve(RTMP *r);
  int RTMP_TLS_Accept(RTMP *r, void *ctx);

  int RTMP_ReadPacket(RTMP *r, RTMPPacket *packet);
  int RTMP_SendPacket(RTMP *r, RTMPPacket *packet, int queue);
  int RTMP_SendChunk(RTMP *r, RTMPChunk *chunk);
  int RTMP_IsConnected(RTMP *r);
  int RTMP_Socket(RTMP *r);
  int RTMP_IsTimedout(RTMP *r);
  double RTMP_GetDuration(RTMP *r);
  int RTMP_ToggleStream(RTMP *r);

  int RTMP_ConnectStream(RTMP *r, int seekTime);
  int RTMP_ReconnectStream(RTMP *r, int seekTime);
  void RTMP_DeleteStream(RTMP *r);
  int RTMP_GetNextMediaPacket(RTMP *r, RTMPPacket *packet);
  int RTMP_ClientPacket(RTMP *r, RTMPPacket *packet);

  void RTMP_Init(RTMP *r);
  void RTMP_Close(RTMP *r);
  RTMP *RTMP_Alloc(void);
  void RTMP_Free(RTMP *r);
  void RTMP_EnableWrite(RTMP *r);

  void *RTMP_TLS_AllocServerContext(const char* cert, const char* key);
  void RTMP_TLS_FreeServerContext(void *ctx);

  int RTMP_LibVersion(void);
  void RTMP_UserInterrupt(void);	/* user typed Ctrl-C */

  int RTMP_SendCtrl(RTMP *r, short nType, unsigned int nObject,
		     unsigned int nTime);

  /* caller probably doesn't know current timestamp, should
   * just use RTMP_Pause instead
   */
  int RTMP_SendPause(RTMP *r, int DoPause, int dTime);
  int RTMP_Pause(RTMP *r, int DoPause);

  int RTMP_FindFirstMatchingProperty(AMFObject *obj, const AVal *name,
				      AMFObjectProperty * p);

  int RTMPSockBuf_Fill(RTMPSockBuf *sb,int udt_tcp);
  int RTMPSockBuf_Send(RTMPSockBuf *sb, const char *buf, int len);
  int RTMPSockBuf_Close(RTMPSockBuf *sb,int udt_tcp);

  int RTMP_SendCreateStream(RTMP *r);
  int RTMP_SendSeek(RTMP *r, int dTime);
  int RTMP_SendServerBW(RTMP *r);
  int RTMP_SendClientBW(RTMP *r);
  void RTMP_DropRequest(RTMP *r, int i, int freeit);
  int RTMP_Read(RTMP *r, char *buf, int size);
  int RTMP_Write(RTMP *r, const char *buf, int size);

/* hashswf.c */
  int RTMP_HashSWF(const char *url, unsigned int *size, unsigned char *hash,
		   int age);

int reconnect(RTMP *r,int status);

void free_portal(RTMP *r);


#ifdef __cplusplus
};
#endif

#endif
