/*
 
 ------------------------------------------------------------------------------------
 MSSExtractor.h
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

#import <Cocoa/Cocoa.h>
#import "MSSCanvasController.h"

@class MSSExtractor;

/** Defines requirements for a controller object to receive feedback from a MSSExtractor */
@protocol MSSExtractorDelegate

/** Will be called whenever an element is extracted with a given key. 
 @param extractor The extractor object from which the element comes
 @param element The fragment which has been return from the extractor
 @param identifier The name of the extraction job
 */
- (void) extractor:(MSSExtractor*)extractor extractElement:(id)element forJob:(NSString*)identifier;
/** Will be called when the extraction is completed 
 @param extractor The extractor object which extraction has been completed
 @param error An error during the extraction (if occurred)
 */
- (void) extractor:(MSSExtractor*)extractor didCompleteWithError:(NSError*)error;
@optional
/** Will be called whenever an image has been saved. 
 @param extractor The extractor which has saved the image
 @param fileUrl The URL to the saved file
 @param error An error during the saving (if occurred)
 */
- (void) extractor:(MSSExtractor*)extractor didSaveImage:(NSURL*)fileUrl withError:(NSError*)error;

@end


IB_DESIGNABLE
/** Extracts content from the contentView of MSSCanvasController or its MSSCanvas. */
@interface MSSExtractor : NSObject <MSSCanvasEventSubscriber> {
    
}
/** @name Delegation */
/** The controller object to receive feedback from the extractor */
@property (nonatomic) IBOutlet id<MSSExtractorDelegate> delegate;
/** @name Properties */
/** Identifies for which extraction job the extracation element is created */
@property (nonatomic, readonly) NSString *jobIdentifier;
/** The ViewController which manages the canvas. */
@property (nonatomic) IBOutlet MSSCanvasController *canvasController;
/** A file prefix which the files will be saved with. */
@property (nonatomic) IBInspectable NSString *filePrefix;
/** The folder URL in which the files will be stored. */
@property (nonatomic) NSURL *saveDirectory;

/** @name Results */
/** Holds the extracted elements. */
@property (nonatomic, readonly) NSMutableArray *elements;

/** @name Methods */
/** Saves an image with a chosen file type
 @param image The image which should be saved
 @param fileType The type of image which should be created
 */
- (BOOL) saveImage:(NSImage*)image withType:(NSBitmapImageFileType)fileType compression:(ExtractorImageCompressionLevel)compressionLevel;
/** Extracts each selected part from the content holder. Returing elements are using the given identifier. 
 @param identifier A unique identifier for the job in order to recognize corresponding return values
 */
- (void) prepareJob:(NSString*)identifier;
/** Sets the observation for changes on the canvas */
- (void) setObservation;
/** Will be called whenever an element is extracted.
 @param element The element which has been extracted
 @param error An error during the extraction (if occurred)
 */
- (void) didExtractElement:(id)element withError:(NSError*)error;

@end

