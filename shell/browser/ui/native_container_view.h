// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_UI_NATIVE_CONTAINER_VIEW_H_
#define SHELL_BROWSER_UI_NATIVE_CONTAINER_VIEW_H_

#include <vector>

#include "base/macros.h"
#include "shell/browser/native_browser_view.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Rect;
}

#if defined(OS_MAC)
namespace electron {
class NativeView;
}
#elif defined(TOOLKIT_VIEWS)
namespace views {
class LayoutManager;
class View;
}
#endif

namespace electron {

class NativeBrowserView;
class NativeContainerViewImpl;
class NativeWindow;

class NativeContainerView {
 public:
  virtual ~NativeContainerView();

  static NativeContainerView* Create();

#if defined(OS_MAC)
  NativeView* GetView() const;
#elif defined(TOOLKIT_VIEWS)
  views::View* GetView() const;
#endif

  NativeContainerViewImpl* GetViewImpl();
  void SetAutoResizeFlags(uint8_t flags);
  void SetBounds(const gfx::Rect& bounds);
  gfx::Rect GetBounds();
  void SetBackgroundColor(SkColor color);

  void AddBrowserView(NativeBrowserView* view);
  void RemoveBrowserView(NativeBrowserView* view);
  void SetTopBrowserView(NativeBrowserView* view);
  void AddContainerView(NativeContainerView* view);
  void RemoveContainerView(NativeContainerView* view);
  void SetTopContainerView(NativeContainerView* view);

#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
  void SetLayoutManager(views::LayoutManager* layout_manager);
#endif

  void SetStyleProperty(const std::string& name, const std::string& value);
  void SetStyleProperty(const std::string& name, float value);
  void Layout();

  NativeContainerView* parent_view() { return parent_view_; }
  void set_parent_view(NativeContainerView* parent) { parent_view_ = parent; }

  void set_owner_window(NativeWindow* window) { owner_window_ = window; }

  void SetOwnerWindow(NativeWindow* window);
  NativeWindow* GetOwnerWindow();

  void TriggerBeforeunloadEvents();

#if defined(OS_MAC)
  void UpdateDraggableRegions();
#endif

 protected:
  explicit NativeContainerView(NativeContainerViewImpl* view_impl);

  void SetOwnerWindowForChildren(NativeWindow* window);

  std::list<NativeBrowserView*> browser_views() const { return browser_views_; }

  std::list<NativeContainerView*> container_views() const {
    return container_views_;
  }

  void add_browser_view(NativeBrowserView* browser_view) {
    browser_views_.push_back(browser_view);
  }
  void remove_browser_view(NativeBrowserView* browser_view) {
    browser_views_.remove_if(
        [&browser_view](NativeBrowserView* n) { return (n == browser_view); });
  }

  void add_container_view(NativeContainerView* container_view) {
    container_views_.push_back(container_view);
  }
  void remove_container_view(NativeContainerView* container_view) {
    container_views_.remove_if([&container_view](NativeContainerView* n) {
      return (n == container_view);
    });
  }

  NativeContainerViewImpl* container_view_impl_;

  // The browser view layer.
  std::list<NativeBrowserView*> browser_views_;

  // The container view layer.
  std::list<NativeContainerView*> container_views_;

  NativeContainerView* parent_view_ = nullptr;
  NativeWindow* owner_window_ = nullptr;

 private:
  DISALLOW_COPY_AND_ASSIGN(NativeContainerView);
};

}  // namespace electron

#endif  // SHELL_BROWSER_UI_NATIVE_CONTAINER_VIEW_H_
