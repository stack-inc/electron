// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_scroll.h"

#include <utility>

#include "shell/browser/ui/native_container.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/size_conversions.h"

namespace electron {

NativeScroll::NativeScroll() {
  PlatformInit();
  SetContentView(new NativeContainer);
#if 0
  on_scroll.SetDelegate(this, kOnScroll);
#endif
}

NativeScroll::~NativeScroll() = default;

void NativeScroll::SetContentView(scoped_refptr<NativeView> view) {
  if (content_view_)
    content_view_->SetParent(nullptr);
  PlatformSetContentView(view.get());
  content_view_ = std::move(view);
  content_view_->SetParent(this);
}

NativeView* NativeScroll::GetContentView() const {
  return content_view_.get();
}

gfx::Size NativeScroll::GetContentSize() const {
  return GetContentView()->GetBounds().size();
}

void NativeScroll::DetachChildView(NativeBrowserView* view) {
}

void NativeScroll::DetachChildView(NativeView* view) {
}

void NativeScroll::TriggerBeforeunloadEvents() {
  if (content_view_.get())
    content_view_->TriggerBeforeunloadEvents();
}

void NativeScroll::SetWindowForChildren(NativeWindow* window) {
  if (content_view_.get())
    content_view_->SetWindow(window);
}

}  // namespace electron
