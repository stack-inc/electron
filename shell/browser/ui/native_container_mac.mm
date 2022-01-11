// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_container.h"

#include "shell/browser/ui/cocoa/electron_native_container.h"
#include "shell/browser/ui/native_web_view_mac.h"

namespace electron {

void NativeContainer::PlatformInit() {
  TakeOverView([[ElectronNativeContainer alloc] init]);
}

void NativeContainer::PlatformDestroy() {}

void NativeContainer::PlatformAddChildView(NativeBrowserView* child) {
  NativeWebView* web_view = new NativeWebView(child);
  gfx::Rect bounds = child->GetLastBounds();
if (bounds.width() && bounds.height())
  web_view->SetBounds(bounds);
else
web_view->SetStyleProperty("flex", 1);
  AddChildView(web_view);
}

void NativeContainer::PlatformAddChildView(NativeView* child) {
  [GetNative() addSubview:child->GetNative()];
  // Handle the wants_layer_infected routine, which makes every view in
  // relationship with this view has wantsLayer set to true.
  NativeViewPrivate* priv = [GetNative() nativeViewPrivate];
  if (priv->wants_layer_infected) {
    [child->GetNative() setWantsLayer:YES];
  } else {
    if (IsNativeView(child->GetNative()) &&
        [child->GetNative() nativeViewPrivate]->wants_layer_infected) {
      // Just got infected, set wantsLayer on all children.
      priv->wants_layer_infected = true;
      SetWantsLayer(true);
      for (int i = 0; i < ChildCount(); ++i)
        [ChildAt(i)->GetNative() setWantsLayer:YES];
    }
  }
}

void NativeContainer::PlatformRemoveChildView(NativeBrowserView* child) {
}

void NativeContainer::PlatformRemoveChildView(NativeView* child) {
  [child->GetNative() removeFromSuperview];
  // Revert wantsLayer to default.
  NSView* nc = child->GetNative();
  if (IsNativeView(nc))
    [nc setWantsLayer:[nc nativeViewPrivate]->wants_layer];
  else
    [nc setWantsLayer:NO];
}

void NativeContainer::UpdateDraggableRegions() {
  for (NativeBrowserView* view : browser_children_)
    view->UpdateDraggableRegions(view->GetDraggableRegions());

  for (auto view : children_)
    view->UpdateDraggableRegions();
}

}  // namespace electron
