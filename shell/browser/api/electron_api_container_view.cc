// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/api/electron_api_container_view.h"

#include "gin/handle.h"
#include "shell/browser/api/electron_api_browser_view.h"
#include "shell/browser/api/electron_api_web_contents.h"
#include "shell/browser/browser.h"
#include "shell/browser/native_window.h"
#include "shell/common/gin_converters/gfx_converter.h"
#include "shell/common/gin_helper/dictionary.h"
#include "shell/common/gin_helper/object_template_builder.h"
#include "shell/common/node_includes.h"

namespace electron {

namespace api {

ContainerView::ContainerView(gin::Arguments* args,
                             NativeContainer* container)
    : BaseView(args, container), container_(container) {}

ContainerView::~ContainerView() = default;

void ContainerView::AddBrowserView(v8::Local<v8::Value> value) {
  if (!container_.get())
    return;

  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  gin::Handle<BrowserView> browser_view;
  if (value->IsObject() && gin::ConvertFromV8(isolate, value, &browser_view)) {
    auto get_that_view = browser_views_.find(browser_view->ID());
    if (get_that_view == browser_views_.end()) {
      if (browser_view->web_contents()) {
        // If we're reparenting a BrowserView, ensure that it's detached from
        // its previous owner window/view.
        auto* owner_window = browser_view->owner_window();
        auto* owner_view = browser_view->owner_view();
        if (owner_window) {
          owner_window->RemoveBrowserView(browser_view->view());
          browser_view->SetOwnerWindow(nullptr);
        } else if (owner_view && owner_view != container_.get()) {
          owner_view->DetachChildView(browser_view->view());
          browser_view->SetOwnerView(nullptr);
        }

        container_->AddChildView(browser_view->view());
        browser_view->SetOwnerView(container_.get());
      }
      browser_views_[browser_view->ID()].Reset(isolate, value);
    }
  }
}

void ContainerView::RemoveBrowserView(v8::Local<v8::Value> value) {
  if (!container_.get())
    return;

  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  gin::Handle<BrowserView> browser_view;
  if (value->IsObject() && gin::ConvertFromV8(isolate, value, &browser_view)) {
    auto get_that_view = browser_views_.find(browser_view->ID());
    if (get_that_view != browser_views_.end()) {
      if (browser_view->web_contents()) {
        container_->RemoveChildView(browser_view->view());
        browser_view->SetOwnerView(nullptr);
      }
      (*get_that_view).second.Reset(isolate, value);
      browser_views_.erase(get_that_view);
    }
  }
}

void ContainerView::SetTopBrowserView(v8::Local<v8::Value> value,
                                      gin_helper::ErrorThrower thrower) {
  if (!container_.get())
    return;

  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  gin::Handle<BrowserView> browser_view;
  if (value->IsObject() && gin::ConvertFromV8(isolate, value, &browser_view)) {
    if (!browser_view->web_contents())
      return;
    auto* owner_view = browser_view->owner_view();
    auto get_that_view = browser_views_.find(browser_view->ID());
    if (get_that_view == browser_views_.end() ||
        (owner_view && owner_view != container_.get())) {
      thrower.ThrowError("Given BrowserView is not attached to the container view");
      return;
    }

    container_->SetTopChildView(browser_view->view());
  }
}

std::vector<v8::Local<v8::Value>> ContainerView::GetBrowserViews() const {
  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  std::vector<v8::Local<v8::Value>> ret;

  for (auto const& views_iter : browser_views_) {
    ret.push_back(v8::Local<v8::Value>::New(isolate, views_iter.second));
  }

  return ret;
}

void ContainerView::AddChildView(v8::Local<v8::Value> value) {
  if (!container_.get())
    return;

  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  gin::Handle<BaseView> base_view;
  if (value->IsObject() &&
      gin::ConvertFromV8(isolate, value, &base_view)) {
    auto get_that_view = base_views_.find(base_view->ID());
    if (get_that_view == base_views_.end()) {
      // If we're reparenting a BaseView, ensure that it's detached from
      // its previous owner window/view.
      auto* owner_window = base_view->view()->GetWindow();
      auto* owner_view = base_view->view()->GetParent();
      if (owner_window) {
        owner_window->RemoveChildView(base_view->view());
        base_view->view()->SetWindow(nullptr);
      } else if (owner_view && owner_view != container_.get()) {
        owner_view->DetachChildView(base_view->view());
        base_view->view()->SetParent(nullptr);
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
  if (value->IsObject() &&
      gin::ConvertFromV8(isolate, value, &base_view)) {
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
  if (value->IsObject() &&
      gin::ConvertFromV8(isolate, value, &base_view)) {
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

  auto* view = new ContainerView(args, new NativeContainer());
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
      .SetMethod("addBrowserView", &ContainerView::AddBrowserView)
      .SetMethod("removeBrowserView", &ContainerView::RemoveBrowserView)
      .SetMethod("setTopBrowserView", &ContainerView::SetTopBrowserView)
      .SetMethod("getBrowserViews", &ContainerView::GetBrowserViews)
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
  dict.Set("ContainerView", gin_helper::CreateConstructor<ContainerView>(
                            isolate, base::BindRepeating(&ContainerView::New)));
}

}  // namespace

NODE_LINKED_MODULE_CONTEXT_AWARE(electron_browser_container_view, Initialize)
