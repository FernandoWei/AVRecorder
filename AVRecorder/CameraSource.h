//
//  CameraSource.h
//  AVRecorder
//
//  Created by fernando on 16/3/25.
//  Copyright © 2016年 XCKanKan. All rights reserved.
//

#ifndef CameraSource_h
#define CameraSource_h

#import "H264HwEncoderImpl.h"
#import <UIKit/UIKit.h>

@interface CameraSource: NSObject  <AVCaptureVideoDataOutputSampleBufferDelegate>

+ (instancetype)shareInstance;
- (void)startCamera:(UIView*) displayView;
- (void)stopCamera;

@end

#endif /* CameraSource_h */
