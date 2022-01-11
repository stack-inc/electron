// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/api/electron_api_base_view.h"

#include "gin/handle.h"
#include "shell/browser/browser.h"
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

BaseView::BaseView(gin::Arguments* args,
                   NativeView* native_view)
    : id_(GetNextId()),
      view_(native_view) {
#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
  auto* nview = view_->GetNative();
  if (nview)
    nview->AddObserver(this);
#endif
}

BaseView::~BaseView() = default;

#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
void BaseView::OnViewIsDeleting(views::View* observed_view) {
  view_.reset();
  Unpin();
}
#endif

void BaseView::SetBounds(const gfx::Rect& bounds) {
  if (view_.get())
    view_->SetBounds(bounds);
}

gfx::Rect BaseView::GetBounds() {
  if (view_.get())
    return view_->GetBounds();
  return gfx::Rect();
}

void BaseView::SetBackgroundColor(const std::string& color_name) {
  if (view_.get())
    view_->SetBackgroundColor(ParseHexColor(color_name));
}

void BaseView::SetStringProperty(const std::string& name, const std::string& value) {
  if (view_.get())
    view_->SetStyleProperty(name, value);
}

void BaseView::SetNumericProperty(const std::string& name, float value) {
  if (view_.get())
    view_->SetStyleProperty(name, value);
}

void BaseView::Layout() {
  if (view_.get())
    view_->Layout();
}

// static
gin_helper::WrappableBase* BaseView::New(gin_helper::ErrorThrower thrower,
                                         gin::Arguments* args) {
  if (!Browser::Get()->is_ready()) {
    thrower.ThrowError("Cannot create View before app is ready");
    return nullptr;
  }

  auto* base_view = new BaseView(args, new NativeView());
#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
  base_view->Pin(args->isolate());
#endif
  base_view->InitWithArgs(args);
  return base_view;
}

// static
void BaseView::BuildPrototype(v8::Isolate* isolate,
                              v8::Local<v8::FunctionTemplate> prototype) {
  prototype->SetClassName(gin::StringToV8(isolate, "BaseView"));
  gin_helper::ObjectTemplateBuilder(isolate, prototype->PrototypeTemplate())
      .SetMethod("setBounds", &BaseView::SetBounds)
      .SetMethod("getBounds", &BaseView::GetBounds)
      .SetMethod("setBackgroundColor", &BaseView::SetBackgroundColor)
      .SetMethod("setStringProperty", &BaseView::SetStringProperty)
      .SetMethod("setNumericProperty", &BaseView::SetNumericProperty)
      .SetMethod("layout", &BaseView::Layout)
      .Build();
}

}  // namespace api

}  // namespace electron

namespace {

using electron::api::BaseView;

void Initialize(v8::Local<v8::Object> exports,
                v8::Local<v8::Value> unused,
                v8::Local<v8::Context> context,
                void* priv) {
  v8::Isolate* isolate = context->GetIsolate();
  BaseView::SetConstructor(isolate, base::BindRepeating(&BaseView::New));

  gin_helper::Dictionary constructor(
      isolate,
      BaseView::GetConstructor(isolate)->GetFunction(context).ToLocalChecked());

  gin_helper::Dictionary dict(isolate, exports);
  dict.Set("BaseView", constructor);
}

}  // namespace

NODE_LINKED_MODULE_CONTEXT_AWARE(electron_browser_base_view, Initialize)
