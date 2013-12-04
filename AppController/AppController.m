//
//  AppController.m
//  AppController
//
//  Created by Zichen Zheng on 12/2/13.
//  Copyright (c) 2013 Zichen Zheng. All rights reserved.
//

#import "AppController.h"

@implementation AppController

@synthesize frameOutputURL;

- (BOOL) setupAVCapture {
	NSError *error = nil;
    
    captureSession = [AVCaptureSession new];
	[captureSession setSessionPreset:AVCaptureSessionPresetPhoto];
	
	// Select a video device, make an input
	for (AVCaptureDevice *device in [AVCaptureDevice devices]) {
		if ([device hasMediaType:AVMediaTypeVideo] || [device hasMediaType:AVMediaTypeMuxed]) {
			AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:device error:&error];
			if (error) {
                [captureSession release];
                NSLog(@"deviceInputWithDevice failed with error %@", [error localizedDescription]);
				return NO;
            }
			if ([captureSession canAddInput:input])
				[captureSession addInput:input];
			break;
		}
	}
	
	// Make a still image output
	stillImageOutput = [AVCaptureStillImageOutput new];
	if ([captureSession canAddOutput:stillImageOutput])
		[captureSession addOutput:stillImageOutput];
	
	// Make a preview layer so we can see the visual output of an AVCaptureSession
	previewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:captureSession];
	[previewLayer setVideoGravity:AVLayerVideoGravityResizeAspectFill];
	[previewLayer setFrame:[previewView bounds]];
	[[previewLayer connection] setAutomaticallyAdjustsVideoMirroring:NO];
	[[previewLayer connection] setVideoMirrored:YES];
    
    // add the preview layer to the hierarchy
	CALayer *rootLayer = [previewView layer];
	[rootLayer setBackgroundColor: CGColorGetConstantColor(kCGColorBlack)];
	[rootLayer addSublayer: previewLayer];
    // add a text field (or label) on top of the preview, showing the gesture status
    [rootLayer addSublayer: [gdStatusView layer]];
	
    // start the capture session running, note this is an async operation
    // status is provided via notifications such as AVCaptureSessionDidStartRunningNotification/AVCaptureSessionDidStopRunningNotification
    [captureSession startRunning];
	
	return YES;
}

- (void) saveImage {
    // initiate a still image capture, return immediately
    // the completionHandler is called when a sample buffer has been captured
	AVCaptureConnection *stillImageConnection = [stillImageOutput connectionWithMediaType:AVMediaTypeVideo];
	[stillImageOutput captureStillImageAsynchronouslyFromConnection:stillImageConnection completionHandler:^(CMSampleBufferRef imageDataSampleBuffer, NSError *__strong error) {
        NSData* imageData = [AVCaptureStillImageOutput jpegStillImageNSDataRepresentation:imageDataSampleBuffer];
        NSImage* image = [[NSImage alloc] initWithData:imageData];
        imageData = [image TIFFRepresentation];
        NSBitmapImageRep* bitmap = [NSBitmapImageRep imageRepWithData: imageData];
        imageData = [bitmap representationUsingType: NSPNGFileType properties: nil];
        [imageData writeToFile: [frameOutputURL path] atomically:YES];
        [image autorelease];  // avoid potential leak
	}];
}

- (void) appControl {
    while (true) {
        // save frame as an image
        [self saveImage];
        
        // execute gesture detection routine
        NSTask* gdTask = [[NSTask alloc] init];  // task for gesture detection
        NSString* gdExecPath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent: @"GestureDetector/GestureDetector"];
        [gdTask setLaunchPath: gdExecPath];
        [gdTask setArguments: [NSArray arrayWithObjects: @"-m", @"5", @"-i", frameOutputURL, nil]];
        [gdTask launch];
        [gdTask waitUntilExit];
        
        if ([gdTask terminationStatus] != 0) continue;
        
        // read detection result from text file
        NSURL* gdResultURL = [NSURL URLWithString: @"GestureDetector/predict.result" relativeToURL:[[NSBundle mainBundle] resourceURL]];;
        NSString* gdResultStr = [[NSString alloc] initWithContentsOfURL: gdResultURL encoding:NSUTF8StringEncoding error: nil];
        int gdCode = [gdResultStr intValue];
        NSLog(@"Gesture code: %d\n", gdCode);
        
        itunes = [SBApplication applicationWithBundleIdentifier: @"com.apple.iTunes"];
        if (gdCode == 0) {
            [gdStatusView setStringValue: @"No Gesture"];
            if ([itunes isRunning]) {
                if (iTunesEPlSPlaying != [itunes playerState]) {
                    [itunes playpause];
                }
            }
        }
        else if (gdCode == 1) {
            [gdStatusView setStringValue: @"Fist"];
        }
        else if (gdCode == 2) {
            [gdStatusView setStringValue: @"Five"];
        }
        else if (gdCode == 3) {
            [gdStatusView setStringValue: @"Pointing"];
        }
        
    }
}

- (IBAction) startStop: (id) sender {
	if (started) {
		// finish
        [appControlThread cancel];
		[sender setTitle:@"Start"];
	}
	else {
        [appControlThread start];
		[sender setTitle:@"Stop"];
	}
	started = !started;
}

- (void) close {
    [captureSession stopRunning];
	[previewLayer removeFromSuperlayer];
	[previewLayer setSession:nil];
    [previewLayer release];
    [stillImageOutput release];
    [captureSession release];
	[super close];
}

- (IBAction) togglePreviewMirrored: (id) sender {
	[[previewLayer connection] setVideoMirrored:[(NSButton *)sender state]];
}

- (id) init {
    self = [super init];
    self.frameOutputURL = [NSURL URLWithString: @"frame.png" relativeToURL:[[NSBundle mainBundle] resourceURL]];
    [[NSFileManager defaultManager] removeItemAtURL:self.frameOutputURL error:nil];
    appControlThread = [[NSThread alloc] initWithTarget:self selector:@selector(appControl) object:nil];
    [self setDisplayName: @"AppController Monitor"];
    [gdStatusView setStringValue: @"No Gesture"];
    
    return self;
}

- (NSString*) windowNibName {
	return @"AppControllerMonitor";
}

- (void) windowControllerDidLoadNib: (NSWindowController*) aController {
	[super windowControllerDidLoadNib:aController];
	[self setupAVCapture];
}

@end
