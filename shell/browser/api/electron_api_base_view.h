// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_API_ELECTRON_API_BASE_VIEW_H_
#define SHELL_BROWSER_API_ELECTRON_API_BASE_VIEW_H_

#include <memory>
#include <string>

#include "shell/browser/ui/native_view.h"
#include "shell/common/gin_helper/error_thrower.h"
#include "shell/common/gin_helper/pinnable.h"
#include "shell/common/gin_helper/wrappable.h"

#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
#include "ui/views/view_observer.h"
#endif

namespace gfx {
class Rect;
}

namespace gin {
class Arguments;
template <typename T>
class Handle;
}  // namespace gin

namespace electron {

namespace api {

class BaseView : public gin_helper::Wrappable<BaseView>,
                      public gin_helper::Pinnable<BaseView>
#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
                      , public views::ViewObserver
#endif
{
 public:
  static gin_helper::WrappableBase* New(gin_helper::ErrorThrower thrower,
                                        gin::Arguments* args);

  static void BuildPrototype(v8::Isolate* isolate,
                             v8::Local<v8::FunctionTemplate> prototype);

  NativeView* view() const { return view_.get(); }

  int32_t ID() const { return id_; }

 protected:
  BaseView(gin::Arguments* args, NativeView* native_view);
  ~BaseView() override;

#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
  // views::ViewObserver:
  void OnViewIsDeleting(views::View* observed_view) override;
#endif

 private:
  friend class ScrollView;

  void SetBounds(const gfx::Rect& bounds);
  gfx::Rect GetBounds() const;
#if defined(OS_MAC)
  gfx::Point OffsetFromView(gin::Handle<BaseView> from) const;
  gfx::Point OffsetFromWindow() const;
#endif
  void SetVisible(bool visible);
  bool IsVisible() const;
  bool IsTreeVisible() const;
  void Focus();
  bool HasFocus() const;
  void SetFocusable(bool focusable);
  bool IsFocusable() const;
  void SetBackgroundColor(const std::string& color_name);
  void SetStringProperty(const std::string& name, const std::string& value);
  void SetNumericProperty(const std::string& name, float value);
  void Layout();

  int32_t id_;

  scoped_refptr<NativeView> view_;

  DISALLOW_COPY_AND_ASSIGN(BaseView);
};

}  // namespace api

}  // namespace electron

#endif  // SHELL_BROWSER_API_ELECTRON_API_BASE_VIEW_H_
