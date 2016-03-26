//
//  H264HwEncoderImpl.m
//  h264v1
//
//  Created by Ganvir, Manish on 3/31/15.
//  Copyright (c) 2015 Ganvir, Manish. All rights reserved.
//

#import "H264HwEncoderImpl.h"
#define YUV_FRAME_SIZE 2000
#define FRAME_WIDTH
#define NUMBEROFRAMES 300
#define DURATION 12

@import VideoToolbox;
@import AVFoundation;

@implementation H264HwEncoderImpl
{
    VTCompressionSessionRef EncodingSession;
    dispatch_queue_t aQueue;
    CMFormatDescriptionRef  format;
    CMSampleTimingInfo * timingInfo;
    BOOL initialized;
    int  frameCount;
    NSMutableData *sps;
    NSMutableData *pps;
    bool sentConfig;
}
@synthesize error;

+ (instancetype) shareInstance
{
    static H264HwEncoderImpl* encoder = nil;
    if (encoder == nil)
    {
        static dispatch_once_t token;
        dispatch_once(&token, ^{
            encoder = [[H264HwEncoderImpl alloc] init];
        });
    }
    
    return encoder;
}

- (void) initWithConfiguration
{
    EncodingSession = nil;
    initialized = true;
    aQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    frameCount = 0;
    sps = NULL;
    pps = NULL;
    sentConfig = false;
    
}

//回调函数
void didCompressH264(void *outputCallbackRefCon, void *sourceFrameRefCon, OSStatus status, VTEncodeInfoFlags infoFlags, CMSampleBufferRef sampleBuffer )
{
    //CMBlockBufferRef block = CMSampleBufferGetDataBuffer(sampleBuffer);
    //CFArrayRef attachments = CMSampleBufferGetSampleAttachmentsArray(sampleBuffer, false);
//    CMTime pts = CMSampleBufferGetPresentationTimeStamp(sampleBuffer);
//    CMTime dts = CMSampleBufferGetDecodeTimeStamp(sampleBuffer);
    
    
    NSLog(@"didCompressH264 called with status %d infoFlags %d", (int)status, (int)infoFlags);
    if (status != 0) return;
    
    if (!CMSampleBufferDataIsReady(sampleBuffer))
    {
        NSLog(@"didCompressH264 data is not ready ");
        return;
    }
    
   H264HwEncoderImpl* encoder = (__bridge H264HwEncoderImpl*)outputCallbackRefCon;
   
   // Check if we have got a key frame first
    bool keyframe = !CFDictionaryContainsKey( (CFArrayGetValueAtIndex(CMSampleBufferGetSampleAttachmentsArray(sampleBuffer, true), 0)), kCMSampleAttachmentKey_NotSync);
    
   if (keyframe)
   {
        CMFormatDescriptionRef format = CMSampleBufferGetFormatDescription(sampleBuffer);
        size_t sparameterSetSize, sparameterSetCount;
        const uint8_t *sparameterSet;
        OSStatus statusCode = CMVideoFormatDescriptionGetH264ParameterSetAtIndex(format, 0, &sparameterSet, &sparameterSetSize, &sparameterSetCount, 0 );
        if (statusCode == noErr)
        {
            // Found sps and now check for pps
            size_t pparameterSetSize, pparameterSetCount;
            const uint8_t *pparameterSet;
            OSStatus statusCode = CMVideoFormatDescriptionGetH264ParameterSetAtIndex(format, 1, &pparameterSet, &pparameterSetSize, &pparameterSetCount, 0 );
            if (statusCode == noErr)
            {
                encoder->sps = [NSMutableData dataWithBytes:sparameterSet length:sparameterSetSize];
                encoder->pps = [NSMutableData dataWithBytes:pparameterSet length:pparameterSetSize];
//                
                [encoder dataPacketizer:encoder->sps keyFrame: true];
                [encoder dataPacketizer:encoder->pps keyFrame: true];
            }
        }
    }
    
    //从sample buffer中拿到block buffer
    CMBlockBufferRef dataBuffer = CMSampleBufferGetDataBuffer(sampleBuffer);
    size_t length;
    char *dataPointer;
    CMBlockBufferGetDataPointer(dataBuffer, 0, 0, &length, &dataPointer);
    
    NSMutableData* data = [[NSMutableData alloc] initWithBytes:dataPointer length:length];
    [encoder dataPacketizer:data keyFrame: false];
}

//要使用大端模式存储数据
- (void) dataPacketizer:(NSMutableData*)data keyFrame:(bool)key
{
    NSMutableData* tagData = [NSMutableData dataWithLength:5];  //前五个字节为固定长度
    uint8_t frameType_codecId = 0x27;
    [tagData replaceBytesInRange:NSMakeRange(0, 1) withBytes:&frameType_codecId length:1];
    uint8_t avcc = 0x01;
    [tagData replaceBytesInRange:NSMakeRange(1, 1) withBytes:&avcc length:1];
    if (key && !sentConfig)
    {
        if (self->sps.length > 0 && self->pps.length > 0)
        {
            frameType_codecId = 0x17;
            [tagData replaceBytesInRange:NSMakeRange(0, 1) withBytes:&frameType_codecId length:1];
            
            //组装AVC sequence header
            uint8_t version = 0x01;
            [tagData appendBytes:&version length:1];
            [tagData appendBytes:[self->sps mutableBytes] + 1 length:3]; // sps[1], sps[2], sps[3]
            uint8_t num = 0xff;  // 6 bits reserved + 2 bits nal size length - 1 (11)
            [tagData appendBytes:&num length:1];
            num = 0xe1; // 3 bits reserved (111) + 5 bits number of sps (00001)
            [tagData appendBytes:&num length:1];
            uint16_t spsSize = self->sps.length;
            uint8_t size[2] = {((spsSize >> 8) & 0xff), (spsSize & 0xff)};
            [tagData appendBytes:&size[0] length:2];
            [tagData appendBytes:[self->sps mutableBytes] length:self->sps.length];
            num = 0x01; //pps 个数
            [tagData appendBytes:&num length:1];
            uint16_t ppsSize = self->pps.length;
            size[0] = (ppsSize >> 8) & 0xff;
            size[1] = ppsSize & 0xff;
            [tagData appendBytes:&size[0] length:2];
            [tagData appendBytes:[self->pps mutableBytes] length:self->pps.length];
            
            //发送出去
            // to do with rtmp
            
            //AVC sequence header只需要发送一次
            sentConfig = true;
        }
        else
        {
            return;
        }
    }
    
    [tagData appendData:data];
    
    //发送出去
    // to do with rtmp
}

- (void) initEncode:(int)width  height:(int)height
{
    [self initWithConfiguration];
    dispatch_sync(aQueue, ^{
        // Create the compression session
        OSStatus status = VTCompressionSessionCreate(NULL, width, height, kCMVideoCodecType_H264, NULL, NULL, NULL, didCompressH264, (__bridge void *)(self),  &EncodingSession);
        NSLog(@"H264: VTCompressionSessionCreate %d", (int)status);
        
        if (status != 0)
        {
            NSLog(@"H264: Unable to create a H264 session");
            error = @"H264: Unable to create a H264 session";
            
            return ;
        }
        
        // Set the properties
        VTSessionSetProperty(EncodingSession, kVTCompressionPropertyKey_RealTime, kCFBooleanTrue);
        //VTSessionSetProperty(EncodingSession, kVTCompressionPropertyKey_MaxKeyFrameInterval, 240);
        VTSessionSetProperty(EncodingSession, kVTCompressionPropertyKey_ProfileLevel, kVTProfileLevel_H264_Baseline_AutoLevel);
        VTSessionSetProperty(EncodingSession, kVTCompressionPropertyKey_AllowFrameReordering, kCFBooleanFalse);

        // Tell the encoder to start encoding
        VTCompressionSessionPrepareToEncodeFrames(EncodingSession);
    });
}
- (void) encode:(CMSampleBufferRef )sampleBuffer
{
     dispatch_sync(aQueue, ^{
        
          frameCount++;
            // Get the CV Image buffer
            CVImageBufferRef imageBuffer = (CVImageBufferRef)CMSampleBufferGetImageBuffer(sampleBuffer);
            
            // Create properties
            CMTime presentationTimeStamp = CMTimeMake(frameCount, 1000);  //单位是毫秒，1000在这里表示是一秒的多少分之一
         CMTime duration = CMTimeMake(1, 20);
            //CMTime duration = CMTimeMake(1, DURATION);
            VTEncodeInfoFlags flags;
            
            // Pass it to the encoder
            OSStatus statusCode = VTCompressionSessionEncodeFrame(EncodingSession,
                                                         imageBuffer,
                                                         presentationTimeStamp,
                                                         duration,
                                                         NULL, NULL, &flags);
            // Check for error
            if (statusCode != noErr) {
                NSLog(@"H264: VTCompressionSessionEncodeFrame failed with %d", (int)statusCode);
                error = @"H264: VTCompressionSessionEncodeFrame failed ";
                
                // End the session
                VTCompressionSessionInvalidate(EncodingSession);
                CFRelease(EncodingSession);
                EncodingSession = NULL;
                error = NULL;
                return;
            }
            NSLog(@"H264: VTCompressionSessionEncodeFrame Success");
       });
    
    
}
- (void) changeResolution:(int)width  height:(int)height
{
}

- (void) End
{
    // Mark the completion
    VTCompressionSessionCompleteFrames(EncodingSession, kCMTimeInvalid);
    
    // End the session
    VTCompressionSessionInvalidate(EncodingSession);
    CFRelease(EncodingSession);
    EncodingSession = NULL;
    error = NULL;

}

@end
