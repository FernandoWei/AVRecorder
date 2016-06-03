//
//  RTMPSession.m
//  AVRecorder
//
//  Created by fernando on 16/3/25.
//  Copyright © 2016年 fernando All rights reserved.
//

#import "RTMPSession.h"
#include <memory>
#import <Foundation/Foundation.h>


@interface RTMPSession()
{
    RTMP* _rtmp;
}

@end

@implementation RTMPSession

+ (instancetype)shareInstance
{
    static RTMPSession* rtmpSession = nil;
    if (rtmpSession == nil)
    {
        static dispatch_once_t token;
        dispatch_once(&token, ^{
            rtmpSession = [[RTMPSession alloc] init];
            rtmpSession->_rtmp = RTMP_Alloc();
            RTMP_Init(rtmpSession->_rtmp);
            rtmpSession->_rtmp->Link.timeout = 5;
        });
    }
    
    return rtmpSession;
}

- (BOOL) setupRTMPStream:(NSString*) url
{
    if (!RTMP_SetupURL(_rtmp, (char*)[url UTF8String]))
//    char *strUrl = (char *)[url cStringUsingEncoding:NSASCIIStringEncoding];
//    if (!RTMP_SetupURL(_rtmp, strUrl))
    {
        NSLog(@"failed to setup url!");
        return NO;
    }
    
    RTMP_EnableWrite(_rtmp);

    if (!RTMP_Connect(_rtmp, NULL))
    {
        NSLog(@"failed to connect to the url!");
        return NO;
    }
    
    if (!RTMP_ConnectStream(_rtmp, 0))
    {
        NSLog(@"failed to connect to the stream");
        return NO;
    }
    
    return YES;
}

- (void) send:(NSMutableData*) data dataType:(int)type timestamp:(uint32_t)timestamp
{
    RTMPPacket rtmpPkt;
    RTMPPacket_Reset(&rtmpPkt);
    RTMPPacket_Alloc(&rtmpPkt, data.length);
    
    rtmpPkt.m_packetType = type;
    rtmpPkt.m_nBodySize = data.length;
    rtmpPkt.m_nTimeStamp = timestamp;
    rtmpPkt.m_headerType = RTMP_PACKET_SIZE_LARGE;
    rtmpPkt.m_nChannel = 4;
    rtmpPkt.m_nInfoField2 = self->_rtmp->m_stream_id;
    memcpy(rtmpPkt.m_body, [data mutableBytes], [data length]);
    
    if (!RTMP_IsConnected(_rtmp)){
        NSLog(@"rtmp is not connected!");
    }
    
    if (!RTMP_SendPacket(_rtmp, &rtmpPkt, 0)){
        NSLog(@"failed to send data");
    }
    RTMPPacket_Free(&rtmpPkt);
}

- (void) freeRTMPStream
{
    RTMP_Close(self->_rtmp);
    RTMP_Free(self->_rtmp);
}

@end