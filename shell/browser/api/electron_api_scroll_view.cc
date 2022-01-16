// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/api/electron_api_scroll_view.h"

#include "gin/handle.h"
#include "shell/browser/api/electron_api_base_view.h"
#include "shell/browser/browser.h"
#include "shell/browser/javascript_environment.h"
#include "shell/browser/native_window.h"
#include "shell/common/color_util.h"
#include "shell/common/gin_converters/gfx_converter.h"
#include "shell/common/gin_helper/constructor.h"
#include "shell/common/gin_helper/dictionary.h"
#include "shell/common/gin_helper/object_template_builder.h"
#include "shell/common/node_includes.h"
#include "ui/gfx/geometry/size.h"

namespace electron {

namespace api {

namespace {

ScrollBarMode ConvertToScrollBarMode(std::string mode) {
  if (mode == "disabled")
    return ScrollBarMode::kDisabled;
  else if (mode == "hidden-but-enabled")
    return ScrollBarMode::kHiddenButEnabled;
  return ScrollBarMode::kEnabled;
}

std::string ConvertFromScrollBarMode(ScrollBarMode mode) {
  if (mode == ScrollBarMode::kDisabled)
    return "disabled";
  else if (mode == ScrollBarMode::kHiddenButEnabled)
    return "hidden-but-enabled";
  return "enabled";
}

}  // namespace

ScrollView::ScrollView(gin::Arguments* args, NativeScrollView* scroll)
    : BaseView(args, scroll), scroll_(scroll) {}

ScrollView::~ScrollView() = default;

void ScrollView::SetContentView(v8::Local<v8::Value> value) {
  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  gin::Handle<BaseView> content_view;
  if (value->IsObject() && gin::ConvertFromV8(isolate, value, &content_view)) {
    if (content_view->ID() != content_view_id_) {
      // If we're reparenting a BaseView, ensure that it's detached from
      // its previous owner window/view.
      auto* owner_window = content_view->view()->GetWindow();
      auto* owner_view = content_view->view()->GetParent();
      if (owner_window) {
        owner_window->RemoveChildView(content_view->view());
        content_view->view()->SetWindow(nullptr);
      } else if (owner_view && owner_view != scroll_.get()) {
        owner_view->DetachChildView(content_view->view());
        content_view->view()->SetParent(nullptr);
      }

      scroll_->SetContentView(content_view->view());
      content_view_id_ = content_view->ID();
      content_view_.Reset(isolate, value);
    }
  }
}

v8::Local<v8::Value> ScrollView::GetContentView() const {
  v8::Isolate* isolate = JavascriptEnvironment::GetIsolate();
  v8::Locker locker(isolate);
  v8::HandleScope handle_scope(isolate);

  if (content_view_.IsEmpty())
    return v8::Null(isolate);

  return v8::Local<v8::Value>::New(isolate, content_view_);
}

void ScrollView::SetContentSize(gfx::Size size) {
  if (scroll_.get())
    scroll_->SetContentSize(size);
}

gfx::Size ScrollView::GetContentSize() {
  if (scroll_.get())
    return scroll_->GetContentSize();
  return gfx::Size();
}

int ScrollView::GetMinHeight() {
#if !defined(OS_MAC)
  if (scroll_.get())
    return scroll_->GetMinHeight();
#endif
  return -1;
}

int ScrollView::GetMaxHeight() {
#if !defined(OS_MAC)
  if (scroll_.get())
    return scroll_->GetMaxHeight();
#endif
  return -1;
}

void ScrollView::ClipHeightTo(int min_height, int max_height) {
#if !defined(OS_MAC)
  if (scroll_.get())
    scroll_->ClipHeightTo(min_height, max_height);
#endif
}

gfx::Rect ScrollView::GetVisibleRect() {
#if !defined(OS_MAC)
  if (scroll_.get())
    return scroll_->GetVisibleRect();
#endif
  return gfx::Rect();
}

void ScrollView::SetHorizontalScrollBarMode(std::string mode) {
  if (scroll_.get())
    scroll_->SetHorizontalScrollBarMode(ConvertToScrollBarMode(mode));
}

std::string ScrollView::GetHorizontalScrollBarMode() {
  if (scroll_.get())
    return ConvertFromScrollBarMode(scroll_->GetHorizontalScrollBarMode());
  return "enabled";
}

void ScrollView::SetVerticalScrollBarMode(std::string mode) {
  if (scroll_.get())
    scroll_->SetVerticalScrollBarMode(ConvertToScrollBarMode(mode));
}

std::string ScrollView::GetVerticalScrollBarMode() {
  if (scroll_.get())
    return ConvertFromScrollBarMode(scroll_->GetVerticalScrollBarMode());
  return "enabled";
}

void ScrollView::SetAllowKeyboardScrolling(bool allow) {
#if !defined(OS_MAC)
  if (scroll_.get())
    scroll_->SetAllowKeyboardScrolling(allow);
#endif
}

bool ScrollView::GetAllowKeyboardScrolling() {
#if !defined(OS_MAC)
  if (scroll_.get())
    return scroll_->GetAllowKeyboardScrolling();
#endif
  return false;
}

void ScrollView::SetDrawOverflowIndicator(bool indicator) {
#if !defined(OS_MAC)
  if (scroll_.get())
    scroll_->SetDrawOverflowIndicator(indicator);
#endif
}

bool ScrollView::GetDrawOverflowIndicator() {
#if !defined(OS_MAC)
  if (scroll_.get())
    return scroll_->GetDrawOverflowIndicator();
#endif
  return false;
}

// static
gin_helper::WrappableBase* ScrollView::New(gin_helper::ErrorThrower thrower,
                                           gin::Arguments* args) {
  if (!Browser::Get()->is_ready()) {
    thrower.ThrowError("Cannot create ScrollView before app is ready");
    return nullptr;
  }

  auto* view = new ScrollView(args, new NativeScrollView());
#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
  view->Pin(args->isolate());
#endif
  view->InitWithArgs(args);
  return view;
}

// static
void ScrollView::BuildPrototype(v8::Isolate* isolate,
                                v8::Local<v8::FunctionTemplate> prototype) {
  prototype->SetClassName(gin::StringToV8(isolate, "ScrollView"));
  gin_helper::ObjectTemplateBuilder(isolate, prototype->PrototypeTemplate())
      .SetMethod("setContentView", &ScrollView::SetContentView)
      .SetMethod("getContentView", &ScrollView::GetContentView)
      .SetMethod("setContentSize", &ScrollView::SetContentSize)
      .SetMethod("getContentSize", &ScrollView::GetContentSize)
      .SetMethod("getMinHeight", &ScrollView::GetMinHeight)
      .SetMethod("getMaxHeight", &ScrollView::GetMaxHeight)
      .SetMethod("clipHeightTo", &ScrollView::ClipHeightTo)
      .SetMethod("getVisibleRect", &ScrollView::GetVisibleRect)
      .SetMethod("setHorizontalScrollBarMode",
                 &ScrollView::SetHorizontalScrollBarMode)
      .SetMethod("getHorizontalScrollBarMode",
                 &ScrollView::GetHorizontalScrollBarMode)
      .SetMethod("setVerticalScrollBarMode",
                 &ScrollView::SetVerticalScrollBarMode)
      .SetMethod("getVerticalScrollBarMode",
                 &ScrollView::GetVerticalScrollBarMode)
      .SetMethod("setAllowKeyboardScrolling", &ScrollView::SetAllowKeyboardScrolling)
      .SetMethod("getAllowKeyboardScrolling", &ScrollView::GetAllowKeyboardScrolling)
      .SetMethod("setDrawOverflowIndicator", &ScrollView::SetDrawOverflowIndicator)
      .SetMethod("getDrawOverflowIndicator", &ScrollView::GetDrawOverflowIndicator)
      .Build();
}

}  // namespace api

}  // namespace electron

namespace {

using electron::api::ScrollView;

void Initialize(v8::Local<v8::Object> exports,
                v8::Local<v8::Value> unused,
                v8::Local<v8::Context> context,
                void* priv) {
  v8::Isolate* isolate = context->GetIsolate();
  gin_helper::Dictionary dict(isolate, exports);
  dict.Set("ScrollView", gin_helper::CreateConstructor<ScrollView>(
                            isolate, base::BindRepeating(&ScrollView::New)));
}

}  // namespace

NODE_LINKED_MODULE_CONTEXT_AWARE(electron_browser_scroll_view, Initialize)
