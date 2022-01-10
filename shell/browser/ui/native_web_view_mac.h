// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_UI_NATIVE_WEB_VIEW_H_
#define SHELL_BROWSER_UI_NATIVE_WEB_VIEW_H_

#include "shell/browser/native_browser_view.h"
#include "shell/browser/ui/native_view.h"

namespace electron {

class NativeWebView : public NativeView {
 public:
  explicit NativeWebView(NativeBrowserView* browser_view);

  // NativeView:
  void SetBounds(const gfx::Rect& bounds) override;

 protected:
  ~NativeWebView() override;

 private:
  NativeBrowserView* browser_view_;
};

}  // namespace electron

#endif  // SHELL_BROWSER_UI_NATIVE_WEB_VIEW_H_
