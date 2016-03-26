//
//  CameraSource.m
//  AVRecorder
//
//  Created by fernando on 16/3/25.
//  Copyright © 2016年 XCKanKan. All rights reserved.
//

#import "CameraSource.h"
#import <Foundation/Foundation.h>

@interface CameraSource()
{
    H264HwEncoderImpl *h264Encoder;
    AVCaptureSession *captureSession;
    AVCaptureVideoPreviewLayer *previewLayer;
    AVCaptureConnection* connection;
}
@end

@implementation CameraSource

+ (instancetype)shareInstance
{
    static CameraSource *camera = nil;
    
    if (camera == nil)
    {
        static dispatch_once_t token;
        dispatch_once(&token, ^{
            camera = [[CameraSource alloc] init];
        });
    }
    return camera;
}

- (void) startCamera:(UIView*)displayView
{
    // make input device
    NSError *deviceError;
    AVCaptureDevice *cameraDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    AVCaptureDeviceInput *inputDevice = [AVCaptureDeviceInput deviceInputWithDevice:cameraDevice error:&deviceError];
    
    // make output device
    AVCaptureVideoDataOutput *outputDevice = [[AVCaptureVideoDataOutput alloc] init];
    NSString* key = (NSString*)kCVPixelBufferPixelFormatTypeKey;
    NSNumber* val = [NSNumber numberWithUnsignedInt:kCVPixelFormatType_420YpCbCr8BiPlanarFullRange];
    NSDictionary* videoSettings = [NSDictionary dictionaryWithObject:val forKey:key];
    outputDevice.videoSettings = videoSettings;
    [outputDevice setSampleBufferDelegate:self queue:dispatch_get_main_queue()];
    
    // initialize capture session
    captureSession = [[AVCaptureSession alloc] init];
    [captureSession addInput:inputDevice];
    [captureSession addOutput:outputDevice];
    
    // begin configuration for the AVCaptureSession
    [captureSession beginConfiguration];
    [captureSession setSessionPreset:[NSString stringWithString:AVCaptureSessionPreset1280x720]];
    connection = [outputDevice connectionWithMediaType:AVMediaTypeVideo];
    [captureSession commitConfiguration];
    
    // make preview layer and add so that camera's view is displayed on screen
    previewLayer = [AVCaptureVideoPreviewLayer layerWithSession:captureSession];
    [previewLayer setVideoGravity:AVLayerVideoGravityResizeAspect];
    previewLayer.frame = displayView.bounds;
    [displayView.layer addSublayer:previewLayer];
    
    [captureSession startRunning];
    
    h264Encoder = [H264HwEncoderImpl shareInstance];
    if (h264Encoder)
    {
        [h264Encoder initEncode:1280 height:720];
    }
}

- (void) stopCamera
{
    [captureSession stopRunning];
    [previewLayer removeFromSuperlayer];
    
    if (h264Encoder)
    {
        [h264Encoder End];
    }
}

-(void) captureOutput:(AVCaptureOutput*)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection*)connection
{
    NSLog(@"frame captured at ");
    if (h264Encoder)
    {
        [h264Encoder encode:sampleBuffer];
    }
}

@end