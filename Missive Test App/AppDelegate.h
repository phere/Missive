//
//  AppDelegate.h
//  Missive Test App
//
//  Created by Rowan James on 26/04/2013.
//  Copyright (c) 2013 Rowan James. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;

@property (assign) IBOutlet NSTextView *textArea;

- (IBAction)sendClicked:(id)sender;

@end
