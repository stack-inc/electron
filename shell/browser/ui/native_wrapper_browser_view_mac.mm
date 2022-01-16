// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_wrapper_browser_view.h"

#include "shell/browser/api/electron_api_browser_view.h"
#include "shell/browser/ui/cocoa/electron_native_view.h"
#include "shell/browser/ui/inspectable_web_contents_view.h"
#include "skia/ext/skia_utils_mac.h"

@interface ElectronNativeWrapperBrowserView : NSView <ElectronNativeView> {
 @private
  electron::NativeViewPrivate private_;
  SkColor background_color_;
}
@end

@implementation ElectronNativeWrapperBrowserView

- (void)drawRect:(NSRect)dirtyRect {
  if ([[self subviews] count])
    return;

  gfx::RectF dirty(dirtyRect);
  CGContextRef context = reinterpret_cast<CGContextRef>(
      [[NSGraphicsContext currentContext] graphicsPort]);
  CGContextSetRGBStrokeColor(context, SkColorGetR(background_color_) / 255.0f,
                             SkColorGetG(background_color_) / 255.0f,
                             SkColorGetB(background_color_) / 255.0f,
                             SkColorGetA(background_color_) / 255.0f);
  CGContextSetRGBFillColor(context, SkColorGetR(background_color_) / 255.0f,
                           SkColorGetG(background_color_) / 255.0f,
                           SkColorGetB(background_color_) / 255.0f,
                           SkColorGetA(background_color_) / 255.0f);
  CGContextFillRect(context, dirty.ToCGRect());
}

- (electron::NativeViewPrivate*)nativeViewPrivate {
  return &private_;
}

- (void)setNativeBackgroundColor:(SkColor)color {
  background_color_ = color;
  [self setNeedsDisplay:YES];
}

@end

namespace electron {

void NativeWrapperBrowserView::PlatformInit() {
  NSView* view = [[ElectronNativeWrapperBrowserView alloc] init];
  TakeOverView(view);
}

void NativeWrapperBrowserView::PlatformSetBrowserView() {
  if (api_browser_view_ && api_browser_view_->view()->GetInspectableWebContentsView()) {
    auto* page = api_browser_view_->view()->GetInspectableWebContentsView()
                     ->GetNativeView().GetNativeNSView();
    [GetNative() addSubview:page];
  }
}

void NativeWrapperBrowserView::PlatformDetachBrowserView() {
  if (api_browser_view_ && api_browser_view_->view()->GetInspectableWebContentsView()) {
    auto* page = api_browser_view_->view()->GetInspectableWebContentsView()
                     ->GetNativeView().GetNativeNSView();
    [page removeFromSuperview];
  }
}

void NativeWrapperBrowserView::UpdateDraggableRegions() {
  if (api_browser_view_) {
    api_browser_view_->view()->UpdateDraggableRegions(
        api_browser_view_->view()->GetDraggableRegions());
  }
}

}  // namespace electron
