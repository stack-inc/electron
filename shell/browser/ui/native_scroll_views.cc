// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_scroll.h"

#include "ui/gfx/geometry/size.h"

namespace electron {

void NativeScroll::PlatformInit() {
  auto* scroll = [[ElectronNativeScroll alloc] initWithShell:this];
  scroll.drawsBackground = NO;
  if (scroll.scrollerStyle == NSScrollerStyleOverlay) {
    scroll.hasHorizontalScroller = YES;
    scroll.hasVerticalScroller = YES;
  }
  [scroll.contentView setCopiesOnScroll:NO];
  TakeOverView(scroll);
}

void NativeScroll::PlatformSetContentView(NativeView* view) {
  auto* scroll = static_cast<NSScrollView*>(GetNative());
  scroll.documentView = view->GetNative();
}

void NativeScroll::SetContentSize(const gfx::Size& size) {
  auto* scroll = static_cast<ElectronNativeScroll*>(GetNative());
  NSSize content_size = size.ToCGSize();
  [scroll setContentSize:content_size];
  [scroll.documentView setFrameSize:content_size];
}

void NativeScroll::SetScrollPosition(float horizon, float vertical) {
  auto* scroll = static_cast<ElectronNativeScroll*>(GetNative());
  if (![scroll.documentView isFlipped])
    vertical = NSHeight(scroll.documentView.bounds) - vertical;
  [scroll.documentView scrollPoint:NSMakePoint(horizon, vertical)];
}

std::tuple<float, float> NativeScroll::GetScrollPosition() const {
  auto* scroll = static_cast<ElectronNativeScroll*>(GetNative());
  NSPoint point = scroll.contentView.bounds.origin;
  if (![scroll.documentView isFlipped]) {
    point.y = NSHeight(scroll.documentView.bounds) -
              NSHeight(scroll.contentView.bounds) - point.y;
  }
  return std::make_tuple(point.x, point.y);
}

std::tuple<float, float> NativeScroll::GetMaximumScrollPosition() const {
  auto* scroll = static_cast<ElectronNativeScroll*>(GetNative());
  NSRect docBounds = scroll.documentView.bounds;
  NSRect clipBounds = scroll.contentView.bounds;
  return std::make_tuple(NSMaxX(docBounds) - NSWidth(clipBounds),
                         NSMaxY(docBounds) - NSHeight(clipBounds));
}

void NativeScroll::SetOverlayScrollbar(bool overlay) {
  auto* scroll = static_cast<ElectronNativeScroll*>(GetNative());
  scroll.scrollerStyle =
      overlay ? NSScrollerStyleOverlay : NSScrollerStyleLegacy;
}

bool NativeScroll::IsOverlayScrollbar() const {
  auto* scroll = static_cast<ElectronNativeScroll*>(GetNative());
  return scroll.scrollerStyle == NSScrollerStyleOverlay;
}

void NativeScroll::SetScrollbarPolicy(Policy h_policy, Policy v_policy) {
  auto* scroll = static_cast<ElectronNativeScroll*>(GetNative());
  scroll.hasHorizontalScroller = h_policy != Policy::Never;
  scroll.hasVerticalScroller = v_policy != Policy::Never;
}

std::tuple<NativeScroll::Policy, NativeScroll::Policy>
NativeScroll::GetScrollbarPolicy() const {
  auto* scroll = static_cast<ElectronNativeScroll*>(GetNative());
  Policy h_policy =
      scroll.hasHorizontalScroller ? Policy::Automatic : Policy::Never;
  Policy v_policy =
      scroll.hasVerticalScroller ? Policy::Automatic : Policy::Never;
  return std::make_tuple(h_policy, v_policy);
}

void NativeScroll::SetScrollElasticity(Elasticity h, Elasticity v) {
  auto* scroll = static_cast<ElectronNativeScroll*>(GetNative());
  scroll.horizontalScrollElasticity = static_cast<NSScrollElasticity>(h);
  scroll.verticalScrollElasticity = static_cast<NSScrollElasticity>(v);
}

std::tuple<NativeScroll::Elasticity, NativeScroll::Elasticity>
NativeScroll::GetScrollElasticity() const {
  auto* scroll = static_cast<ElectronNativeScroll*>(GetNative());
  Elasticity h = static_cast<Elasticity>(scroll.horizontalScrollElasticity);
  Elasticity v = static_cast<Elasticity>(scroll.verticalScrollElasticity);
  return std::make_tuple(h, v);
}

#if 0
void NativeScroll::OnConnect(int identifier) {
  if (identifier == kOnScroll) {
    auto* scroll = static_cast<ElectronNativeScroll*>(GetNative());
    [scroll setEventEnabled:YES];
  }
}
#endif

}  // namespace electron
