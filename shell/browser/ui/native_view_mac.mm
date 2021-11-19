#include "shell/browser/ui/cocoa/electron_native_view.h"

#include "base/mac/foundation_util.h"
#include "base/mac/scoped_cftyperef.h"
#include "shell/browser/ui/native_container.h"
#include "skia/ext/skia_utils_mac.h"
#include "ui/gfx/geometry/point_conversions.h"
#include "ui/gfx/geometry/rect_conversions.h"

namespace electron {

void NativeView::PlatformDestroy() {
  if (IsNativeView(view_)) {
    // The view may be referenced after this class gets destroyed.
    NativeViewPrivate* priv = [view_ nativeViewPrivate];
    priv->shell = nullptr;
  }
  [view_ release];
}

void NativeView::TakeOverView(NATIVEVIEW view) {
  view_ = view;

  if (!IsNativeView(view))
    return;

  // Install events handle for the view's class.
  Class cl = [view class];
  if (!NativeViewMethodsInstalled(cl)) {
    InstallNativeViewMethods(cl);
  }

  // Initialize private bits of the view.
  NativeViewPrivate* priv = [view nativeViewPrivate];
  priv->shell = this;

  // Set the |focusable| property to the parent class's default one.
  SEL cmd = @selector(acceptsFirstResponder);
  auto super_impl = reinterpret_cast<BOOL (*)(NSView*, SEL)>(
      [[view superclass] instanceMethodForSelector:cmd]);
  priv->focusable = super_impl(view, cmd);
}

void NativeView::SetBounds(const gfx::RectF& bounds) {
  NSRect frame = bounds.ToCGRect();
  [view_ setFrame:frame];
  // Calling setFrame manually does not trigger resizeSubviewsWithOldSize.
  [view_ resizeSubviewsWithOldSize:frame.size];
}

gfx::Vector2dF NativeView::OffsetFromView(const NativeView* from) const {
  NSPoint point = [view_ convertPoint:NSZeroPoint toView:from->view_];
  return gfx::Vector2dF(point.x, point.y);
}

gfx::Vector2dF NativeView::OffsetFromWindow() const {
  NSPoint point = [view_ convertPoint:NSZeroPoint toView:nil];
  return gfx::Vector2dF(point.x, point.y);
}

gfx::RectF NativeView::GetBounds() const {
  return gfx::RectF([view_ frame]);
}

void NativeView::SetPixelBounds(const gfx::Rect& bounds) {
  SetBounds(gfx::RectF(bounds));
}

gfx::Rect NativeView::GetPixelBounds() const {
  return ToNearestRect(GetBounds());
}

void NativeView::SchedulePaint() {
  [view_ setNeedsDisplay:YES];
}

void NativeView::SchedulePaintRect(const gfx::RectF& rect) {
  [view_ setNeedsDisplayInRect:rect.ToCGRect()];
}

void NativeView::PlatformSetVisible(bool visible) {
  [view_ setHidden:!visible];
}

bool NativeView::IsVisible() const {
  return ![view_ isHidden];
}

bool NativeView::IsTreeVisible() const {
  return ![view_ isHiddenOrHasHiddenAncestor];
}

void NativeView::Focus() {
  if (view_.window && IsFocusable())
    [view_.window makeFirstResponder:view_];
}

bool NativeView::HasFocus() const {
  if (view_.window)
    return view_.window.firstResponder == view_;
  else
    return false;
}

void NativeView::SetFocusable(bool focusable) {
  NativeViewPrivate* priv = [view_ nativeViewPrivate];
  priv->focusable = focusable;
}

bool NativeView::IsFocusable() const {
  return [view_ acceptsFirstResponder];
}

void NativeView::SetBackgroundColor(SkColor color) {
  if (IsNativeView(view_))
    [view_ setNativeBackgroundColor:color];
#if 0
  view_.wantsLayer = YES;
  view_.layer.backgroundColor = skia::CGColorCreateFromSkColor(color);
#endif
}

void NativeView::SetWantsLayer(bool wants) {
  [view_ nativeViewPrivate]->wants_layer = wants;
  [view_ setWantsLayer:wants];
}

bool NativeView::WantsLayer() const {
  return [view_ wantsLayer];
}

}  // namespace electron
