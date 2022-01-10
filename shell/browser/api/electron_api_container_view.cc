// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/api/electron_api_container_view.h"

#include "gin/handle.h"
#include "shell/browser/api/electron_api_browser_view.h"
#include "shell/browser/api/electron_api_web_contents.h"
#include "shell/browser/browser.h"
#include "shell/browser/native_window.h"
#include "shell/common/color_util.h"
#include "shell/common/gin_converters/gfx_converter.h"
#include "shell/common/gin_helper/dictionary.h"
#include "shell/common/gin_helper/object_template_builder.h"
#include "shell/common/node_includes.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/views/view.h"

namespace {

int32_t GetNextId() {
  static int32_t next_id = 1;
  return next_id++;
}

}  // namespace

namespace electron {

namespace api {

ContainerView::ContainerView(gin::Arguments* args,
                             NativeContainer* view)
    : id_(GetNextId()),
      native_view_(view),
      view_(view) {
#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
  auto* view = view_->GetNative();
  if (view)
    view->AddObserver(this);
#endif
}

ContainerView::ContainerView(gin::Arguments* args,
                             NativeView* view)
    : id_(GetNextId()),
      native_view_(view) {
}

ContainerView::~ContainerView() = default;

#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
void ContainerView::OnViewIsDeleting(views::View* view) {
  view_.reset();
  Unpin();
}
#endif

void ContainerView::DetachFromParent(NativeWindow* window) {
  if (!view_)
    return;

  auto* parent = view_->GetParent();
  if (parent) {
    //@parent_view_api->RemoveContainerView(this);
  } else {
    auto* owner_window = view()->GetOwnerWindow();
    if (owner_window && owner_window != window) {
      owner_window->RemoveContainerView(view());
    }
  }
}

void ContainerView::SetBounds(const gfx::Rect& bounds) {
  if (native_view_)
    native_view_->SetBounds(bounds);
}

gfx::Rect ContainerView::GetBounds() {
  if (native_view_)
    return native_view_->GetBounds();
  return gfx::Rect();
}

void ContainerView::SetBackgroundColor(const std::string& color_name) {
  if (native_view_)
    native_view_->SetBackgroundColor(ParseHexColor(color_name));
}

void ContainerView::AddBrowserView(v8::Local<v8::Value> value) {
  if (!view_)
    return;

  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  gin::Handle<BrowserView> browser_view;
  if (value->IsObject() && gin::ConvertFromV8(isolate, value, &browser_view)) {
    auto get_that_view = browser_views_.find(browser_view->ID());
    if (get_that_view == browser_views_.end()) {
      if (browser_view->web_contents()) {
        view_->AddChildView(browser_view->view());
        browser_view->web_contents()->SetOwnerWindow(view_->GetOwnerWindow());
      }
      browser_views_[browser_view->ID()].Reset(isolate, value);
    }
  }
}

void ContainerView::RemoveBrowserView(v8::Local<v8::Value> value) {
  if (!view_)
    return;

  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  gin::Handle<BrowserView> browser_view;
  if (value->IsObject() && gin::ConvertFromV8(isolate, value, &browser_view)) {
    auto get_that_view = browser_views_.find(browser_view->ID());
    if (get_that_view != browser_views_.end()) {
      if (browser_view->web_contents()) {
        view_->RemoveChildView(browser_view->view());
        browser_view->web_contents()->SetOwnerWindow(nullptr);
      }
      (*get_that_view).second.Reset(isolate, value);
      browser_views_.erase(get_that_view);
    }
  }
}

void ContainerView::SetTopBrowserView(v8::Local<v8::Value> value,
                                      gin_helper::ErrorThrower thrower) {
  if (!view_)
    return;

  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  gin::Handle<BrowserView> browser_view;
  if (value->IsObject() && gin::ConvertFromV8(isolate, value, &browser_view)) {
    if (!browser_view->web_contents())
      return;
    auto* owner_window = browser_view->web_contents()->owner_window();
    auto get_that_view = browser_views_.find(browser_view->ID());
    if (get_that_view == browser_views_.end() ||
        (owner_window && owner_window != view_->GetOwnerWindow())) {
      thrower.ThrowError("Given BrowserView is not attached to the window");
      return;
    }

    view_->SetTopChildView(browser_view->view());
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

void ContainerView::AddContainerView(v8::Local<v8::Value> value) {
  if (!view_)
    return;

  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  gin::Handle<ContainerView> container_view;
  if (value->IsObject() &&
      gin::ConvertFromV8(isolate, value, &container_view)) {
    auto get_that_view = container_views_.find(container_view->ID());
    if (get_that_view == container_views_.end()) {
      // If we're reparenting a ContainerView, ensure that it's detached from
      // its previous owner window/container.
      container_view->DetachFromParent(nullptr);

      view_->AddChildView(container_view->view());
      container_view->view()->SetParent(view());
      container_views_[container_view->ID()].Reset(isolate, value);
    }
  }
}

void ContainerView::RemoveContainerView(v8::Local<v8::Value> value) {
  if (!view_)
    return;

  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  gin::Handle<ContainerView> container_view;
  if (value->IsObject() &&
      gin::ConvertFromV8(isolate, value, &container_view)) {
    auto get_that_view = container_views_.find(container_view->ID());
    if (get_that_view != container_views_.end()) {
      view_->RemoveChildView(container_view->view());
      container_view->view()->SetOwnerWindow(nullptr);
      (*get_that_view).second.Reset(isolate, value);
      container_views_.erase(get_that_view);
    }
  }
}

void ContainerView::SetTopContainerView(v8::Local<v8::Value> value,
                                        gin_helper::ErrorThrower thrower) {
  if (!view_)
    return;

  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  gin::Handle<ContainerView> container_view;
  if (value->IsObject() &&
      gin::ConvertFromV8(isolate, value, &container_view)) {
    auto* parent_view = container_view->view()->GetParent();
    auto get_that_view = container_views_.find(container_view->ID());
    if (get_that_view == container_views_.end() ||
        (parent_view && parent_view != view_->GetParent())) {
      thrower.ThrowError("Given ContainerView is not attached to the view");
      return;
    }

    view_->SetTopChildView(container_view->view());
  }
}

std::vector<v8::Local<v8::Value>> ContainerView::GetContainerViews() const {
  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  std::vector<v8::Local<v8::Value>> ret;

  for (auto const& views_iter : container_views_) {
    ret.push_back(v8::Local<v8::Value>::New(isolate, views_iter.second));
  }

  return ret;
}

void ContainerView::SetStringProperty(const std::string& name, const std::string& value) {
  if (!native_view_)
    return;
  native_view_->SetStyleProperty(name, value);
}

void ContainerView::SetNumericProperty(const std::string& name, float value) {
  if (!native_view_)
    return;
  native_view_->SetStyleProperty(name, value);
}

void ContainerView::Layout() {
  if (!native_view_)
    return;
  native_view_->Layout();
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
      .SetMethod("setBounds", &ContainerView::SetBounds)
      .SetMethod("getBounds", &ContainerView::GetBounds)
      .SetMethod("setBackgroundColor", &ContainerView::SetBackgroundColor)
      .SetMethod("addBrowserView", &ContainerView::AddBrowserView)
      .SetMethod("removeBrowserView", &ContainerView::RemoveBrowserView)
      .SetMethod("setTopBrowserView", &ContainerView::SetTopBrowserView)
      .SetMethod("getBrowserViews", &ContainerView::GetBrowserViews)
      .SetMethod("addContainerView", &ContainerView::AddContainerView)
      .SetMethod("removeContainerView", &ContainerView::RemoveContainerView)
      .SetMethod("setTopContainerView", &ContainerView::SetTopContainerView)
      .SetMethod("getContainerViews", &ContainerView::GetContainerViews)
      .SetMethod("setStringProperty", &ContainerView::SetStringProperty)
      .SetMethod("setNumericProperty", &ContainerView::SetNumericProperty)
      .SetMethod("layout", &ContainerView::Layout)
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
  ContainerView::SetConstructor(isolate, base::BindRepeating(&ContainerView::New));

  gin_helper::Dictionary constructor(
      isolate,
      ContainerView::GetConstructor(isolate)->GetFunction(context).ToLocalChecked());

  gin_helper::Dictionary dict(isolate, exports);
  dict.Set("ContainerView", constructor);
}

}  // namespace

NODE_LINKED_MODULE_CONTEXT_AWARE(electron_browser_container_view, Initialize)
