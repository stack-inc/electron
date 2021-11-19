#include "shell/browser/ui/native_view.h"

#include <utility>

#include "base/strings/string_util.h"
#include "shell/browser/browser.h"
#include "shell/browser/native_window.h"
#include "shell/browser/ui/native_container.h"
#include "shell/browser/ui/yoga_util.h"
#include "shell/common/color_util.h"
#include "third_party/yoga/Yoga.h"

// This header required DEBUG to be defined.
#if defined(DEBUG)
#include "third_party/yoga/YGNodePrint.h"
#else
#define DEBUG
#include "third_party/yoga/YGNodePrint.h"
#undef DEBUG
#endif

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

// static
const char NativeView::kClassName[] = "View";

NativeView::NativeView() : view_(nullptr) {
  // Create node with the default yoga config.
  yoga_config_ = YGConfigNew();
  YGConfigCopy(yoga_config_, Browser::Get()->yoga_config());
  node_ = YGNodeNewWithConfig(yoga_config_);
  YGNodeSetContext(node_, this);
}

NativeView::~NativeView() {
  PlatformDestroy();

  // Free yoga config and node.
  YGNodeFree(node_);
  YGConfigFree(yoga_config_);
}

const char* NativeView::GetClassName() const {
  return kClassName;
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
    static_cast<NativeContainer*>(GetParent())->Layout();
}

void NativeView::UpdateDefaultStyle() {
  gfx::SizeF min_size = GetMinimumSize();
  YGNodeStyleSetMinWidth(node_, min_size.width());
  YGNodeStyleSetMinHeight(node_, min_size.height());
  Layout();
}

void NativeView::SetStyleProperty(const std::string& name, const std::string& value) {
  std::string key(ParseName(name));
  if (key == "backgroundcolor")
    SetBackgroundColor(ParseHexColor(value));
  else
    SetYogaProperty(node_, key, value);
}

void NativeView::SetStyleProperty(const std::string& name, float value) {
  SetYogaProperty(node_, ParseName(name), value);
}

std::string NativeView::GetComputedLayout() const {
  std::string result;
  auto options = static_cast<YGPrintOptions>(YGPrintOptionsLayout |
                                             YGPrintOptionsStyle |
                                             YGPrintOptionsChildren);
  facebook::yoga::YGNodeToString(result, node_, options, 0);
  return result;
}

gfx::SizeF NativeView::GetMinimumSize() const {
  return gfx::SizeF();
}

void NativeView::SetParent(NativeView* parent) {
  if (parent) {
    window_ = parent->window_;
    YGConfigCopy(yoga_config_, parent->yoga_config_);
  } else {
    window_ = nullptr;
  }
  parent_ = parent;
}

void NativeView::BecomeContentView(NativeWindow* window) {
  if (window) {
    window_ = window;
    YGConfigCopy(yoga_config_, window->GetYogaConfig());
  } else {
    window_ = nullptr;
  }
  parent_ = nullptr;
}

bool NativeView::IsContainer() const {
  return false;
}

void NativeView::OnSizeChanged() {}

}  // namespace electron
