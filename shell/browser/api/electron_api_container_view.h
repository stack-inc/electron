// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_API_ELECTRON_API_CONTAINER_VIEW_H_
#define SHELL_BROWSER_API_ELECTRON_API_CONTAINER_VIEW_H_

#include <memory>
#include <string>
#include <vector>

#include "shell/browser/ui/native_container_view.h"
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

class NativeWindow;

namespace api {

class ContainerView : public gin_helper::Wrappable<ContainerView>,
                      public gin_helper::Pinnable<ContainerView>
#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
                      , public views::ViewObserver
#endif
{
 public:
  static gin_helper::WrappableBase* New(gin_helper::ErrorThrower thrower,
                                        gin::Arguments* args);

  static void BuildPrototype(v8::Isolate* isolate,
                             v8::Local<v8::FunctionTemplate> prototype);

  NativeContainerView* view() const { return view_.get(); }

  int32_t ID() const { return id_; }

  void DetachFromParent(NativeWindow* window);

 protected:
  ContainerView(gin::Arguments* args, NativeContainerView* container_view);
  ~ContainerView() override;

#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
  // views::ViewObserver:
  void OnViewIsDeleting(views::View* observed_view) override;
#endif

 private:
  friend class ScrollView;

  void SetAutoResize(AutoResizeFlags flags);
  void SetBounds(const gfx::Rect& bounds);
  gfx::Rect GetBounds();
  void SetBackgroundColor(const std::string& color_name);

  void AddBrowserView(v8::Local<v8::Value> value);
  void RemoveBrowserView(v8::Local<v8::Value> value);
  void SetTopBrowserView(v8::Local<v8::Value> value,
                         gin_helper::ErrorThrower thrower);
  std::vector<v8::Local<v8::Value>> GetBrowserViews() const;
  void AddContainerView(v8::Local<v8::Value> value);
  void RemoveContainerView(v8::Local<v8::Value> value);
  void SetTopContainerView(v8::Local<v8::Value> value,
                           gin_helper::ErrorThrower thrower);
  std::vector<v8::Local<v8::Value>> GetContainerViews() const;
  void SetStringProperty(const std::string& name, const std::string& value);
  void SetNumericProperty(const std::string& name, float value);
  void Layout();

  std::unique_ptr<NativeContainerView> view_;

  int32_t id_;

  std::map<int32_t, v8::Global<v8::Value>> browser_views_;
  std::map<int32_t, v8::Global<v8::Value>> container_views_;

  DISALLOW_COPY_AND_ASSIGN(ContainerView);
};

}  // namespace api

}  // namespace electron

#endif  // SHELL_BROWSER_API_ELECTRON_API_CONTAINER_VIEW_H_
