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
#include "ui/gfx/geometry/point.h"
#include "ui/gfx/geometry/rect.h"

#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
#include "ui/views/view.h"
#endif

namespace electron {

namespace api {

BaseView::BaseView(v8::Isolate* isolate, NativeView* native_view)
    : view_(native_view) {
#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
  auto* nview = view_->GetNative();
  if (nview)
    nview->AddObserver(this);
#endif
}

BaseView::BaseView(gin::Arguments* args, NativeView* native_view)
    : BaseView(args->isolate(), native_view) {
  InitWithArgs(args);
}

BaseView::~BaseView() {
  // Remove global reference so the JS object can be garbage collected.
  self_ref_.Reset();
}

void BaseView::InitWith(v8::Isolate* isolate, v8::Local<v8::Object> wrapper) {
  AttachAsUserData(view_.get());
  gin_helper::TrackableObject<BaseView>::InitWith(isolate, wrapper);

  // Reference this object in case it got garbage collected.
  self_ref_.Reset(isolate, wrapper);
}

#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
void BaseView::OnViewIsDeleting(views::View* observed_view) {
  RemoveFromWeakMap();

  // We can not call Destroy here because we need to call Emit first, but we
  // also do not want any method to be used, so just mark as destroyed here.
  MarkDestroyed();

  // Destroy the native class when window is closed.
  base::ThreadTaskRunnerHandle::Get()->PostTask(FROM_HERE, GetDestroyClosure());
}
#endif

void BaseView::SetBounds(const gfx::Rect& bounds) {
  view_->SetBounds(bounds);
}

gfx::Rect BaseView::GetBounds() const {
  return view_->GetBounds();
}

gfx::Point BaseView::OffsetFromView(gin::Handle<BaseView> from) const {
  return view_->OffsetFromView(from->view());
}

gfx::Point BaseView::OffsetFromWindow() const {
  return view_->OffsetFromWindow();
}

void BaseView::SetVisible(bool visible) {
  view_->SetVisible(visible);
}

bool BaseView::IsVisible() const {
  return view_->IsVisible();
}

bool BaseView::IsTreeVisible() const {
  return view_->IsTreeVisible();
}

void BaseView::Focus() {
  view_->Focus();
}

bool BaseView::HasFocus() const {
  return view_->HasFocus();
}

void BaseView::SetFocusable(bool focusable) {
  view_->SetFocusable(focusable);
}

bool BaseView::IsFocusable() const {
  return view_->IsFocusable();
}

void BaseView::SetBackgroundColor(const std::string& color_name) {
  view_->SetBackgroundColor(ParseHexColor(color_name));
}

int32_t BaseView::GetID() const {
  return weak_map_id();
}

bool BaseView::IsContainer() const {
  return view_->IsContainer();
}

// static
gin_helper::WrappableBase* BaseView::New(gin_helper::ErrorThrower thrower,
                                         gin::Arguments* args) {
  if (!Browser::Get()->is_ready()) {
    thrower.ThrowError("Cannot create View before app is ready");
    return nullptr;
  }

  return new BaseView(args, new NativeView());
}

// static
void BaseView::BuildPrototype(v8::Isolate* isolate,
                              v8::Local<v8::FunctionTemplate> prototype) {
  prototype->SetClassName(gin::StringToV8(isolate, "BaseView"));
  gin_helper::Destroyable::MakeDestroyable(isolate, prototype);
  gin_helper::ObjectTemplateBuilder(isolate, prototype->PrototypeTemplate())
      .SetMethod("setBounds", &BaseView::SetBounds)
      .SetMethod("getBounds", &BaseView::GetBounds)
      .SetMethod("offsetFromView", &BaseView::OffsetFromView)
      .SetMethod("offsetFromWindow", &BaseView::OffsetFromWindow)
      .SetMethod("setVisible", &BaseView::SetVisible)
      .SetMethod("isVisible", &BaseView::IsVisible)
      .SetMethod("isTreeVisible", &BaseView::IsTreeVisible)
      .SetMethod("focus", &BaseView::Focus)
      .SetMethod("hasFocus", &BaseView::HasFocus)
      .SetMethod("setFocusable", &BaseView::SetFocusable)
      .SetMethod("isFocusable", &BaseView::IsFocusable)
      .SetMethod("setBackgroundColor", &BaseView::SetBackgroundColor)
      .SetProperty("id", &BaseView::GetID)
      .SetProperty("isContainer", &BaseView::IsContainer)
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
  constructor.SetMethod("fromId", &BaseView::FromWeakMapID);
  constructor.SetMethod("getAllViews", &BaseView::GetAll);

  gin_helper::Dictionary dict(isolate, exports);
  dict.Set("BaseView", constructor);
}

}  // namespace

NODE_LINKED_MODULE_CONTEXT_AWARE(electron_browser_base_view, Initialize)
