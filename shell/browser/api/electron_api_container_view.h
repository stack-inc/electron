// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_API_ELECTRON_API_CONTAINER_VIEW_H_
#define SHELL_BROWSER_API_ELECTRON_API_CONTAINER_VIEW_H_

#include <memory>
#include <vector>

#include "shell/browser/api/electron_api_base_view.h"
#include "shell/browser/ui/native_container_view.h"

namespace electron {

namespace api {

class ContainerView : public BaseView {
 public:
  static gin_helper::WrappableBase* New(gin_helper::ErrorThrower thrower,
                                        gin::Arguments* args);

  static void BuildPrototype(v8::Isolate* isolate,
                             v8::Local<v8::FunctionTemplate> prototype);

 protected:
  ContainerView(gin::Arguments* args, NativeContainerView* container);
  ~ContainerView() override;

  void AddChildView(v8::Local<v8::Value> value);
  void RemoveChildView(v8::Local<v8::Value> value);
  void SetTopChildView(v8::Local<v8::Value> value,
                       gin_helper::ErrorThrower thrower);
  std::vector<v8::Local<v8::Value>> GetViews() const;

 private:
  NativeContainerView* container_;

  std::map<int32_t, v8::Global<v8::Value>> base_views_;

  DISALLOW_COPY_AND_ASSIGN(ContainerView);
};

}  // namespace api

}  // namespace electron

#endif  // SHELL_BROWSER_API_ELECTRON_API_CONTAINER_VIEW_H_
