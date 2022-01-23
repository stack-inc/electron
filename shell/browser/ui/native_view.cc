// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_view.h"

#include <utility>

#include "base/strings/string_util.h"
#include "shell/browser/browser.h"
#include "shell/browser/native_window.h"
#include "shell/browser/ui/native_container_view.h"
#include "shell/browser/ui/yoga_util.h"
#include "shell/common/color_util.h"
#include "third_party/yoga/Yoga.h"
#include "ui/gfx/geometry/rect.h"

namespace electron {

namespace {

// Convert case to lower and remove non-ASCII characters.
std::string ParseName(const std::string& name) {
  std::string parsed;
  parsed.reserve(name.size());
  for (char c : name) {
    if (base::IsAsciiAlpha(c))
      parsed.push_back(base::ToLowerASCII(c));
  }
  return parsed;
}

}  // namespace

NativeView::NativeView() : view_(nullptr) {
  // Create node with the default yoga config.
  yoga_config_ = YGConfigNew();
  YGConfigCopy(yoga_config_, Browser::Get()->yoga_config());
  node_ = YGNodeNewWithConfig(yoga_config_);
  YGNodeSetContext(node_, this);
  PlatformInit();
}

NativeView::~NativeView() {
  PlatformDestroy();

  // Free yoga config and node.
  YGNodeFree(node_);
  YGConfigFree(yoga_config_);
}

void NativeView::SetVisible(bool visible) {
  if (visible == IsVisible())
    return;
  PlatformSetVisible(visible);
  YGNodeStyleSetDisplay(node_, visible ? YGDisplayFlex : YGDisplayNone);
  Layout();
}

void NativeView::Layout() {
  // By default just make parent do layout.
  if (GetParent() && GetParent()->IsContainer())
    static_cast<NativeContainerView*>(GetParent())->Layout();
}

void NativeView::SetStyleProperty(const std::string& name,
                                  const std::string& value) {
  std::string key(ParseName(name));
  if (key == "backgroundcolor")
    SetBackgroundColor(ParseHexColor(value));
  else
    SetYogaProperty(node_, key, value);
}

void NativeView::SetStyleProperty(const std::string& name, float value) {
  SetYogaProperty(node_, ParseName(name), value);
}

void NativeView::SetParent(NativeView* parent) {
  if (parent) {
    SetWindow(parent->window_);
    YGConfigCopy(yoga_config_, parent->yoga_config_);
  } else {
    SetWindow(nullptr);
  }
  parent_ = parent;
}

void NativeView::BecomeContentView(NativeWindow* window) {
  SetWindow(window);
  if (window)
    YGConfigCopy(yoga_config_, window->GetYogaConfig());
  parent_ = nullptr;
}

void NativeView::SetWindow(NativeWindow* window) {
  window_ = window;
  SetWindowForChildren(window);
}

void NativeView::SetWindowForChildren(NativeWindow* window) {}

bool NativeView::IsContainer() const {
  return false;
}

void NativeView::OnSizeChanged() {}

void NativeView::DetachChildView(NativeView* view) {}

void NativeView::TriggerBeforeunloadEvents() {}

}  // namespace electron
