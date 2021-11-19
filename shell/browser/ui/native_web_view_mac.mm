#include "shell/browser/ui/native_web_view_mac.h"

#include "shell/browser/ui/cocoa/electron_native_view.h"
#include "shell/browser/ui/inspectable_web_contents_view.h"

@interface ElectronNativeWebView : NSView <ElectronNativeView> {
 @private
  electron::NativeViewPrivate private_;
}
@end

@implementation ElectronNativeWebView

- (void)drawRect:(NSRect)dirtyRect {
  if ([[self subviews] count])
    return;
}

- (electron::NativeViewPrivate*)nativeViewPrivate {
  return &private_;
}

- (void)setNativeBackgroundColor:(SkColor)color {
}

@end

namespace electron {

// static
const char NativeWebView::kClassName[] = "WebView";

NativeWebView::NativeWebView(NativeBrowserView* browser_view)
    : browser_view_(browser_view) {
  auto* page = browser_view->GetInspectableWebContentsView()
                   ->GetNativeView()
                   .GetNativeNSView();
  NSView* view = [[ElectronNativeWebView alloc] init];
  [view addSubview:page];
  TakeOverView(view);
}

NativeWebView::~NativeWebView() {}

const char* NativeWebView::GetClassName() const {
  return kClassName;
}

void NativeWebView::SetBounds(const gfx::RectF& bounds) {
  NativeView::SetBounds(bounds);
  browser_view_->SetBounds(
      gfx::Rect(bounds.x(), bounds.y(), bounds.width(), bounds.height()));
}

}  // namespace electron
