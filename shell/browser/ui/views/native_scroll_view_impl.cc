// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/views/native_scroll_view_impl.h"

namespace electron {

NativeScrollViewImpl::NativeScrollViewImpl(views::ScrollView* view)
    : NativeContainerViewImpl(view), scroll_view_(view) {}

NativeScrollViewImpl::~NativeScrollViewImpl() = default;

void NativeScrollViewImpl::SetContentView(NativeContainerViewImpl* view) {
  DCHECK(scroll_view_);

  if (!view)
    return;

  view->set_delete_view(false);
  content_view_ = view;
  auto content_view = std::unique_ptr<views::View>(view->GetView());
  scroll_view_->SetContents(std::move(content_view));
}

NativeContainerViewImpl* NativeScrollViewImpl::GetContentView() {
  DCHECK(scroll_view_);
  DCHECK_EQ(content_view_->GetView(), scroll_view_->contents());
  return content_view_;
}

void NativeScrollViewImpl::SetContentSize(gfx::Size size) {
  if (content_view_)
    content_view_->GetView()->SetSize(size);
}

gfx::Size NativeScrollViewImpl::GetContentSize() {
  if (content_view_)
    return content_view_->GetView()->size();
  return gfx::Size();
}

int NativeScrollViewImpl::GetMinHeight() {
  DCHECK(scroll_view_);
  return scroll_view_->GetMinHeight();
}

int NativeScrollViewImpl::GetMaxHeight() {
  DCHECK(scroll_view_);
  return scroll_view_->GetMaxHeight();
}

void NativeScrollViewImpl::ClipHeightTo(int min_height, int max_height) {
  DCHECK(scroll_view_);
  scroll_view_->ClipHeightTo(min_height, max_height);
}

gfx::Rect NativeScrollViewImpl::GetVisibleRect() {
  DCHECK(scroll_view_);
  return scroll_view_->GetVisibleRect();
}

ScrollBarMode NativeScrollViewImpl::GetScrollBarMode(
    views::ScrollView::ScrollBarMode mode) {
  switch (mode) {
    case views::ScrollView::ScrollBarMode::kDisabled:
      return ScrollBarMode::kDisabled;
    case views::ScrollView::ScrollBarMode::kHiddenButEnabled:
      return ScrollBarMode::kHiddenButEnabled;
    case views::ScrollView::ScrollBarMode::kEnabled:
      return ScrollBarMode::kEnabled;
  }
}

void NativeScrollViewImpl::SetHorizontalScrollBarMode(ScrollBarMode mode) {
  DCHECK(scroll_view_);
  views::ScrollView::ScrollBarMode scroll_bar_mode =
      views::ScrollView::ScrollBarMode::kEnabled;
  switch (mode) {
    case ScrollBarMode::kDisabled:
      scroll_bar_mode = views::ScrollView::ScrollBarMode::kDisabled;
      break;
    case ScrollBarMode::kHiddenButEnabled:
      scroll_bar_mode = views::ScrollView::ScrollBarMode::kHiddenButEnabled;
      break;
    case ScrollBarMode::kEnabled:
      scroll_bar_mode = views::ScrollView::ScrollBarMode::kEnabled;
  }
  scroll_view_->SetHorizontalScrollBarMode(scroll_bar_mode);
}

ScrollBarMode NativeScrollViewImpl::GetHorizontalScrollBarMode() {
  DCHECK(scroll_view_);
  return GetScrollBarMode(scroll_view_->GetHorizontalScrollBarMode());
}

void NativeScrollViewImpl::SetVerticalScrollBarMode(ScrollBarMode mode) {
  DCHECK(scroll_view_);
  views::ScrollView::ScrollBarMode scroll_bar_mode =
      views::ScrollView::ScrollBarMode::kEnabled;
  switch (mode) {
    case ScrollBarMode::kDisabled:
      scroll_bar_mode = views::ScrollView::ScrollBarMode::kDisabled;
      break;
    case ScrollBarMode::kHiddenButEnabled:
      scroll_bar_mode = views::ScrollView::ScrollBarMode::kHiddenButEnabled;
      break;
    case ScrollBarMode::kEnabled:
      scroll_bar_mode = views::ScrollView::ScrollBarMode::kEnabled;
  }
  scroll_view_->SetVerticalScrollBarMode(scroll_bar_mode);
}

ScrollBarMode NativeScrollViewImpl::GetVerticalScrollBarMode() {
  DCHECK(scroll_view_);
  return GetScrollBarMode(scroll_view_->GetVerticalScrollBarMode());
}

void NativeScrollViewImpl::SetAllowKeyboardScrolling(bool allow) {
  DCHECK(scroll_view_);
  scroll_view_->SetAllowKeyboardScrolling(allow);
}

bool NativeScrollViewImpl::GetAllowKeyboardScrolling() {
  DCHECK(scroll_view_);
  return scroll_view_->GetAllowKeyboardScrolling();
}

void NativeScrollViewImpl::SetDrawOverflowIndicator(bool indicator) {
  DCHECK(scroll_view_);
  scroll_view_->SetDrawOverflowIndicator(indicator);
}

bool NativeScrollViewImpl::GetDrawOverflowIndicator() {
  DCHECK(scroll_view_);
  return scroll_view_->GetDrawOverflowIndicator();
}

NativeScrollView* NativeScrollView::Create() {
  return new NativeScrollView(
      new NativeScrollViewImpl(new views::ScrollView()));
}

}  // namespace electron
