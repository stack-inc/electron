// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_UI_NATIVE_SCROLL_VIEW_H_
#define SHELL_BROWSER_UI_NATIVE_SCROLL_VIEW_H_

#include "shell/browser/ui/native_container_view.h"

namespace electron {

class NativeScrollViewImpl;

enum class ScrollBarMode { kDisabled, kHiddenButEnabled, kEnabled };

class NativeScrollView : public NativeContainerView {
 public:
  ~NativeScrollView() override;

  static NativeScrollView* Create();

  void SetContentView(NativeContainerView* view);
  NativeContainerView* GetContentView();
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

 protected:
  explicit NativeScrollView(NativeScrollViewImpl* view_impl);

  NativeScrollViewImpl* scroll_view_impl_;
  NativeContainerView* content_view_ = nullptr;

 private:
  DISALLOW_COPY_AND_ASSIGN(NativeScrollView);
};

}  // namespace electron

#endif  // SHELL_BROWSER_UI_NATIVE_SCROLL_VIEW_H_
