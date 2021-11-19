// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_UI_VIEWS_NATIVE_CONTAINER_VIEW_IMPL_H_
#define SHELL_BROWSER_UI_VIEWS_NATIVE_CONTAINER_VIEW_IMPL_H_

#include <memory>
#include <vector>

#include "ui/views/view.h"

namespace views {
class LayoutManager;
}

namespace electron {

class NativeBrowserView;
class NativeContainerView;

class NativeContainerViewImpl {
 public:
  explicit NativeContainerViewImpl(views::View* view);
  virtual ~NativeContainerViewImpl();

  NativeContainerViewImpl(const NativeContainerViewImpl&) = delete;
  NativeContainerViewImpl& operator=(
      const NativeContainerViewImpl&) = delete;

  void SetAutoResizeProportions(const gfx::Size& window_size);
  void AutoResize(const gfx::Rect& new_window,
                  int width_delta,
                  int height_delta);
  uint8_t GetAutoResizeFlags() { return auto_resize_flags_; }

  views::View* GetView();
  void SetAutoResizeFlags(uint8_t flags);
  void SetBounds(const gfx::Rect& bounds);
  gfx::Rect GetBounds();
  void SetBackgroundColor(SkColor color);

  void AddBrowserView(NativeBrowserView* view);
  void RemoveBrowserView(NativeBrowserView* view);
  void SetTopBrowserView(NativeBrowserView* view);
  void AddContainerView(NativeContainerViewImpl* view);
  void RemoveContainerView(NativeContainerViewImpl* view);
  void SetTopContainerView(NativeContainerViewImpl* view);

  void SetLayoutManager(views::LayoutManager* layout_manager);

  // Should delete the |view_| in destructor.
  void set_delete_view(bool should) { delete_view_ = should; }

 private:
  void ResetAutoResizeProportions();

  bool delete_view_ = true;
  views::View* view_;

  uint8_t auto_resize_flags_ = 0;

  bool auto_horizontal_proportion_set_ = false;
  float auto_horizontal_proportion_width_ = 0.;
  float auto_horizontal_proportion_left_ = 0.;

  bool auto_vertical_proportion_set_ = false;
  float auto_vertical_proportion_height_ = 0.;
  float auto_vertical_proportion_top_ = 0.;
};

}  // namespace electron

#endif  // SHELL_BROWSER_UI_VIEWS_NATIVE_CONTAINER_VIEW_IMPL_H_
