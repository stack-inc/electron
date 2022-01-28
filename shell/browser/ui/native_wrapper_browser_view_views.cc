// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_wrapper_browser_view.h"

#include "shell/browser/api/electron_api_browser_view.h"
#include "shell/browser/ui/inspectable_web_contents_view.h"
#include "ui/views/view.h"

namespace electron {

void NativeWrapperBrowserView::PlatformInit() {
  TakeOverView(new views::View());
}

void NativeWrapperBrowserView::PlatformSetBrowserView() {
  if (!GetNative())
    return;
  if (api_browser_view_ &&
      api_browser_view_->view()->GetInspectableWebContentsView()) {
    GetNative()->AddChildView(
        api_browser_view_->view()->GetInspectableWebContentsView()->GetView());
  }
}

void NativeWrapperBrowserView::PlatformDetachBrowserView() {
  if (!GetNative())
    return;
  if (api_browser_view_ &&
      api_browser_view_->view()->GetInspectableWebContentsView()) {
    GetNative()->RemoveChildView(
        api_browser_view_->view()->GetInspectableWebContentsView()->GetView());
  }
}

}  // namespace electron
