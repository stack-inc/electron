// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_UI_COCOA_ELECTRON_NATIVE_VIEW_H_
#define SHELL_BROWSER_UI_COCOA_ELECTRON_NATIVE_VIEW_H_

#import <Cocoa/Cocoa.h>

#include "third_party/skia/include/core/SkColor.h"

namespace electron {

class NativeView;

// A private class that holds views specific private data.
// Object-C does not support multi-inheiritance, so it is impossible to add
// common data members for UI elements. Our workaround is to manually add
// this class as member for each view.
struct NativeViewPrivate {
  NativeViewPrivate();
  ~NativeViewPrivate();

  NativeView* shell = nullptr;
  bool focusable = true;
  bool is_content_view = false;
  bool wants_layer = false;           // default value for wantsLayer
  bool wants_layer_infected = false;  // infects the wantsLayer property
};

}  // namespace electron

// The methods that every View should implemented.
@protocol ElectronNativeView
- (electron::NativeViewPrivate*)nativeViewPrivate;
- (void)setNativeBackgroundColor:(SkColor)color;
@end

// Extended methods of ElectronNativeView.
@interface NSView (ElectronNativeViewMethods) <ElectronNativeView>
- (electron::NativeView*)shell;
@end

namespace electron {

// Return whether a class is part of views system.
bool IsNativeView(id view);

// Return whether a class has been installed with custom methods.
bool NativeViewMethodsInstalled(Class cl);

// Add custom view methods to class.
void InstallNativeViewMethods(Class cl);

}  // namespace electron

#endif  // SHELL_BROWSER_UI_COCOA_ELECTRON_NATIVE_VIEW_H_
