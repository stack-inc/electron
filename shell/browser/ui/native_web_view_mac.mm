// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

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

NativeWebView::NativeWebView(NativeBrowserView* browser_view)
    : browser_view_(browser_view) {
  auto* page = browser_view->GetInspectableWebContentsView()
                   ->GetNativeView()
                   .GetNativeNSView();
  NSView* view = [[ElectronNativeWebView alloc] init];
  [view addSubview:page];
  TakeOverView(view);
}

NativeWebView::~NativeWebView() = default;

void NativeWebView::SetBounds(const gfx::Rect& bounds) {
  NativeView::SetBounds(bounds);
  browser_view_->SetBounds(bounds);
}

}  // namespace electron
