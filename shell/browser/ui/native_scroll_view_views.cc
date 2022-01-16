// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_scroll_view.h"

#include "ui/gfx/geometry/size.h"
#include "ui/views/controls/scroll_view.h"

namespace electron {

namespace {

ScrollBarMode GetScrollBarMode(
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

}  // namespace

void NativeScrollView::PlatformInit() {
  TakeOverView(new views::ScrollView());
}

void NativeScrollView::PlatformSetContentView(NativeView* view) {
  if (!view)
    return;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  view->set_delete_view(false);
  auto content_view = std::unique_ptr<views::View>(view->GetNative());
  scroll->SetContents(std::move(content_view));
}

void NativeScrollView::PlatformDetachChildView() {
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  scroll->SetContents(nullptr);
  content_view_->set_delete_view(true);
}

void NativeScrollView::SetContentSize(const gfx::Size& size) {
  if (!content_view_.get())
    return;
  content_view_->GetNative()->SetSize(size);
  Layout();
}

void NativeScrollView::SetScrollPosition(float horizon, float vertical) {
}

std::tuple<float, float> NativeScrollView::GetScrollPosition() const {
  return std::make_tuple(0, 0);
}

std::tuple<float, float> NativeScrollView::GetMaximumScrollPosition() const {
  return std::make_tuple(0, 0);
}

void NativeScrollView::SetHorizontalScrollBarMode(ScrollBarMode mode) {
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
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
  scroll->SetHorizontalScrollBarMode(scroll_bar_mode);
}

ScrollBarMode NativeScrollView::GetHorizontalScrollBarMode() {
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  return GetScrollBarMode(scroll->GetHorizontalScrollBarMode());
}

void NativeScrollView::SetVerticalScrollBarMode(ScrollBarMode mode) {
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
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
  scroll->SetVerticalScrollBarMode(scroll_bar_mode);
}

ScrollBarMode NativeScrollView::GetVerticalScrollBarMode() {
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  return GetScrollBarMode(scroll->GetVerticalScrollBarMode());
}

#if 0
void NativeScrollView::OnConnect(int identifier) {
  if (identifier == kOnScroll) {
    auto* scroll = static_cast<ElectronNativeScrollView*>(GetNative());
    [scroll setEventEnabled:YES];
  }
}
#endif

int NativeScrollView::GetMinHeight() {
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  return scroll->GetMinHeight();
}

int NativeScrollView::GetMaxHeight() {
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  return scroll->GetMaxHeight();
}

void NativeScrollView::ClipHeightTo(int min_height, int max_height) {
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  scroll->ClipHeightTo(min_height, max_height);
}

gfx::Rect NativeScrollView::GetVisibleRect() {
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  return scroll->GetVisibleRect();
}

void NativeScrollView::SetAllowKeyboardScrolling(bool allow) {
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  scroll->SetAllowKeyboardScrolling(allow);
}

bool NativeScrollView::GetAllowKeyboardScrolling() {
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  return scroll->GetAllowKeyboardScrolling();
}

void NativeScrollView::SetDrawOverflowIndicator(bool indicator) {
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  scroll->SetDrawOverflowIndicator(indicator);
}

bool NativeScrollView::GetDrawOverflowIndicator() {
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  return scroll->GetDrawOverflowIndicator();
}

}  // namespace electron
