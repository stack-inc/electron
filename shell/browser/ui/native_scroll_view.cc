// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_scroll_view.h"

#include "shell/browser/ui/views/native_scroll_view_impl.h"

namespace electron {

NativeScrollView::NativeScrollView(NativeScrollViewImpl* view_impl)
    : NativeContainerView(view_impl), scroll_view_impl_(view_impl) {}

NativeScrollView::~NativeScrollView() = default;

void NativeScrollView::SetContentView(NativeContainerView* view) {
  if (!view)
    return;

  view->set_parent_view(this);
  view->SetOwnerWindow(GetOwnerWindow());
  content_view_ = view;

  if (scroll_view_impl_)
    scroll_view_impl_->SetContentView(view->GetViewImpl());
}

NativeContainerView* NativeScrollView::GetContentView() {
  return content_view_;
}

void NativeScrollView::SetContentSize(gfx::Size size) {
  if (scroll_view_impl_)
    scroll_view_impl_->SetContentSize(size);
}

gfx::Size NativeScrollView::GetContentSize() {
  if (scroll_view_impl_)
    return scroll_view_impl_->GetContentSize();
  return gfx::Size();
}

int NativeScrollView::GetMinHeight() {
  if (scroll_view_impl_)
    return scroll_view_impl_->GetMinHeight();
  return -1;
}

int NativeScrollView::GetMaxHeight() {
  if (scroll_view_impl_)
    return scroll_view_impl_->GetMaxHeight();
  return -1;
}

void NativeScrollView::ClipHeightTo(int min_height, int max_height) {
  if (scroll_view_impl_)
    scroll_view_impl_->ClipHeightTo(min_height, max_height);
}

gfx::Rect NativeScrollView::GetVisibleRect() {
  if (scroll_view_impl_)
    return scroll_view_impl_->GetVisibleRect();
  return gfx::Rect();
}

void NativeScrollView::SetHorizontalScrollBarMode(ScrollBarMode mode) {
  if (scroll_view_impl_)
    scroll_view_impl_->SetHorizontalScrollBarMode(mode);
}

ScrollBarMode NativeScrollView::GetHorizontalScrollBarMode() {
  if (scroll_view_impl_)
    return scroll_view_impl_->GetHorizontalScrollBarMode();
  return ScrollBarMode::kEnabled;
}

void NativeScrollView::SetVerticalScrollBarMode(ScrollBarMode mode) {
  if (scroll_view_impl_)
    scroll_view_impl_->SetVerticalScrollBarMode(mode);
}

ScrollBarMode NativeScrollView::GetVerticalScrollBarMode() {
  if (scroll_view_impl_)
    return scroll_view_impl_->GetVerticalScrollBarMode();
  return ScrollBarMode::kEnabled;
}

void NativeScrollView::SetAllowKeyboardScrolling(bool allow) {
  if (scroll_view_impl_)
    scroll_view_impl_->SetAllowKeyboardScrolling(allow);
}

bool NativeScrollView::GetAllowKeyboardScrolling() {
  if (scroll_view_impl_)
    return scroll_view_impl_->GetAllowKeyboardScrolling();
  return true;
}

void NativeScrollView::SetDrawOverflowIndicator(bool indicator) {
  if (scroll_view_impl_)
    scroll_view_impl_->SetDrawOverflowIndicator(indicator);
}

bool NativeScrollView::GetDrawOverflowIndicator() {
  if (scroll_view_impl_)
    return scroll_view_impl_->GetDrawOverflowIndicator();
  return true;
}

}  // namespace electron
