//
//  RTMPSession.h
//  AVRecorder
//
//  Created by fernando on 16/3/25.
//  Copyright © 2016年 XCKanKan. All rights reserved.
//

#ifndef RTMPSession_h
#define RTMPSession_h

#import "rtmp.h"
#import <Foundation/Foundation.h>

@interface RTMPSession : NSObject

+ (instancetype) shareInstance;
- (BOOL) setupRTMPStream:(NSString*) url;
- (void) send:(NSMutableData*) data dataType:(int)type timestamp:(int)timestamp;
- (void) freeRTMPStream;

@end

#endif /* RTMPSession_h */
