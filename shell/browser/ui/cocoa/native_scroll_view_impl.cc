// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/cocoa/native_scroll_view_impl.h"

namespace electron {

NativeScrollViewImpl::NativeScrollViewImpl(NativeScroll* scroll)
    : NativeContainerViewImpl(scroll),
    scroll_(scroll) {
  DCHECK(scroll_);
}

NativeScrollViewImpl::~NativeScrollViewImpl() = default;

void NativeScrollViewImpl::SetContentView(NativeContainerViewImpl* view) {
  DCHECK(scroll_);

  if (!view)
    return;

  content_view_ = view;
  scroll_->SetContentView(view->GetView());
}

NativeContainerViewImpl* NativeScrollViewImpl::GetContentView() {
  DCHECK(scroll_);
  DCHECK_EQ(content_view_->GetView(), scroll_->GetContentView());
  return content_view_;
}

void NativeScrollViewImpl::SetContentSize(gfx::Size size) {
  DCHECK(scroll_);
  scroll_->SetContentSize(gfx::SizeF(size.width(), size.height()));
}

gfx::Size NativeScrollViewImpl::GetContentSize() {
  DCHECK(scroll_);
  gfx::SizeF size = scroll_->GetContentSize();
  return gfx::Size(size.width(), size.height());
}

int NativeScrollViewImpl::GetMinHeight() {
#if 0
  DCHECK(scroll_view_);
  return scroll_view_->GetMinHeight();
#else
return 0;
#endif
}

int NativeScrollViewImpl::GetMaxHeight() {
#if 0
  DCHECK(scroll_view_);
  return scroll_view_->GetMaxHeight();
#else
return 0;
#endif
}

void NativeScrollViewImpl::ClipHeightTo(int min_height, int max_height) {
#if 0
  DCHECK(scroll_view_);
  scroll_view_->ClipHeightTo(min_height, max_height);
#endif
}

gfx::Rect NativeScrollViewImpl::GetVisibleRect() {
#if 0
  DCHECK(scroll_view_);
  return scroll_view_->GetVisibleRect();
#else
return gfx::Rect();
#endif
}

ScrollBarMode NativeScrollViewImpl::GetScrollBarMode(
    NativeScroll::Policy policy) {
  switch (policy) {
    case NativeScroll::Policy::Never:
      return ScrollBarMode::kDisabled;
    case NativeScroll::Policy::Automatic:
      return ScrollBarMode::kHiddenButEnabled;
    case NativeScroll::Policy::Always:
      return ScrollBarMode::kEnabled;
  }
}

void NativeScrollViewImpl::SetHorizontalScrollBarMode(ScrollBarMode mode) {
  DCHECK(scroll_);
  NativeScroll::Policy policy = NativeScroll::Policy::Always;
  switch (mode) {
    case ScrollBarMode::kDisabled:
      policy = NativeScroll::Policy::Never;
      break;
    case ScrollBarMode::kHiddenButEnabled:
      policy = NativeScroll::Policy::Automatic;
      break;
    case ScrollBarMode::kEnabled:
      policy = NativeScroll::Policy::Always;
  }
  std::tuple<NativeScroll::Policy, NativeScroll::Policy> policies = scroll_->GetScrollbarPolicy();
  scroll_->SetScrollbarPolicy(policy, std::get<1>(policies));
}

ScrollBarMode NativeScrollViewImpl::GetHorizontalScrollBarMode() {
  DCHECK(scroll_);
  std::tuple<NativeScroll::Policy, NativeScroll::Policy> policies = scroll_->GetScrollbarPolicy();
  return GetScrollBarMode(std::get<0>(policies));
}

void NativeScrollViewImpl::SetVerticalScrollBarMode(ScrollBarMode mode) {
  DCHECK(scroll_);
  NativeScroll::Policy policy = NativeScroll::Policy::Always;
  switch (mode) {
    case ScrollBarMode::kDisabled:
      policy = NativeScroll::Policy::Never;
      break;
    case ScrollBarMode::kHiddenButEnabled:
      policy = NativeScroll::Policy::Automatic;
      break;
    case ScrollBarMode::kEnabled:
      policy = NativeScroll::Policy::Always;
  }
  std::tuple<NativeScroll::Policy, NativeScroll::Policy> policies = scroll_->GetScrollbarPolicy();
  scroll_->SetScrollbarPolicy(std::get<0>(policies), policy);
}

ScrollBarMode NativeScrollViewImpl::GetVerticalScrollBarMode() {
  DCHECK(scroll_);
  std::tuple<NativeScroll::Policy, NativeScroll::Policy> policies = scroll_->GetScrollbarPolicy();
  return GetScrollBarMode(std::get<1>(policies));
}

void NativeScrollViewImpl::SetAllowKeyboardScrolling(bool allow) {
#if 0
  DCHECK(scroll_view_);
  scroll_view_->SetAllowKeyboardScrolling(allow);
#endif
}

bool NativeScrollViewImpl::GetAllowKeyboardScrolling() {
#if 0
  DCHECK(scroll_view_);
  return scroll_view_->GetAllowKeyboardScrolling();
#else
return false;
#endif
}

void NativeScrollViewImpl::SetDrawOverflowIndicator(bool indicator) {
#if 0
  DCHECK(scroll_view_);
  scroll_view_->SetDrawOverflowIndicator(indicator);
#endif
}

bool NativeScrollViewImpl::GetDrawOverflowIndicator() {
#if 0
  DCHECK(scroll_view_);
  return scroll_view_->GetDrawOverflowIndicator();
#else
return false;
#endif
}

NativeScrollView* NativeScrollView::Create() {
  return new NativeScrollView(
      new NativeScrollViewImpl(new NativeScroll()));
}

}  // namespace electron
