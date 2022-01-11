// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_UI_NATIVE_SCROLL_H_
#define SHELL_BROWSER_UI_NATIVE_SCROLL_H_

#include <tuple>

#include "shell/browser/ui/native_view.h"

namespace electron {

enum class ScrollBarMode { kDisabled, kHiddenButEnabled, kEnabled };

class NativeScroll : public NativeView {
 public:
  NativeScroll();

  // NativeView:
  void DetachChildView(NativeBrowserView* view) override;
  void DetachChildView(NativeView* view) override;
  void TriggerBeforeunloadEvents() override;
#if defined(OS_MAC)
  void UpdateDraggableRegions() override;
#endif

  void SetContentView(scoped_refptr<NativeView> view);
  NativeView* GetContentView() const;

  void SetContentSize(const gfx::Size& size);
  gfx::Size GetContentSize() const;
  void SetScrollPosition(float horizon, float vertical);
  std::tuple<float, float> GetScrollPosition() const;
  std::tuple<float, float> GetMaximumScrollPosition() const;

#if defined(OS_MAC)
  void SetOverlayScrollbar(bool overlay);
  bool IsOverlayScrollbar() const;
#endif

  void SetHorizontalScrollBarMode(ScrollBarMode mode);
  ScrollBarMode GetHorizontalScrollBarMode();
  void SetVerticalScrollBarMode(ScrollBarMode mode);
  ScrollBarMode GetVerticalScrollBarMode();

#if defined(OS_MAC)
  enum class Elasticity {
    Automatic = 0,  // NSScrollElasticityAutomatic = 0
    None = 1,       // NSScrollElasticityNone = 1
    Allowed = 2,    // NSScrollElasticityAllowed = 2
  };
  void SetScrollElasticity(Elasticity h, Elasticity v);
  std::tuple<Elasticity, Elasticity> GetScrollElasticity() const;
#endif

#if !defined(OS_MAC)
  int GetMinHeight();
  int GetMaxHeight();
  void ClipHeightTo(int min_height, int max_height);
  gfx::Rect GetVisibleRect();
  void SetAllowKeyboardScrolling(bool allow);
  bool GetAllowKeyboardScrolling();
  void SetDrawOverflowIndicator(bool indicator);
  bool GetDrawOverflowIndicator();
#endif

#if 0
  // Events.
  Signal<bool(Scroll*)> on_scroll;
#endif

 protected:
  ~NativeScroll() override;

  // NativeView:
  void SetWindowForChildren(NativeWindow* window) override;

  // Following platform implementations should only be called by wrappers.
  void PlatformInit();
  void PlatformSetContentView(NativeView* container);

  enum { kOnScroll };

#if 0
  // SignalDelegate:
  void OnConnect(int identifier) override;
#endif

 private:
  scoped_refptr<NativeView> content_view_;
};

}  // namespace electron

#endif  // SHELL_BROWSER_UI_NATIVE_SCROLL_H_
