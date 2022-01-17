// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_scroll_view.h"

#include "shell/browser/ui/cocoa/electron_native_view.h"
#include "ui/gfx/geometry/point.h"
#include "ui/gfx/geometry/size.h"

@interface ElectronNativeScrollView : NSScrollView <ElectronNativeViewProtocol> {
 @private
  electron::NativeViewPrivate private_;
  electron::NativeScrollView* shell_;
  BOOL subscribed_;
  NSSize content_size_;
}
- (id)initWithShell:(electron::NativeScrollView*)shell;
- (void)setEventEnabled:(BOOL)enable;
- (void)setContentSize:(NSSize)size;
@end

@implementation ElectronNativeScrollView

- (id)initWithShell:(electron::NativeScrollView*)shell {
  if ((self = [super init])) {
    shell_ = shell;
    subscribed_ = NO;
  }
  return self;
}

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
  [super dealloc];
}

- (void)setEventEnabled:(BOOL)enable {
  if (subscribed_ == enable)
    return;
  subscribed_ = enable;
  NSView* contentView = self.contentView;
  if (enable) {
    [contentView setPostsBoundsChangedNotifications:YES];
    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(onScroll:)
               name:NSViewBoundsDidChangeNotification
             object:contentView];
  } else {
    [contentView setPostsBoundsChangedNotifications:NO];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
  }
}

- (void)setContentSize:(NSSize)size {
  content_size_ = size;
}

- (void)onScroll:(NSNotification*)notification {
#if 0
  shell_->on_scroll.Emit(shell_);
#endif
}

- (electron::NativeViewPrivate*)nativeViewPrivate {
  return &private_;
}

- (void)setNativeBackgroundColor:(SkColor)color {
}

- (void)resizeSubviewsWithOldSize:(NSSize)oldBoundsSize {
  // Automatically resize the content view when ScrollView is larger than the
  // content size.
  NSSize parent_size = [self frame].size;
  NSSize content_size = content_size_;
  if (content_size.width < parent_size.width)
    content_size.width = parent_size.width;
  if (content_size.height < parent_size.height)
    content_size.height = parent_size.height;
  [self.documentView setFrameSize:content_size];
  [super resizeSubviewsWithOldSize:oldBoundsSize];
}

@end

namespace electron {

void NativeScrollView::PlatformInit() {
  auto* scroll = [[ElectronNativeScrollView alloc] initWithShell:this];
  scroll.drawsBackground = NO;
  if (scroll.scrollerStyle == NSScrollerStyleOverlay) {
    scroll.hasHorizontalScroller = YES;
    scroll.hasVerticalScroller = YES;
  }
  [scroll.contentView setCopiesOnScroll:NO];
  TakeOverView(scroll);
}

void NativeScrollView::PlatformSetContentView(NativeView* view) {
  auto* scroll = static_cast<NSScrollView*>(GetNative());
  scroll.documentView = view->GetNative();
}

void NativeScrollView::PlatformDetachChildView() {
  auto* scroll = static_cast<NSScrollView*>(GetNative());
  scroll.documentView = nil;
}

void NativeScrollView::SetContentSize(const gfx::Size& size) {
  auto* scroll = static_cast<ElectronNativeScrollView*>(GetNative());
  NSSize content_size = size.ToCGSize();
  [scroll setContentSize:content_size];
  [scroll.documentView setFrameSize:content_size];
}

void NativeScrollView::SetScrollPosition(gfx::Point point) {
  auto* scroll = static_cast<ElectronNativeScrollView*>(GetNative());
  int vertical = point.y();
  if (![scroll.documentView isFlipped])
    vertical = NSHeight(scroll.documentView.bounds) - vertical;
  [scroll.documentView scrollPoint:NSMakePoint(point.x(), vertical)];
}

gfx::Point NativeScrollView::GetScrollPosition() const {
  auto* scroll = static_cast<ElectronNativeScrollView*>(GetNative());
  NSPoint point = scroll.contentView.bounds.origin;
  if (![scroll.documentView isFlipped]) {
    point.y = NSHeight(scroll.documentView.bounds) -
              NSHeight(scroll.contentView.bounds) - point.y;
  }
  return gfx::Point(point.x, point.y);
}

gfx::Point NativeScrollView::GetMaximumScrollPosition() const {
  auto* scroll = static_cast<ElectronNativeScrollView*>(GetNative());
  NSRect docBounds = scroll.documentView.bounds;
  NSRect clipBounds = scroll.contentView.bounds;
  return gfx::Point(NSMaxX(docBounds) - NSWidth(clipBounds),
                    NSMaxY(docBounds) - NSHeight(clipBounds));
}

void NativeScrollView::SetOverlayScrollbar(bool overlay) {
  auto* scroll = static_cast<ElectronNativeScrollView*>(GetNative());
  scroll.scrollerStyle =
      overlay ? NSScrollerStyleOverlay : NSScrollerStyleLegacy;
}

bool NativeScrollView::IsOverlayScrollbar() const {
  auto* scroll = static_cast<ElectronNativeScrollView*>(GetNative());
  return scroll.scrollerStyle == NSScrollerStyleOverlay;
}

void NativeScrollView::SetHorizontalScrollBarMode(ScrollBarMode mode) {
  auto* scroll = static_cast<ElectronNativeScrollView*>(GetNative());
  scroll.hasHorizontalScroller = mode != ScrollBarMode::kDisabled;
}

ScrollBarMode NativeScrollView::GetHorizontalScrollBarMode() const {
  auto* scroll = static_cast<ElectronNativeScrollView*>(GetNative());
  return scroll.hasHorizontalScroller ? ScrollBarMode::kEnabled : ScrollBarMode::kDisabled;
}

void NativeScrollView::SetVerticalScrollBarMode(ScrollBarMode mode) {
  auto* scroll = static_cast<ElectronNativeScrollView*>(GetNative());
  scroll.hasVerticalScroller = mode != ScrollBarMode::kDisabled;
}

ScrollBarMode NativeScrollView::GetVerticalScrollBarMode() const {
  auto* scroll = static_cast<ElectronNativeScrollView*>(GetNative());
  return scroll.hasVerticalScroller ? ScrollBarMode::kEnabled : ScrollBarMode::kDisabled;
}

void NativeScrollView::SetHorizontalScrollElasticity(ScrollElasticity elasticity) {
  auto* scroll = static_cast<ElectronNativeScrollView*>(GetNative());
  scroll.horizontalScrollElasticity = static_cast<NSScrollElasticity>(elasticity);
}

ScrollElasticity NativeScrollView::GetHorizontalScrollElasticity() const {
  auto* scroll = static_cast<ElectronNativeScrollView*>(GetNative());
  return static_cast<ScrollElasticity>(scroll.horizontalScrollElasticity);
}

void NativeScrollView::SetVerticalScrollElasticity(ScrollElasticity elasticity) {
  auto* scroll = static_cast<ElectronNativeScrollView*>(GetNative());
  scroll.verticalScrollElasticity = static_cast<NSScrollElasticity>(elasticity);
}

ScrollElasticity NativeScrollView::GetVerticalScrollElasticity() const {
  auto* scroll = static_cast<ElectronNativeScrollView*>(GetNative());
  return static_cast<ScrollElasticity>(scroll.verticalScrollElasticity);
}

void NativeScrollView::UpdateDraggableRegions() {
  if (content_view_.get())
    content_view_->UpdateDraggableRegions();
}

#if 0
void NativeScrollView::OnConnect(int identifier) {
  if (identifier == kOnScroll) {
    auto* scroll = static_cast<ElectronNativeScrollView*>(GetNative());
    [scroll setEventEnabled:YES];
  }
}
#endif

}  // namespace electron
