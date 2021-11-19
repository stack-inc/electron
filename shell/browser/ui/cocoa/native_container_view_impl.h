// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_UI_COCOA_NATIVE_CONTAINER_VIEW_IMPL_H_
#define SHELL_BROWSER_UI_COCOA_NATIVE_CONTAINER_VIEW_IMPL_H_

#include "shell/browser/ui/cocoa/native_base_view_impl.h"
#include "shell/browser/ui/native_container.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/native_widget_types.h"

namespace electron {
 
class NativeBrowserView;
class NativeContainerView;

class NativeContainerViewImpl : public NativeBaseViewImpl {
 public:
  explicit NativeContainerViewImpl(NativeContainer* container);
  explicit NativeContainerViewImpl(NativeView* view);

  NativeContainerViewImpl(const NativeContainerViewImpl&) = delete;
  NativeContainerViewImpl& operator=(
      const NativeContainerViewImpl&) = delete;

  void AddBrowserView(NativeBrowserView* view);
  void RemoveBrowserView(NativeBrowserView* view);
  void SetTopBrowserView(NativeBrowserView* view);
  void AddContainerView(NativeContainerViewImpl* view);
  void RemoveContainerView(NativeContainerViewImpl* view);
  void SetTopContainerView(NativeContainerViewImpl* view);

 private:
  NativeContainer* container_;
};

}  // namespace electron

#endif  // SHELL_BROWSER_UI_COCOA_NATIVE_CONTAINER_VIEW_IMPL_H_
