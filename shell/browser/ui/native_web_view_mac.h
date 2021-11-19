#ifndef SHELL_BROWSER_UI_NATIVE_WEB_VIEW_H_
#define SHELL_BROWSER_UI_NATIVE_WEB_VIEW_H_

#include "shell/browser/native_browser_view.h"
#include "shell/browser/ui/native_view.h"

namespace electron {

class NativeWebView : public NativeView {
 public:
  explicit NativeWebView(NativeBrowserView* browser_view);

  // View class name.
  static const char kClassName[];

  // NativeView:
  const char* GetClassName() const override;
  void SetBounds(const gfx::RectF& bounds) override;

 protected:
  ~NativeWebView() override;

 private:
  NativeBrowserView* browser_view_;
};

}  // namespace electron

#endif  // SHELL_BROWSER_UI_NATIVE_WEB_VIEW_H_
