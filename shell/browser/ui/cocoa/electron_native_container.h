#ifndef SHELL_BROWSER_UI_COCOA_ELECTRON_NATIVE_CONTAINER_H_
#define SHELL_BROWSER_UI_COCOA_ELECTRON_NATIVE_CONTAINER_H_

#include "shell/browser/ui//native_container.h"
#include "shell/browser/ui/cocoa/electron_native_view.h"
#include "third_party/skia/include/core/SkColor.h"

@interface ElectronNativeContainer : NSView <ElectronNativeView> {
 @private
  electron::NativeViewPrivate private_;
  SkColor background_color_;
}
@end

#endif  // SHELL_BROWSER_UI_COCOA_ELECTRON_NATIVE_CONTAINER_H_
