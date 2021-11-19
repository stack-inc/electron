// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_API_ELECTRON_API_SCROLL_VIEW_H_
#define SHELL_BROWSER_API_ELECTRON_API_SCROLL_VIEW_H_

#include "shell/browser/api/electron_api_container_view.h"
#include "shell/browser/ui/native_scroll_view.h"

namespace gfx {
class Size;
}

namespace electron {

namespace api {

class ScrollView : public ContainerView {
 public:
  static gin_helper::WrappableBase* New(gin_helper::ErrorThrower thrower,
                                        gin::Arguments* args);

  static void BuildPrototype(v8::Isolate* isolate,
                             v8::Local<v8::FunctionTemplate> prototype);

 protected:
  ScrollView(gin::Arguments* args, NativeScrollView* scroll_view);
  ~ScrollView() override;

 private:
  void SetContentView(v8::Local<v8::Value> value);
  v8::Local<v8::Value> GetContentView() const;
  void SetContentSize(gfx::Size size);
  gfx::Size GetContentSize();
  int GetMinHeight();
  int GetMaxHeight();
  void ClipHeightTo(int min_height, int max_height);
  gfx::Rect GetVisibleRect();
  void SetHorizontalScrollBarMode(std::string mode);
  std::string GetHorizontalScrollBarMode();
  void SetVerticalScrollBarMode(std::string mode);
  std::string GetVerticalScrollBarMode();
  void SetAllowKeyboardScrolling(bool allow);
  bool GetAllowKeyboardScrolling();
  void SetDrawOverflowIndicator(bool indicator);
  bool GetDrawOverflowIndicator();

  NativeScrollView* scroll_view_;

  int32_t content_view_id_ = 0;
  v8::Global<v8::Value> content_view_;

  DISALLOW_COPY_AND_ASSIGN(ScrollView);
};

}  // namespace api

}  // namespace electron

#endif  // SHELL_BROWSER_API_ELECTRON_API_SCROLL_VIEW_H_
