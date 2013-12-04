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
    iTunesApplication* itunes;
}

@property (retain) NSURL* frameOutputURL;
- (IBAction) startStop: (id) sender;
- (IBAction) togglePreviewMirrored: (id) sender;
@end
