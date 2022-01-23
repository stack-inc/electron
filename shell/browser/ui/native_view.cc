// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_view.h"

#include <utility>

#include "base/strings/string_util.h"
#include "shell/browser/browser.h"
#include "shell/browser/native_window.h"
#include "shell/browser/ui/native_container_view.h"
#include "shell/common/color_util.h"
#include "ui/gfx/geometry/rect.h"

namespace electron {

NativeView::NativeView() : view_(nullptr) {
  PlatformInit();
}

NativeView::~NativeView() {
  PlatformDestroy();
}

void NativeView::SetVisible(bool visible) {
  if (visible == IsVisible())
    return;
  PlatformSetVisible(visible);
}

void NativeView::SetParent(NativeView* parent) {
  if (parent) {
    SetWindow(parent->window_);
  } else {
    SetWindow(nullptr);
  }
  parent_ = parent;
}

void NativeView::BecomeContentView(NativeWindow* window) {
  SetWindow(window);
  parent_ = nullptr;
}

void NativeView::SetWindow(NativeWindow* window) {
  window_ = window;
  SetWindowForChildren(window);
}

void NativeView::SetWindowForChildren(NativeWindow* window) {}

bool NativeView::IsContainer() const {
  return false;
}

void NativeView::OnSizeChanged() {}

void NativeView::DetachChildView(NativeView* view) {}

void NativeView::TriggerBeforeunloadEvents() {}

}  // namespace electron
