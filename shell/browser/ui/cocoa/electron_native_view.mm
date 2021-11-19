#include "shell/browser/ui/cocoa/electron_native_view.h"

#include <objc/objc-runtime.h>

#include "base/mac/mac_util.h"
#include "shell/browser/native_window_mac.h"
#include "shell/browser/ui/cocoa/electron_ns_window.h"

namespace electron {

namespace {

// Returns whether the view belongs to a frameless window.
bool IsFramelessWindow(NSView* view) {
  if (![view window])
    return false;
  if (![[view window] respondsToSelector:@selector(shell)])
    return false;
  return ![static_cast<ElectronNSWindow*>([view window]) shell]->has_frame();
}

// Following methods are overrided in ElectronNativeView.

bool NativeViewInjected(NSView* self, SEL _cmd) {
  return true;
}

NativeView* GetShell(NSView* self, SEL _cmd) {
  return [self nativeViewPrivate]->shell;
}

BOOL AcceptsFirstResponder(NSView* self, SEL _cmd) {
  return [self nativeViewPrivate]->focusable;
}

// Following methods are overrided in ElectronNativeView to make sure that
// content view of frameless always takes the size of its parent view.

// This method is directly called by NSWindow during a window resize on OSX
// 10.10.0, beta 2. We must override it to prevent the content view from
// shrinking.
void SetFrameSize(NSView* self, SEL _cmd, NSSize size) {
  if (IsFramelessWindow(self) && [self nativeViewPrivate]->is_content_view &&
      [self superview])
    size = [[self superview] bounds].size;

  NSSize old_size = [self bounds].size;

  auto super_impl = reinterpret_cast<decltype(&SetFrameSize)>(
      [[self superclass] instanceMethodForSelector:_cmd]);
  super_impl(self, _cmd, size);

  if (size.width != old_size.width || size.height != old_size.height)
    [self shell]->OnSizeChanged();
}

// The contentView gets moved around during certain full-screen operations.
// This is less than ideal, and should eventually be removed.
void ViewDidMoveToSuperview(NSView* self, SEL _cmd) {
  if (!IsFramelessWindow(self) || ![self nativeViewPrivate]->is_content_view) {
    auto super_impl = reinterpret_cast<decltype(&ViewDidMoveToSuperview)>(
        [[self superclass] instanceMethodForSelector:_cmd]);
    super_impl(self, _cmd);
    return;
  }

  [self setFrame:[[self superview] bounds]];
}

}  // namespace

NativeViewPrivate::NativeViewPrivate() {}

NativeViewPrivate::~NativeViewPrivate() {}

bool IsNativeView(id view) {
  return [view respondsToSelector:@selector(nativeViewPrivate)];
}

bool NativeViewMethodsInstalled(Class cl) {
  return class_getClassMethod(cl, @selector(nativeViewInjected)) != nullptr;
}

void InstallNativeViewMethods(Class cl) {
  class_addMethod(cl, @selector(nativeViewInjected), (IMP)NativeViewInjected,
                  "B@:");
  class_addMethod(cl, @selector(shell), (IMP)GetShell, "^v@:");
  class_addMethod(cl, @selector(acceptsFirstResponder),
                  (IMP)AcceptsFirstResponder, "B@:");
  class_addMethod(cl, @selector(setFrameSize:), (IMP)SetFrameSize,
                  "v@:{_NSSize=ff}");
  class_addMethod(cl, @selector(viewDidMoveToSuperview),
                  (IMP)ViewDidMoveToSuperview, "v@:");
}

}  // namespace electron
