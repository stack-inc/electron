// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_view.h"

#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/rect_conversions.h"
#include "ui/views/background.h"
#include "ui/views/view.h"

namespace electron {

void NativeView::TakeOverView(NATIVEVIEW view) {
  if (view_)
    delete view_;
  view_ = view;
}

void NativeView::PlatformInit() {
  TakeOverView(new views::View());
}

void NativeView::PlatformDestroy() {
  if (delete_view_)
    delete view_;
}

void NativeView::SetBounds(const gfx::Rect& bounds) {
  view_->SetBoundsRect(bounds);
}

gfx::Rect NativeView::GetBounds() const {
  return view_->bounds();
}

void NativeView::PlatformSetVisible(bool visible) {
  view_->SetVisible(visible);
}

bool NativeView::IsVisible() const {
  return view_->GetVisible();
}

bool NativeView::IsTreeVisible() const {
  return view_->GetVisible();
}

void NativeView::SchedulePaint() {
  view_->SchedulePaint();
}

void NativeView::SchedulePaintRect(const gfx::Rect& rect) {
  view_->SchedulePaintInRect(rect);
}

void NativeView::Focus() {
  view_->RequestFocus();
}

bool NativeView::HasFocus() const {
  return view_->HasFocus();
}

void NativeView::SetFocusable(bool focusable) {
}

bool NativeView::IsFocusable() const {
  return view_->IsFocusable();
}

void NativeView::SetBackgroundColor(SkColor color) {
  view_->SetBackground(views::CreateSolidBackground(color));
  view_->SchedulePaint();
}

}  // namespace electron
