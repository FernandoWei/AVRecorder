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
    CameraSource* camera;
}

@end

@implementation ViewController

- (void)startVideoRecording
{
    camera = [CameraSource shareInstance];
    [camera startCamera:_displayView];
}

- (void)endVideoRecording
{
    if (!camera)
    {
        camera = [CameraSource shareInstance];
    }
    
    [camera stopCamera];
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
