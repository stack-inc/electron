// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_container.h"

#include <utility>

#include "base/memory/ptr_util.h"
#include "shell/browser/native_browser_view.h"
#include "shell/browser/ui/inspectable_web_contents_view.h"
#include "ui/views/view.h"

namespace electron {

void NativeContainer::PlatformInit() {
  TakeOverView(new views::View());
}

void NativeContainer::PlatformDestroy() {}

void NativeContainer::PlatformAddChildView(NativeBrowserView* child) {
  if (child->GetInspectableWebContentsView())
    GetNative()->AddChildView(child->GetInspectableWebContentsView()->GetView());
}

void NativeContainer::PlatformAddChildView(NativeView* child) {
  child->set_delete_view(false);
  GetNative()->AddChildView(child->GetNative());
}

void NativeContainer::PlatformRemoveChildView(NativeBrowserView* child) {
  if (child->GetInspectableWebContentsView())
    GetNative()->RemoveChildView(child->GetInspectableWebContentsView()->GetView());
}

void NativeContainer::PlatformRemoveChildView(NativeView* child) {
  child->set_delete_view(true);
  GetNative()->RemoveChildView(child->GetNative());
}

/*
void NativeContainerViewImpl::SetTopBrowserView(NativeBrowserView* view) {
  if (!view)
    return;

  if (view->GetInspectableWebContentsView()) {
    view_->ReorderChildView(view->GetInspectableWebContentsView()->GetView(),
                            -1);
  view_->Layout();
  }
}

void NativeContainerViewImpl::SetTopContainerView(
    NativeContainerViewImpl* view) {
  if (!view)
    return;

  view_->ReorderChildView(view->GetView(), -1);
  view_->Layout();
}
*/

}  // namespace electron
