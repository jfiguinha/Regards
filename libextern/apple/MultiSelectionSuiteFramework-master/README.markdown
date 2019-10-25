# MultiSelectionSuite.framework

## About
The "MultiSelectionSuite" Framework provides an easy way to draw multiple, designable and interactive selection paths (mostly rectangles or any thinkable path) on any **MacOS** (not iOS) view. In addition it has out-of-the-box modules to extract the content of the selected areas.

![Example](http://assets.saschapaulus.de/mss/example.jpg)

## Understand "MultiSelectionSuite"

**In order to have an easy start it is recommended to read the following lines.**
 I will try to keep them as short as possible.

<u>The base of MSS consists of four classes</u>

### MSSSelector
The selector is the actual path (mostly a rectangle) to mark a selection on the content. The selector can have any form since it is a simple NSBezierPath. But if you just like to change colors or lines you don't have to overwrite the MSSSelector class because MSS comes with MSSSelectorStyles which are easily configurable via Interface Builder.

### MSSCanvas
To avoid any conflics with the contentView (e.g. NSImageView) all selectors will be drawn on a so called canvas.That's it. :)

### MSSCanvasController
The canvasController handles all interactions between the view, the canvas, selectors, the mouse and others. canvasController also takes care of drawing, resizing, moving and removing. You don't even have to add the canvas as subview on your view since the controller takes care of adding and arranging too. 

### MSSHandle
A handle is a little path (by default a circle) at the selector's corners. The task of a handle is to provide an anchor to resize the selector in different directions. Like the selector, a handle can have any form you like. To change the colors only, MSS provides a style class (MSSHandleStyle) here as well.

### AddOn: MSSExtractor
An extractor is not part of the actual base classes but yet quite important. Why ? Well, because all the fancy selectors would be very useless unless you can extract the content out of the selected areas. Because the job of an extractor depends on the purpose (e.g. cropping images) the MSSExtractor class is meant to be "abstract" and must be derived for any new purpose. The example project will make that more clear.


## Quick start & further information

If you clone the git repository you'll get a complete Xcode workspace including
the framework (MultiSelectionSuite) itself and an example project (MultiSelectionSuiteApp).
If you press "Run" the framework and the example app should be built.

Now let's take a closer look on the example project:

### Understand the example project

The example project contains three real-world applications for MSS:

- Image cropping
- Zooming 
- Scan data extraction (Using my SPScannerCaptureWindow.bundle)

**Next:** Open the example project (MultiSelectionSuiteApp) and take a look on the CropImageWindow.xib (under "Crop" Group)

- The main view under the window containts a normal NSImageView
- This image view is connected to the Outlet "contentView" of the "Canvas Controller" (MSSCanvasController)
- The Outlet "view" of the "Canvas Controller" is connected to the "Canvas" (MSSCanvas)
- Note that the "Canvas" is just somewhere in the room and not a subview of another view
- The Outlet "delegate" of the "Canvas" is connected to the "Canvas Controller"

**If we only would like to have selectors on the image view without any purpose we would be finish here.**

**<u>BUT</u>** We're not satisfied yet, cause we want extract some content.

- The Outlet "canvasController" of "Image Extractor" (MSSImageExtractor) is connected to  "Canvas Controller"
- The Outlet "contentExtractor" of "Canvas Controller" is connected to "Image Extractor"
- The Outlet "delegate" of the "Image Extractor" is connected to the "File's owner" (CropImageWindow.h)
- The IBAction of the "Save" button is in the "File's Owner" - so open "CropImageWindow.m"
- The IBAction "saveImages" calls the "prepareJob"-Method of the "Image Extractor" (MSSImageExtractor)
- Note that "CropImageWindow" confirms with the "MSSExtractorDelegate" protocol
- Take look in the "extractor:extractElement:forJob"-Method (in CropImageWindow.h)
- Whenever an extractor pulls content (here: images) this delegate method will be called
- When all extractions are done the "extractor:didCompleteWithError"-Method will be called
- The image extractor comes with a "saveImage:withType"-Method which will be called here
- Note that the extractor contains all "elements" which have been extracted ... **DONE**

### Use the framework outside the example project

The example project uses the product (MultiSelectionSuite.framework) of the framework project (MultiSelectionSuite).
If you like to use the framework outside the example project you can either install 
the framework under /Library/Frameworks or embed it as "Private Framework" in your project's Application Bundle. The advantage of latter is there is no need of an installation process
because everything will be inside your Application Bundle. Drawback: In order to use newer versions of the framework you need to rebuild your App, too. However: <a target="_blank" href="https://developer.apple.com/library/mac/documentation/MacOSX/Conceptual/BPFrameworks/Tasks/CreatingFrameworks.html#//apple_ref/doc/uid/20002258-106880-BAJJBIEF">Here</a> you'll learn about how to embed & link to a private framework in your Application.

### Nice to have information

But how, where and when we assign which content fragment we want ? Well, this is all done in the MSSImageExtractor in cooperation with the canvasController. Since this project is open source and MSSImageExtractor has only one method you'll easily understand how it works.

### Strong recommendation

Since you have a good basic understanding how MSS works and how you can use it in your projects I want to stop here explaining the example project. Of course I encourage you to discover the "Scan" and "Zoom" sections in the project. The "Zoom" section uses the styles for selectors (MSSSelectionStyle) and handles (MSSHandleStyle). You'll be delighted how nice & easy these are to use.

The "Scan" section uses a special bundle of mine to encapsulate the quite complex scanning process. With "complex" I mean: There's a lot of boiler code which could distract you from learning more about this framework. Btw: The bundle uses the framework as well. If you like to know more about how scanning works in Cocoa: The bundle is open source too. You'll find it [here](https://github.com/ckteebe/SPScannerCaptureWindow). 

In order to understand the complete workflow between the canvasController and the extractors you should make you own extractor. Please keep in mind to derive your extractor from the master class MSSExtractor. If the extractor is useful for others I would be happy if you make it public for everyone by a pull request.  


## Documention

The code documentation of the framework can be found [here](http://assets.saschapaulus.de/mss/doc/v1/).
