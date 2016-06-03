//
//  ViewController.m
//  AVRecorder
//
//  Created by fernando on 16/3/12.
//  Copyright © 2016年 XCKanKan. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()
{
}

@end

@implementation ViewController

- (void)startVideoRecording
{
    [self setupRTMPSession];
    [[CameraSource shareInstance] startCamera:_displayView];
}

- (void)setupRTMPSession
{
    if (NO == [[RTMPSession shareInstance] setupRTMPStream:@"rtmp://192.168.64.219:19350/myapp/mystream"])
    {
        return;
    }
}

- (void)endVideoRecording
{
    [[CameraSource shareInstance] stopCamera];
    [[RTMPSession shareInstance] freeRTMPStream];
}

- (IBAction)stopButton:(id)sender
{
    [self endVideoRecording];
}

- (IBAction)startButton:(id)sender
{
    [self startVideoRecording];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
