// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_UI_NATIVE_CONTAINER_H_
#define SHELL_BROWSER_UI_NATIVE_CONTAINER_H_

#include <vector>

#include "shell/browser/ui/native_view.h"

namespace electron {

class NativeContainer : public NativeView {
 public:
  NativeContainer();

  // NativeView:
  void Layout() override;
  bool IsContainer() const override;
  void OnSizeChanged() override;
  void DetachChildView(NativeView* view) override;
  void TriggerBeforeunloadEvents() override;
#if defined(OS_MAC)
  void UpdateDraggableRegions() override;
#endif

  // Gets preferred size of view.
  gfx::Size GetPreferredSize() const;

  // Returns the preferred width/height for the specified height/width.
  float GetPreferredHeightForWidth(float width) const;
  float GetPreferredWidthForHeight(float height) const;

  // Add/Remove children.
  void AddChildView(scoped_refptr<NativeView> view);
  void AddChildViewAt(scoped_refptr<NativeView> view, int index);
  void RemoveChildView(NativeView* view);
  void SetTopChildView(NativeView* view);

  // Get children.
  int ChildCount() const { return static_cast<int>(children_.size()); }
  NativeView* ChildAt(int index) const {
    if (index < 0 || index >= ChildCount())
      return nullptr;
    return children_[index].get();
  }

  // Internal: Used by certain implementations to refresh layout.
  void SetChildBoundsFromCSS();

#if 0
  // Events.
  Signal<void(Container*, Painter*, gfx::RectF)> on_draw;
#endif

 protected:
  ~NativeContainer() override;

  // Empty constructor used by subclasses.
  explicit NativeContainer(const char* an_empty_constructor);

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

  // Whether the container should update children's layout.
  bool dirty_ = false;
};

}  // namespace electron

#endif  // SHELL_BROWSER_UI_NATIVE_CONTAINER_H_
