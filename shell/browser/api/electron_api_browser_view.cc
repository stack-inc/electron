// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/api/electron_api_browser_view.h"

#include <vector>

#include "shell/browser/api/electron_api_web_contents.h"
#include "shell/browser/browser.h"
#include "shell/browser/native_browser_view.h"
#include "shell/browser/ui/drag_util.h"
#include "shell/common/color_util.h"
#include "shell/common/gin_converters/gfx_converter.h"
#include "shell/common/gin_converters/value_converter.h"
#include "shell/common/gin_helper/dictionary.h"
#include "shell/common/gin_helper/object_template_builder.h"
#include "shell/common/node_includes.h"
#include "shell/common/options_switches.h"
#include "ui/gfx/geometry/rect.h"

namespace gin {

template <>
struct Converter<electron::AutoResizeFlags> {
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     electron::AutoResizeFlags* auto_resize_flags) {
    gin_helper::Dictionary params;
    if (!ConvertFromV8(isolate, val, &params)) {
      return false;
    }

    uint8_t flags = 0;
    bool width = false;
    if (params.Get("width", &width) && width) {
      flags |= electron::kAutoResizeWidth;
    }
    bool height = false;
    if (params.Get("height", &height) && height) {
      flags |= electron::kAutoResizeHeight;
    }
    bool horizontal = false;
    if (params.Get("horizontal", &horizontal) && horizontal) {
      flags |= electron::kAutoResizeHorizontal;
    }
    bool vertical = false;
    if (params.Get("vertical", &vertical) && vertical) {
      flags |= electron::kAutoResizeVertical;
    }

    *auto_resize_flags = static_cast<electron::AutoResizeFlags>(flags);
    return true;
  }
};

template <>
struct Converter<electron::NativeBrowserView::RoundedCornersOptions> {
  static bool FromV8(
      v8::Isolate* isolate,
      v8::Local<v8::Value> val,
      electron::NativeBrowserView::RoundedCornersOptions* options) {
    gin_helper::Dictionary params;
    if (!ConvertFromV8(isolate, val, &params)) {
      return false;
    }

    *options = electron::NativeBrowserView::RoundedCornersOptions();

    float radius;
    if (params.Get("radius", &radius)) {
      options->radius = radius;
    }

    bool top_left = false;
    if (params.Get("topLeft", &top_left) && top_left) {
      options->top_left = top_left;
    }
    bool top_right = false;
    if (params.Get("topRight", &top_right) && top_right) {
      options->top_right = top_right;
    }
    bool bottom_left = false;
    if (params.Get("bottomLeft", &bottom_left) && bottom_left) {
      options->bottom_left = bottom_left;
    }
    bool bottom_right = false;
    if (params.Get("bottomRight", &bottom_right) && bottom_right) {
      options->bottom_right = bottom_right;
    }

    return true;
  }
};

template <>
struct Converter<electron::NativeBrowserView::ClippingInsetOptions> {
  static bool FromV8(
      v8::Isolate* isolate,
      v8::Local<v8::Value> val,
      electron::NativeBrowserView::ClippingInsetOptions* options) {
    gin_helper::Dictionary params;
    if (!ConvertFromV8(isolate, val, &params)) {
      return false;
    }

    *options = electron::NativeBrowserView::ClippingInsetOptions();

    int top = 0;
    if (params.Get("top", &top) && top) {
      options->top = top;
    }
    int left = 0;
    if (params.Get("left", &left) && left) {
      options->left = left;
    }
    int bottom = 0;
    if (params.Get("bottom", &bottom) && bottom) {
      options->bottom = bottom;
    }
    int right = 0;
    if (params.Get("right", &right) && right) {
      options->right = right;
    }

    return true;
  }
};

}  // namespace gin

namespace {

int32_t GetNextId() {
  static int32_t next_id = 1;
  return next_id++;
}

}  // namespace

namespace electron {

namespace api {

gin::WrapperInfo BrowserView::kWrapperInfo = {gin::kEmbedderNativeGin};

BrowserView::BrowserView(gin::Arguments* args,
                         const gin_helper::Dictionary& options)
    : id_(GetNextId()) {
  v8::Isolate* isolate = args->isolate();
  gin_helper::Dictionary web_preferences =
      gin::Dictionary::CreateEmpty(isolate);
  options.Get(options::kWebPreferences, &web_preferences);
  web_preferences.Set("type", "browserView");

  v8::Local<v8::Value> value;

  // Copy the webContents option to webPreferences. This is only used internally
  // to implement nativeWindowOpen option.
  if (options.Get("webContents", &value)) {
    web_preferences.SetHidden("webContents", value);
  }

  auto web_contents =
      WebContents::CreateFromWebPreferences(args->isolate(), web_preferences);

  web_contents_.Reset(isolate, web_contents.ToV8());
  api_web_contents_ = web_contents.get();
  api_web_contents_->AddObserver(this);
  Observe(web_contents->web_contents());

  view_.reset(
      NativeBrowserView::Create(api_web_contents_->inspectable_web_contents()));
}

BrowserView::~BrowserView() {
  if (api_web_contents_) {  // destroy() called without closing WebContents
    api_web_contents_->RemoveObserver(this);
    api_web_contents_->Destroy();
  }
}

void BrowserView::WebContentsDestroyed() {
  api_web_contents_ = nullptr;
  web_contents_.Reset();
  Unpin();
}

void BrowserView::OnDraggableRegionsUpdated(
    const std::vector<mojom::DraggableRegionPtr>& regions) {
  view_->UpdateDraggableRegions(regions);
}

// static
gin::Handle<BrowserView> BrowserView::New(gin_helper::ErrorThrower thrower,
                                          gin::Arguments* args) {
  if (!Browser::Get()->is_ready()) {
    thrower.ThrowError("Cannot create BrowserView before app is ready");
    return gin::Handle<BrowserView>();
  }

  gin::Dictionary options = gin::Dictionary::CreateEmpty(args->isolate());
  args->GetNext(&options);

  auto handle =
      gin::CreateHandle(args->isolate(), new BrowserView(args, options));
  handle->Pin(args->isolate());
  return handle;
}

void BrowserView::SetAutoResize(AutoResizeFlags flags) {
  view_->SetAutoResizeFlags(flags);
}

void BrowserView::SetBounds(const gfx::Rect& bounds) {
  view_->SetBounds(bounds);
}

gfx::Rect BrowserView::GetBounds() {
  return view_->GetBounds();
}

void BrowserView::SetBackgroundColor(const std::string& color_name) {
  view_->SetBackgroundColor(ParseHexColor(color_name));
}

void BrowserView::SetZIndex(int z_index) {
  view_->SetZIndex(z_index);
}

int BrowserView::GetZIndex() {
  return view_->GetZIndex();
}

void BrowserView::SetRoundedCorners(
    const NativeBrowserView::RoundedCornersOptions& options) {
  return view_->SetRoundedCorners(options);
}

void BrowserView::SetClippingInsets(
    const NativeBrowserView::ClippingInsetOptions& options) {
  return view_->SetClippingInsets(options);
}

bool BrowserView::IsClickThrough() {
  return view_->GetInspectableWebContents()->IsClickThrough();
}

void BrowserView::SetClickThrough(bool clickThrough) {
  view_->GetInspectableWebContents()->SetClickThrough(clickThrough);
}

v8::Local<v8::Value> BrowserView::GetWebContents(v8::Isolate* isolate) {
  if (web_contents_.IsEmpty()) {
    return v8::Null(isolate);
  }

  return v8::Local<v8::Value>::New(isolate, web_contents_);
}

// static
v8::Local<v8::ObjectTemplate> BrowserView::FillObjectTemplate(
    v8::Isolate* isolate,
    v8::Local<v8::ObjectTemplate> templ) {
  return gin::ObjectTemplateBuilder(isolate, "BrowserView", templ)
      .SetMethod("setAutoResize", &BrowserView::SetAutoResize)
      .SetMethod("setBounds", &BrowserView::SetBounds)
      .SetMethod("getBounds", &BrowserView::GetBounds)
      .SetMethod("setBackgroundColor", &BrowserView::SetBackgroundColor)
      .SetMethod("setRoundedCorners", &BrowserView::SetRoundedCorners)
      .SetProperty("zIndex", &BrowserView::GetZIndex, &BrowserView::SetZIndex)
      .SetProperty("clickThrough", &BrowserView::IsClickThrough,
                   &BrowserView::SetClickThrough)
      .SetMethod("setClippingInsets", &BrowserView::SetClippingInsets)
      .SetProperty("webContents", &BrowserView::GetWebContents)
      .Build();
}

}  // namespace api

}  // namespace electron

namespace {

using electron::api::BrowserView;

void Initialize(v8::Local<v8::Object> exports,
                v8::Local<v8::Value> unused,
                v8::Local<v8::Context> context,
                void* priv) {
  v8::Isolate* isolate = context->GetIsolate();

  gin_helper::Dictionary dict(isolate, exports);
  dict.Set("BrowserView", BrowserView::GetConstructor(context));
}

}  // namespace

NODE_LINKED_MODULE_CONTEXT_AWARE(electron_browser_browser_view, Initialize)
