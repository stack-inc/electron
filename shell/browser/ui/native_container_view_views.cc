// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_container_view.h"

#include <utility>

#include "base/memory/ptr_util.h"
#include "ui/views/view.h"

namespace electron {

void NativeContainerView::PlatformInit() {
  TakeOverView(new views::View());
}

void NativeContainerView::PlatformDestroy() {}

void NativeContainerView::PlatformAddChildView(NativeView* view) {
  view->set_delete_view(false);
  GetNative()->AddChildView(view->GetNative());
}

void NativeContainerView::PlatformRemoveChildView(NativeView* view) {
  view->set_delete_view(true);
  GetNative()->RemoveChildView(view->GetNative());
}

void NativeContainerView::PlatformSetTopView(NativeView* view) {
  GetNative()->ReorderChildView(view->GetNative(), -1);
}

}  // namespace electron
