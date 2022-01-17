// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_container_view.h"

#include "shell/browser/ui/cocoa/electron_native_view.h"

namespace electron {

void NativeContainerView::PlatformInit() {
  TakeOverView([[ElectronNativeView alloc] init]);
}

void NativeContainerView::PlatformDestroy() {}

void NativeContainerView::PlatformAddChildView(NativeView* view) {
  [GetNative() addSubview:view->GetNative()];
  // Handle the wants_layer_infected routine, which makes every view in
  // relationship with this view has wantsLayer set to true.
  NativeViewPrivate* priv = [GetNative() nativeViewPrivate];
  if (priv->wants_layer_infected) {
    [view->GetNative() setWantsLayer:YES];
  } else {
    if (IsNativeView(view->GetNative()) &&
        [view->GetNative() nativeViewPrivate]->wants_layer_infected) {
      // Just got infected, set wantsLayer on all children.
      priv->wants_layer_infected = true;
      SetWantsLayer(true);
      for (int i = 0; i < ChildCount(); ++i)
        [ChildAt(i)->GetNative() setWantsLayer:YES];
    }
  }
}

void NativeContainerView::PlatformRemoveChildView(NativeView* view) {
  [view->GetNative() removeFromSuperview];
  // Revert wantsLayer to default.
  NSView* native_view = view->GetNative();
  if (IsNativeView(native_view))
    [native_view setWantsLayer:[native_view nativeViewPrivate]->wants_layer];
  else
    [native_view setWantsLayer:NO];
}

void NativeContainerView::PlatformSetTopView(NativeView* view) {
  auto* native_view = view->GetNative();
  [GetNative() addSubview:native_view positioned:NSWindowAbove relativeTo:nil];
  native_view.hidden = NO;
}

void NativeContainerView::UpdateDraggableRegions() {
  for (auto view : children_)
    view->UpdateDraggableRegions();
}

}  // namespace electron
