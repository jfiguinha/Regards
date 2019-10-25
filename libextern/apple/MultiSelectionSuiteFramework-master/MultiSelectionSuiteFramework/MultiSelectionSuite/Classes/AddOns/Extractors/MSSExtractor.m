/*
 
 ------------------------------------------------------------------------------------
 MSSExtractor.m
 ------------------------------------------------------------------------------------
 The extractor is an Add-On for SelectorSuite to extract content from the contentView
 of the canvasController. It must be derived for each purpose like image cropping
 or scanning (see MSSImageExtractor or MSSScanExtractor).
 Tipp: For a quick start please read the README document.
 ------------------------------------------------------------------------------------
 
 The MIT License (MIT)
 
 Copyright (c) 2014 Sascha Paulus
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 */

#import "MSSExtractor.h"
#import "MSSError.h"
#import "MSSPreferences.h"

@implementation MSSExtractor

@synthesize saveDirectory;
@synthesize elements = _elements;
@synthesize jobIdentifier = _jobIdentifier;

#pragma mark - Init

- (void) commonInit {
    _elements = [NSMutableArray array];
    [self setObservation];
}

-(instancetype)init {
    self = [super init];
    if (self) {
       [self commonInit];
    }
    return self;
}

#pragma mark - MSSCanvasEventSubscriber

- (void) setObservation {
    return;
}

- (void)canvasHasChanged {
    return;
}

- (void)selectorHasChanged:(MSSSelector *)selector {
    return;
}


#pragma mark - Default methods

- (void) prepareJob:(NSString*)identifier {

    _jobIdentifier = identifier;
}

- (BOOL) saveImage:(NSImage*)image withType:(NSBitmapImageFileType)fileType compression:(ExtractorImageCompressionLevel)compressionLevel {
    
    
    if(self.saveDirectory==nil) {
        NSDictionary *errorDesc = @{ NSLocalizedDescriptionKey : MSS_ERROR_DESC(MSSErrorNoSaveDirectory) };
        NSError *error = [NSError errorWithDomain:MSSErrorDomain code:MSSErrorNoSaveDirectory userInfo:errorDesc];
        [self.delegate extractor:self didSaveImage:nil withError:error];
        return NO;
    }
    
    if([[NSFileManager defaultManager] fileExistsAtPath:[self.saveDirectory path]]) {
        NSNumber *isDirectory;
        BOOL success = [self.saveDirectory getResourceValue:&isDirectory forKey:NSURLIsDirectoryKey error:nil];
        if(!success || ![isDirectory boolValue]) {
            NSDictionary *errorDesc = @{ NSLocalizedDescriptionKey : MSS_ERROR_DESC(MSSErrorSaveDirectoryNotExists) };
            NSError *error = [NSError errorWithDomain:MSSErrorDomain code:MSSErrorSaveDirectoryNotExists userInfo:errorDesc];
            [self.delegate extractor:self didSaveImage:nil withError:error];
            return NO;
        }
    } else {
        NSDictionary *errorDesc = @{ NSLocalizedDescriptionKey : MSS_ERROR_DESC(MSSErrorSaveDirectoryUrlIsNoDirectory) };
        NSError *error = [NSError errorWithDomain:MSSErrorDomain code:MSSErrorSaveDirectoryUrlIsNoDirectory userInfo:errorDesc];
        [self.delegate extractor:self didSaveImage:nil withError:error];
        return NO;
    }
    
    NSString *extention = nil;
    
    switch (fileType) {
        case NSPNGFileType:
            extention = @".png";
            break;
        case NSJPEGFileType:
            extention = @".jpg";
            break;
        case NSGIFFileType:
            extention = @".gif";
            break;
        case NSBMPFileType:
            extention = @".bmp";
            break;
        case NSTIFFFileType:
            extention = @".tiff";
            break;
        default:
            break;
    }
    
    if(extention!=nil) {
        
        int i = 0;
        NSURL *saveUrl = nil;
        while (saveUrl == nil || [[NSFileManager defaultManager] fileExistsAtPath:[saveUrl path]]) {
            i++;
            NSString *filename = [NSString stringWithFormat:@"%@%i%@",self.filePrefix,i,extention];
            saveUrl = [self.saveDirectory URLByAppendingPathComponent:filename];
        }
        
        CGFloat imageCompression = 1.0;
        NSError *error = nil;
        NSDictionary *options = nil;
        if(fileType == NSJPEGFileType) {
            switch (compressionLevel) {
                case ExtractorImageCompressionLevelLow:
                    imageCompression = 0.8;
                    break;
                case ExtractorImageCompressionLevelMiddle:
                    imageCompression = 0.6;
                    break;
                case ExtractorImageCompressionLevelHigh:
                    imageCompression = 0.3;
                    break;
                case ExtractorImageCompressionLevelHighest:
                    imageCompression = 0.1;
                    break;
                default:
                    imageCompression = 1.0;
                    break;
            }
            options = [NSDictionary dictionaryWithObjectsAndKeys:
                       [NSNumber numberWithDouble:imageCompression], NSImageCompressionFactor,
                       [NSNumber numberWithBool:NO], NSImageProgressive,
                       nil];
        }
        
        NSBitmapImageRep *rep = [[NSBitmapImageRep alloc] initWithData:[image TIFFRepresentation]];
        NSData *fileData = [rep representationUsingType:fileType properties:options];
        [fileData writeToURL:saveUrl options:NSDataWritingWithoutOverwriting error:&error];
        if(error!=nil) {
            [self.delegate extractor:self didSaveImage:nil withError:error];
            NSDictionary *errorDesc = @{ NSLocalizedDescriptionKey : MSS_ERROR_DESC(MSSErrorImageSavingFailed) };
            NSError *error = [NSError errorWithDomain:MSSErrorDomain code:MSSErrorImageSavingFailed userInfo:errorDesc];
            [self.delegate extractor:self didSaveImage:nil withError:error];
        } else {
            [self.delegate extractor:self didSaveImage:saveUrl withError:nil];
        }
        
    } else {
        
        NSDictionary *errorDesc = @{ NSLocalizedDescriptionKey : MSS_ERROR_DESC(MSSErrorFileTypeNotSupportedYet) };
        NSError *error = [NSError errorWithDomain:MSSErrorDomain code:MSSErrorFileTypeNotSupportedYet userInfo:errorDesc];
        [self.delegate extractor:self didSaveImage:nil withError:error];
        return NO;
    }
    
    return YES;
}

#pragma mark - delegate notification

- (void) didExtractElement:(id)element withError:(NSError*)error {
    
    if(error == nil && element != nil) {
        if(self.jobIdentifier == nil) _jobIdentifier = MSSExtractorJobUnknown;
        [_elements addObject:element];
        [self.delegate extractor:self extractElement:element forJob:self.jobIdentifier];
    } else {
        [self.delegate extractor:self didCompleteWithError:error];
    }
}




@end
