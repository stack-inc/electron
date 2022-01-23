// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_UI_NATIVE_VIEW_H_
#define SHELL_BROWSER_UI_NATIVE_VIEW_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "third_party/skia/include/core/SkColor.h"

#if defined(OS_MAC)
#ifdef __OBJC__
@class NSView;
#else
struct NSView;
#endif
#else
namespace views {
class View;
}
#endif

namespace gfx {
class Point;
class Rect;
}  // namespace gfx

namespace electron {

#if defined(OS_MAC)
using NATIVEVIEW = NSView*;
#else
using NATIVEVIEW = views::View*;
#endif

class NativeWindow;

// The base class for all kinds of views.
class NativeView : public base::RefCounted<NativeView> {
 public:
  NativeView();

  // Change position and size.
  virtual void SetBounds(const gfx::Rect& bounds);

  // Get position and size.
  gfx::Rect GetBounds() const;

#if defined(OS_MAC)
  // Coordinate convertions.
  gfx::Point OffsetFromView(const NativeView* from) const;
  gfx::Point OffsetFromWindow() const;
#endif

  // Show/Hide the view.
  void SetVisible(bool visible);
  bool IsVisible() const;

  // Whether the view and its parent are visible.
  bool IsTreeVisible() const;

  // Mark the whole view as dirty.
  void SchedulePaint();

  // Repaint the rect
  void SchedulePaintRect(const gfx::Rect& rect);

  // Move the keyboard focus to the view.
  void Focus();
  bool HasFocus() const;

  // Whether the view can be focused.
  void SetFocusable(bool focusable);
  bool IsFocusable() const;

  // Display related styles.
  void SetBackgroundColor(SkColor color);

#if defined(OS_MAC)
  void SetWantsLayer(bool wants);
  bool WantsLayer() const;
#endif

  // Get parent.
  NativeView* GetParent() const { return parent_; }

  // Get window.
  NativeWindow* GetWindow() const { return window_; }

  // Get the native View object.
  NATIVEVIEW GetNative() const { return view_; }

  // Internal: Set parent view.
  void SetParent(NativeView* parent);
  void BecomeContentView(NativeWindow* window);

  void SetWindow(NativeWindow* window);

  // Internal: Whether this class inherits from Container.
  virtual bool IsContainer() const;

  // Internal: Notify that view's size has changed.
  virtual void OnSizeChanged();

  virtual void DetachChildView(NativeView* view);

  virtual void TriggerBeforeunloadEvents();

#if defined(OS_MAC)
  virtual void UpdateDraggableRegions();
#endif

#if !defined(OS_MAC)
  // Should delete the |view_| in destructor.
  void set_delete_view(bool should) { delete_view_ = should; }
#endif

 protected:
  virtual ~NativeView();

  // Called by subclasses to take the ownership of |view|.
  void TakeOverView(NATIVEVIEW view);

  void PlatformInit();
  void PlatformDestroy();
  void PlatformSetVisible(bool visible);

  virtual void SetWindowForChildren(NativeWindow* window);

 private:
  friend class base::RefCounted<NativeView>;

  // Relationships.
  NativeView* parent_ = nullptr;
  NativeWindow* window_ = nullptr;

  // The native implementation.
  NATIVEVIEW view_ = nullptr;

#if !defined(OS_MAC)
  bool delete_view_ = true;
#endif
};

}  // namespace electron

#endif  // SHELL_BROWSER_UI_NATIVE_VIEW_H_
