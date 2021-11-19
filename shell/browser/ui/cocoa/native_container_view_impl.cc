// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/cocoa/native_container_view_impl.h"

#include <vector>

#include "shell/browser/native_browser_view.h"
#include "shell/browser/ui/inspectable_web_contents_view.h"
#include "shell/browser/ui/native_container_view.h"
#include "shell/browser/ui/native_web_view_mac.h"
#include "skia/ext/skia_utils_mac.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/views/background.h"

namespace electron {

NativeContainerViewImpl::NativeContainerViewImpl(NativeContainer* container)
    : NativeBaseViewImpl(container),
      container_(container) {
  DCHECK(container);
}

NativeContainerViewImpl::NativeContainerViewImpl(NativeView* view)
    : NativeBaseViewImpl(view),
      container_(nullptr) {
  DCHECK(view);
}

void NativeContainerViewImpl::AddBrowserView(NativeBrowserView* view) {
  if (!view || !container_)
    return;

  NativeWebView* web_view = new NativeWebView(view);
  gfx::Rect bounds = view->GetLastBounds();
if (bounds.width() && bounds.height())
  web_view->SetBounds(gfx::RectF(bounds.x(), bounds.y(), bounds.width(), bounds.height()));
else
web_view->SetStyleProperty("flex", 1);
  container_->AddChildView(web_view);
  container_->Layout();
}

void NativeContainerViewImpl::RemoveBrowserView(NativeBrowserView* view) {
}

void NativeContainerViewImpl::SetTopBrowserView(NativeBrowserView* view) {
}

void NativeContainerViewImpl::AddContainerView(NativeContainerViewImpl* view) {
  if (!view || !container_)
    return;

  container_->AddChildView(view->GetView());
  container_->Layout();
}

void NativeContainerViewImpl::RemoveContainerView(
    NativeContainerViewImpl* view) {
}

void NativeContainerViewImpl::SetTopContainerView(
    NativeContainerViewImpl* view) {
}

// static
NativeContainerView* NativeContainerView::Create() {
  return new NativeContainerView(
      new NativeContainerViewImpl(new NativeContainer()));
}

}  // namespace electron
