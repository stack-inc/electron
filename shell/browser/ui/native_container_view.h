// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_UI_NATIVE_CONTAINER_VIEW_H_
#define SHELL_BROWSER_UI_NATIVE_CONTAINER_VIEW_H_

#include <vector>

#include "shell/browser/ui/native_view.h"

namespace electron {

class NativeContainerView : public NativeView {
 public:
  NativeContainerView();

  // NativeView:
  bool IsContainer() const override;
  void DetachChildView(NativeView* view) override;
  void TriggerBeforeunloadEvents() override;
#if defined(OS_MAC)
  void UpdateDraggableRegions() override;
#endif

  // Add/Remove children.
  void AddChildView(scoped_refptr<NativeView> view);
  void AddChildViewAt(scoped_refptr<NativeView> view, int index);
  bool RemoveChildView(NativeView* view);
  void SetTopChildView(NativeView* view);

  // Get children.
  int ChildCount() const { return static_cast<int>(children_.size()); }
  NativeView* ChildAt(int index) const {
    if (index < 0 || index >= ChildCount())
      return nullptr;
    return children_[index].get();
  }

#if 0
  // Events.
  Signal<void(Container*, Painter*, gfx::RectF)> on_draw;
#endif

 protected:
  ~NativeContainerView() override;

  // Empty constructor used by subclasses.
  explicit NativeContainerView(const char* an_empty_constructor);

  // NativeView:
  void SetWindowForChildren(NativeWindow* window) override;

  void PlatformInit();
  void PlatformDestroy();
  void PlatformAddChildView(NativeView* view);
  void PlatformRemoveChildView(NativeView* view);
  void PlatformSetTopView(NativeView* view);

 private:
  // The view layer.
  std::vector<scoped_refptr<NativeView>> children_;
};

}  // namespace electron

#endif  // SHELL_BROWSER_UI_NATIVE_CONTAINER_VIEW_H_
