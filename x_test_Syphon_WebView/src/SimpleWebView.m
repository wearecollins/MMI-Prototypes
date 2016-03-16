
#import "SimpleWebView.h"


@implementation SimpleWebView

-(void) awakeFromNib
{
//    [[self mainFrame] mainFrameURL:[NSString stringWithUTF8String:"http://mrdoob.com"]];
    [[self mainFrame] loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:[NSString stringWithUTF8String:"http://mrdoob.com"]]]];
}
@end
