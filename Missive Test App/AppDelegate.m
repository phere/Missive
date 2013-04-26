//
//  AppDelegate.m
//  Missive Test App
//
//  Created by Rowan James on 26/04/2013.
//  Copyright (c) 2013 Rowan James. All rights reserved.
//

#import "AppDelegate.h"

#import "Dispatcher.h"

@interface AppDelegate () <NSTextViewDelegate>

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	// Insert code here to initialize your application
}

- (IBAction)sendClicked:(id)sender
{
	NSString *text = self.textArea.textStorage.string;
	
	dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
	dispatch_async(queue, ^{
		Dispatcher* dispatcher = [Dispatcher new];
		[dispatcher send:text];
	});
	[self.textArea selectAll:self];
	[self.textArea insertText:@""];
}

@end
