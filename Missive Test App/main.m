//
//  main.m
//  Missive Test App
//
//  Created by Rowan James on 26/04/2013.
//  Copyright (c) 2013 Rowan James. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "ErrorReporter.h"
#import "Publisher.h"

int main(int argc, char *argv[])
{
	[ErrorReporter start];
	[Publisher start];
	
	return NSApplicationMain(argc, (const char **)argv);
}
