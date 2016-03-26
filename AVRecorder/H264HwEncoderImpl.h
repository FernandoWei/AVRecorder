//
//  H264HwEncoderImpl.h
//  h264v1
//
//  Created by Ganvir, Manish on 3/31/15.
//  Copyright (c) 2015 Ganvir, Manish. All rights reserved.
//

#import <Foundation/Foundation.h>

@import AVFoundation;
@interface H264HwEncoderImpl : NSObject
+ (instancetype)shareInstance;
- (void) initEncode:(int)width  height:(int)height;
- (void) changeResolution:(int)width  height:(int)height;
- (void) encode:(CMSampleBufferRef )sampleBuffer;
- (void) End;

@property (weak, nonatomic) NSString *error;

@end
