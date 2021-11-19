// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/cocoa/native_base_view_impl.h"

#include "skia/ext/skia_utils_mac.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/rect_f.h"
#include "ui/views/background.h"

namespace electron {

NativeBaseViewImpl::NativeBaseViewImpl(NativeView* view) : view_(view) {}

NativeBaseViewImpl::~NativeBaseViewImpl() = default;

NativeView* NativeBaseViewImpl::GetView() const {
  return view_.get();
}

void NativeBaseViewImpl::SetAutoResizeFlags(uint8_t flags) {}

void NativeBaseViewImpl::SetBounds(const gfx::Rect& bounds) {
  view_->SetBounds(
      gfx::RectF(bounds.x(), bounds.y(), bounds.width(), bounds.height()));
}

gfx::Rect NativeBaseViewImpl::GetBounds() {
  gfx::RectF bounds = view_->GetBounds();
  return gfx::Rect(bounds.x(), bounds.y(), bounds.width(), bounds.height());
}

void NativeBaseViewImpl::SetBackgroundColor(SkColor color) {
#if 0
  auto* view = GetNativeView().GetNativeNSView();
  view.wantsLayer = YES;
  view.layer.backgroundColor = skia::CGColorCreateFromSkColor(color);
#endif
  view_->SetBackgroundColor(color);
}

void NativeBaseViewImpl::SetStyleProperty(const std::string& name,
                                          const std::string& value) {
  view_->SetStyleProperty(name, value);
}

void NativeBaseViewImpl::SetStyleProperty(const std::string& name,
                                          float value) {
  view_->SetStyleProperty(name, value);
}

void NativeBaseViewImpl::Layout() {
  view_->Layout();
}

}  // namespace electron
