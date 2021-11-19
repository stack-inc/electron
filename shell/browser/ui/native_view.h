#ifndef SHELL_BROWSER_UI_NATIVE_VIEW_H_
#define SHELL_BROWSER_UI_NATIVE_VIEW_H_

#include <set>
#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/geometry/rect_f.h"
#include "ui/gfx/geometry/size_f.h"

typedef struct YGNode *YGNodeRef;
typedef struct YGConfig *YGConfigRef;

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
  // The view class name.
  static const char kClassName[];

  // Return the receiving view's class name. A view class is a string which
  // uniquely identifies the view class. It is intended to be used as a way to
  // find out during run time if a view can be safely casted to a specific view
  // subclass.
  virtual const char* GetClassName() const;

  // Coordiante convertions.
  gfx::Vector2dF OffsetFromView(const NativeView* from) const;
  gfx::Vector2dF OffsetFromWindow() const;

  // Internal: Change position and size.
  virtual void SetBounds(const gfx::RectF& bounds);

  // Get position and size.
  gfx::RectF GetBounds() const;

  // Internal: The real pixel bounds that depends on the scale factor.
  void SetPixelBounds(const gfx::Rect& bounds);
  gfx::Rect GetPixelBounds() const;

  // Update layout.
  virtual void Layout();

  // Mark the whole view as dirty.
  void SchedulePaint();

  // Repaint the rect
  void SchedulePaintRect(const gfx::RectF& rect);

  // Show/Hide the view.
  void SetVisible(bool visible);
  bool IsVisible() const;

  // Whether the view and its parent are visible.
  bool IsTreeVisible() const;

  // Move the keyboard focus to the view.
  void Focus();
  bool HasFocus() const;

  // Whether the view can be focused.
  void SetFocusable(bool focusable);
  bool IsFocusable() const;

  // Display related styles.
  void SetBackgroundColor(SkColor color);

  // Set layout related styles without doing layout.
  // While this is public API, it should only be used by language bindings.
  void SetStyleProperty(const std::string& name, const std::string& value);
  void SetStyleProperty(const std::string& name, float value);

  // Set styles and re-compute the layout.
  template<typename... Args>
  void SetStyle(const std::string& name, const std::string& value,
                Args... args) {
    SetStyleProperty(name, value);
    SetStyle(args...);
    Layout();
  }
  template<typename... Args>
  void SetStyle(const std::string& name, float value, Args... args) {
    SetStyleProperty(name, value);
    SetStyle(args...);
    Layout();
  }
  void SetStyle() {
  }

  // Return the string representation of yoga style.
  std::string GetComputedLayout() const;

  // Return the minimum size of view.
  virtual gfx::SizeF GetMinimumSize() const;

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

  // Internal: Whether this class inherits from Container.
  virtual bool IsContainer() const;

  // Internal: Notify that view's size has changed.
  virtual void OnSizeChanged();

  // Internal: Get the CSS node of the view.
  YGNodeRef node() const { return node_; }

 protected:
  NativeView();
  virtual ~NativeView();

  // Update the default style.
  void UpdateDefaultStyle();

  // Called by subclasses to take the ownership of |view|.
  void TakeOverView(NATIVEVIEW view);

  void PlatformInit();
  void PlatformDestroy();
  void PlatformSetVisible(bool visible);

 private:
  friend class base::RefCounted<NativeView>;

  // Relationships.
  NativeView* parent_ = nullptr;
  NativeWindow* window_ = nullptr;

  // The native implementation.
  NATIVEVIEW view_;

  // The config of its yoga node.
  YGConfigRef yoga_config_;

  // The node recording CSS styles.
  YGNodeRef node_;
};

}  // namespace electron

#endif  // SHELL_BROWSER_UI_NATIVE_VIEW_H_
