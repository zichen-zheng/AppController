//
//  AppController.h
//  AppController
//
//  Created by Zichen Zheng on 12/2/13.
//  Copyright (c) 2013 Zichen Zheng. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <AVFoundation/AVFoundation.h>
#import <AppKit/NSImage.h>
#import <Foundation/NSTimer.h>
#import "iTunes.h"

@interface AppController: NSDocument {
    IBOutlet NSView* previewView;
    IBOutlet NSTextField* gdStatusView;
    AVCaptureSession* captureSession;
    AVCaptureVideoPreviewLayer* previewLayer;
    AVCaptureStillImageOutput* stillImageOutput;
    BOOL started;
    NSURL* frameOutputURL;
    NSThread* appControlThread;
    iTunesApplication* itunesApp;
    int previousGDCode;
}

@property (retain) NSURL* frameOutputURL;
@property (atomic) BOOL endAppControlLoop;
- (IBAction) startStop: (id) sender;
- (IBAction) togglePreviewMirrored: (id) sender;
@end
