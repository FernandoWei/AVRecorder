//
//  RTMPSession.m
//  AVRecorder
//
//  Created by fernando on 16/3/25.
//  Copyright © 2016年 XCKanKan. All rights reserved.
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
        });
    }
    
    return rtmpSession;
}

- (BOOL) setupRTMPStream:(NSString*) url
{
    //unique_ptr支持动态数组，而shared_ptr不能直接支持动态数组
    std::unique_ptr<char[]> url_c(new char[[url length] + 1]);
    //std::shared_ptr<char[]> url_c_shared(new char[4]);
    [url getCString:url_c.get() maxLength:[url length] + 1 encoding:NSUTF8StringEncoding];
    
    if (RTMP_SetupURL(_rtmp, url_c.get()) < 0)
    {
        NSLog(@"failed to setup url!");
        return NO;
    }
    
    if (RTMP_Connect(_rtmp, NULL) < 0)
    {
        NSLog(@"failed to connect to the url!");
        return NO;
    }
    
    if (RTMP_ConnectStream(_rtmp, 0) < 0)
    {
        NSLog(@"failed to connect to the stream");
        return NO;
    }
    
    return YES;
}

- (void) send:(NSMutableData*) data dataType:(int)type timestamp:(int)timestamp
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
    
    RTMP_SendPacket(_rtmp, &rtmpPkt, 0);
    RTMPPacket_Free(&rtmpPkt);
}

- (void) freeRTMPStream
{
    RTMP_Close(self->_rtmp);
    RTMP_Free(self->_rtmp);
}

@end