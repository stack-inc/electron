// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/api/electron_api_container_view.h"

#include "gin/handle.h"
#include "shell/browser/api/electron_api_browser_view.h"
#include "shell/browser/browser.h"
#include "shell/browser/native_window.h"
#include "shell/common/gin_helper/dictionary.h"
#include "shell/common/gin_helper/object_template_builder.h"
#include "shell/common/node_includes.h"
#include "ui/gfx/geometry/size.h"

namespace electron {

namespace api {

ContainerView::ContainerView(gin::Arguments* args,
                             NativeContainerView* container)
    : BaseView(args, container), container_(container) {}

ContainerView::~ContainerView() = default;

void ContainerView::AddChildView(v8::Local<v8::Value> value) {
  if (!container_.get())
    return;

  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  gin::Handle<BaseView> base_view;
  if (value->IsObject() && gin::ConvertFromV8(isolate, value, &base_view)) {
    auto get_that_view = base_views_.find(base_view->ID());
    if (get_that_view == base_views_.end()) {
      // If we're reparenting a BaseView, ensure that it's detached from
      // its previous owner window/view.
      auto* owner_window = base_view->view()->GetWindow();
      auto* owner_view = base_view->view()->GetParent();
      if (owner_view && owner_view != container_.get()) {
        owner_view->DetachChildView(base_view->view());
        base_view->view()->SetParent(nullptr);
      } else if (owner_window) {
        owner_window->RemoveChildView(base_view->view());
        base_view->view()->SetWindow(nullptr);
      }

      container_->AddChildView(base_view->view());
      base_views_[base_view->ID()].Reset(isolate, value);
    }
  }
}

void ContainerView::RemoveChildView(v8::Local<v8::Value> value) {
  if (!container_.get())
    return;

  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  gin::Handle<BaseView> base_view;
  if (value->IsObject() && gin::ConvertFromV8(isolate, value, &base_view)) {
    auto get_that_view = base_views_.find(base_view->ID());
    if (get_that_view != base_views_.end()) {
      container_->RemoveChildView(base_view->view());
      (*get_that_view).second.Reset(isolate, value);
      base_views_.erase(get_that_view);
    }
  }
}

void ContainerView::SetTopChildView(v8::Local<v8::Value> value,
                                    gin_helper::ErrorThrower thrower) {
  if (!container_.get())
    return;

  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  gin::Handle<BaseView> base_view;
  if (value->IsObject() && gin::ConvertFromV8(isolate, value, &base_view)) {
    auto* owner_view = base_view->view()->GetParent();
    auto get_that_view = base_views_.find(base_view->ID());
    if (get_that_view == base_views_.end() ||
        (owner_view && owner_view != container_.get())) {
      thrower.ThrowError("Given BaseView is not attached to the view");
      return;
    }

    container_->SetTopChildView(base_view->view());
  }
}

std::vector<v8::Local<v8::Value>> ContainerView::GetViews() const {
  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  std::vector<v8::Local<v8::Value>> ret;

  for (auto const& views_iter : base_views_) {
    ret.push_back(v8::Local<v8::Value>::New(isolate, views_iter.second));
  }

  return ret;
}

// static
gin_helper::WrappableBase* ContainerView::New(gin_helper::ErrorThrower thrower,
                                              gin::Arguments* args) {
  if (!Browser::Get()->is_ready()) {
    thrower.ThrowError("Cannot create ContainerView before app is ready");
    return nullptr;
  }

  auto* view = new ContainerView(args, new NativeContainerView());
#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
  view->Pin(args->isolate());
#endif
  view->InitWithArgs(args);
  return view;
}

// static
void ContainerView::BuildPrototype(v8::Isolate* isolate,
                                   v8::Local<v8::FunctionTemplate> prototype) {
  prototype->SetClassName(gin::StringToV8(isolate, "ContainerView"));
  gin_helper::ObjectTemplateBuilder(isolate, prototype->PrototypeTemplate())
      .SetMethod("addChildView", &ContainerView::AddChildView)
      .SetMethod("removeChildView", &ContainerView::RemoveChildView)
      .SetMethod("setTopChildView", &ContainerView::SetTopChildView)
      .SetMethod("getViews", &ContainerView::GetViews)
      .Build();
}

}  // namespace api

}  // namespace electron

namespace {

using electron::api::ContainerView;

void Initialize(v8::Local<v8::Object> exports,
                v8::Local<v8::Value> unused,
                v8::Local<v8::Context> context,
                void* priv) {
  v8::Isolate* isolate = context->GetIsolate();
  gin_helper::Dictionary dict(isolate, exports);
  dict.Set("ContainerView",
           gin_helper::CreateConstructor<ContainerView>(
               isolate, base::BindRepeating(&ContainerView::New)));
}

}  // namespace

NODE_LINKED_MODULE_CONTEXT_AWARE(electron_browser_container_view, Initialize)
