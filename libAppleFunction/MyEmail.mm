//
//  MyObject.m
//  Regards
//
//  Created by figuinha jacques on 10/08/2015.
//
//
#ifdef __APPLE__
#import <Foundation/Foundation.h>
#import "RegardsInclude.h"
#include "MyEmailInterface.h"
#import <AppKit/AppKit.h>
#include "wx/osx/private.h"
#include "wx/uri.h"
@interface sharingServiceDelegate : NSObject <NSSharingServiceDelegate>
@end

@interface sharingServiceDelegate ()
@property NSString *recipientString;
@property NSString *subjectString;
@property NSString *bodyString;
@property NSURL <NSPasteboardWriting> *attachmentURL;
@property NSSharingService *emailSharingService;
@end


@implementation sharingServiceDelegate

- (id)init {
    
    self = [super init];
    
    if (self) {
        NSSharingService *emailSharingService = [NSSharingService sharingServiceNamed:NSSharingServiceNameComposeEmail];
        emailSharingService.delegate = self;
        self.emailSharingService = emailSharingService;
    }
    
    return self;
}

- (BOOL)shareUsingEmail
{
    NSArray *shareItems =@[_bodyString, _attachmentURL];
    self.emailSharingService.recipients = @[_recipientString];
    self.emailSharingService.subject = _subjectString;
    
    if([self.emailSharingService canPerformWithItems:shareItems]){
        [self.emailSharingService performWithItems:shareItems];
        return TRUE;
    } else {
        // handle failure ...
    }
    
    return FALSE;
}

@end

MyEmailImpl::MyEmailImpl( void )
{   }

MyEmailImpl::~MyEmailImpl( void )
{
}


void MyEmailImpl::SendEmail(const wxString &to, const wxString &subject, const wxString & filePicture)
{
    NSString * toAddress = wxNSStringWithWxString(to);
    NSString * subjectEmail = wxNSStringWithWxString(subject);
    //NSString * photo = wxNSStringWithWxString(filePicture);
    NSString *bodyText = @"Your body text \n\r";
    
    wxURI photo = wxURI(wxString( wxT("file://") ) + filePicture);
    wxCFStringRef uri(photo.BuildURI());
    NSURL *url = [NSURL URLWithString: uri.AsNSString()];
    
    sharingServiceDelegate * shareDelegate = [[sharingServiceDelegate alloc] init];
    
    shareDelegate.recipientString = toAddress;
    shareDelegate.subjectString = subjectEmail;
    shareDelegate.bodyString = bodyText;
    shareDelegate.attachmentURL = url;
    
    if([shareDelegate shareUsingEmail]==FALSE)
        NSLog(@"Email Sharing Service failed.\n");
    

    NSLog(@"Message passed to Mail");
    
}
#endif