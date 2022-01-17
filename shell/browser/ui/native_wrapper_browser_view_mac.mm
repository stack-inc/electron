// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_wrapper_browser_view.h"

#include "shell/browser/api/electron_api_browser_view.h"
#include "shell/browser/ui/cocoa/electron_native_view.h"
#include "shell/browser/ui/inspectable_web_contents_view.h"

namespace electron {

void NativeWrapperBrowserView::PlatformInit() {
  TakeOverView([[ElectronNativeView alloc] init]);
}

void NativeWrapperBrowserView::PlatformSetBrowserView() {
  if (api_browser_view_ &&
      api_browser_view_->view()->GetInspectableWebContentsView()) {
    auto* page = api_browser_view_->view()
                     ->GetInspectableWebContentsView()
                     ->GetNativeView()
                     .GetNativeNSView();
    [GetNative() addSubview:page];
  }
}

void NativeWrapperBrowserView::PlatformDetachBrowserView() {
  if (api_browser_view_ &&
      api_browser_view_->view()->GetInspectableWebContentsView()) {
    auto* view = api_browser_view_->view()
                     ->GetInspectableWebContentsView()
                     ->GetNativeView()
                     .GetNativeNSView();
    [view removeFromSuperview];
  }
}

void NativeWrapperBrowserView::UpdateDraggableRegions() {
  if (api_browser_view_) {
    api_browser_view_->view()->UpdateDraggableRegions(
        api_browser_view_->view()->GetDraggableRegions());
  }
}

}  // namespace electron
