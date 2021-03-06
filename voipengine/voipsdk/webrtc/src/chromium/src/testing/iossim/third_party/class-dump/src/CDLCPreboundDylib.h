// -*- mode: ObjC -*-

//  This file is part of class-dump, a utility for examining the Objective-C segment of Mach-O files.
//  Copyright (C) 1997-1998, 2000-2001, 2004-2010 Steve Nygard.

#import "CDLoadCommand.h"
#include <mach-o/loader.h>

@interface CDLCPreboundDylib : CDLoadCommand
{
    struct prebound_dylib_command preboundDylibCommand;
    NSString *name;
}

- (id)initWithDataCursor:(CDDataCursor *)cursor machOFile:(CDMachOFile *)aMachOFile;
- (void)dealloc;

- (uint32_t)cmd;
- (uint32_t)cmdsize;

- (NSString *)name;

@end
