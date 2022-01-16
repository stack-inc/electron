// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_container.h"

#include <utility>

#include "base/memory/ptr_util.h"
#include "ui/views/view.h"

namespace electron {

void NativeContainer::PlatformInit() {
  TakeOverView(new views::View());
}

void NativeContainer::PlatformDestroy() {}

void NativeContainer::PlatformAddChildView(NativeView* child) {
  child->set_delete_view(false);
  GetNative()->AddChildView(child->GetNative());
}

void NativeContainer::PlatformRemoveChildView(NativeView* child) {
  child->set_delete_view(true);
  GetNative()->RemoveChildView(child->GetNative());
}

void NativeContainer::PlatformSetTopView(NativeView* view) {
  GetNative()->ReorderChildView(view->GetNative(), -1);
}

}  // namespace electron
