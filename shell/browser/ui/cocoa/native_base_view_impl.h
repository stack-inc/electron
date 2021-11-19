// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_UI_COCOA_NATIVE_BASE_VIEW_IMPL_H_
#define SHELL_BROWSER_UI_COCOA_NATIVE_BASE_VIEW_IMPL_H_

#include "shell/browser/ui/native_view.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Rect;
}

namespace electron {
 
class NativeBaseViewImpl {
 public:
  explicit NativeBaseViewImpl(NativeView* view);
  virtual ~NativeBaseViewImpl();

  NativeBaseViewImpl(const NativeBaseViewImpl&) = delete;
  NativeBaseViewImpl& operator=(
      const NativeBaseViewImpl&) = delete;

  NativeView* GetView() const;
  void SetAutoResizeFlags(uint8_t flags);
  void SetBounds(const gfx::Rect& bounds);
  gfx::Rect GetBounds();
  void SetBackgroundColor(SkColor color);
  void SetStyleProperty(const std::string& name, const std::string& value);
  void SetStyleProperty(const std::string& name, float value);
  void Layout();

 private:
  scoped_refptr<NativeView> view_;
};

}  // namespace electron

#endif  // SHELL_BROWSER_UI_COCOA_NATIVE_BASE_VIEW_IMPL_H_
