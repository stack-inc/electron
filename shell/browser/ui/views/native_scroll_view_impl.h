// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_UI_VIEWS_NATIVE_SCROLL_VIEW_IMPL_H_
#define SHELL_BROWSER_UI_VIEWS_NATIVE_SCROLL_VIEW_IMPL_H_

#include "shell/browser/ui/native_scroll_view.h"
#include "shell/browser/ui/views/native_container_view_impl.h"
#include "ui/views/controls/scroll_view.h"

namespace electron {

class NativeScrollViewImpl : public NativeContainerViewImpl {
 public:
  explicit NativeScrollViewImpl(views::ScrollView* view);
  ~NativeScrollViewImpl() override;

  void SetContentView(NativeContainerViewImpl* view);
  NativeContainerViewImpl* GetContentView();
  void SetContentSize(gfx::Size size);
  gfx::Size GetContentSize();
  int GetMinHeight();
  int GetMaxHeight();
  void ClipHeightTo(int min_height, int max_height);
  gfx::Rect GetVisibleRect();
  void SetHorizontalScrollBarMode(ScrollBarMode mode);
  ScrollBarMode GetHorizontalScrollBarMode();
  void SetVerticalScrollBarMode(ScrollBarMode mode);
  ScrollBarMode GetVerticalScrollBarMode();
  void SetAllowKeyboardScrolling(bool allow);
  bool GetAllowKeyboardScrolling();
  void SetDrawOverflowIndicator(bool indicator);
  bool GetDrawOverflowIndicator();

 private:
  ScrollBarMode GetScrollBarMode(views::ScrollView::ScrollBarMode mode);

  views::ScrollView* scroll_view_;
  NativeContainerViewImpl* content_view_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(NativeScrollViewImpl);
};

}  // namespace electron

#endif  // SHELL_BROWSER_UI_VIEWS_NATIVE_SCROLL_VIEW_IMPL_H_
