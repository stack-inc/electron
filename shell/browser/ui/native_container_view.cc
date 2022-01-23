// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_container_view.h"

#include <algorithm>
#include <limits>
#include <utility>

#include "base/logging.h"
#include "shell/browser/browser.h"
#include "third_party/yoga/Yoga.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/rect_conversions.h"
#include "ui/gfx/geometry/rect_f.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/size_conversions.h"
#include "ui/gfx/geometry/size_f.h"

namespace electron {

namespace {

// Whether a Container is a root CSS node.
inline bool IsRootYGNode(NativeContainerView* view) {
  return !YGNodeGetParent(view->node()) || !view->IsContainer();
}

// Get bounds from the CSS node.
inline gfx::Rect GetYGNodeBounds(YGNodeRef node) {
  return ToNearestRect(
      gfx::RectF(YGNodeLayoutGetLeft(node), YGNodeLayoutGetTop(node),
                 YGNodeLayoutGetWidth(node), YGNodeLayoutGetHeight(node)));
}

}  // namespace

NativeContainerView::NativeContainerView() {
  PlatformInit();
}

NativeContainerView::NativeContainerView(const char* an_empty_constructor) {}

NativeContainerView::~NativeContainerView() {
  PlatformDestroy();
}

void NativeContainerView::Layout() {
  if (!Browser::Get()->use_yoga())
    return;
  // For child CSS node, tell parent to do the layout.
  if (!IsRootYGNode(this)) {
    dirty_ = true;
    static_cast<NativeContainerView*>(GetParent())->Layout();
    // The parent may choose to not update this view because its size is not
    // changed, in that case we need to force updating here.
    // This usually happens after adding a child view, since the container does
    // not change its size.
    // TODO(zcbenz): Revisit the logic here, should have a cleaner way.
    if (dirty_)
      SetChildBoundsFromCSS();
    return;
  }

  // So this is a root CSS node, calculate the layout and set bounds.
  gfx::Size size(GetBounds().size());
  YGNodeCalculateLayout(node(), size.width(), size.height(), YGDirectionLTR);
  SetChildBoundsFromCSS();
}

bool NativeContainerView::IsContainer() const {
  return true;
}

void NativeContainerView::OnSizeChanged() {
  NativeView::OnSizeChanged();
  if (!Browser::Get()->use_yoga())
    return;
  if (IsRootYGNode(this))
    Layout();
  else
    SetChildBoundsFromCSS();
}

void NativeContainerView::DetachChildView(NativeView* view) {
  RemoveChildView(view);
}

void NativeContainerView::TriggerBeforeunloadEvents() {
  for (auto view : children_)
    view->TriggerBeforeunloadEvents();
}

void NativeContainerView::SetWindowForChildren(NativeWindow* window) {
  for (auto view : children_)
    view->SetWindow(window);
}

gfx::Size NativeContainerView::GetPreferredSize() const {
  float nan = std::numeric_limits<float>::quiet_NaN();
  YGNodeCalculateLayout(node(), nan, nan, YGDirectionLTR);
  return ToRoundedSize(
      gfx::SizeF(YGNodeLayoutGetWidth(node()), YGNodeLayoutGetHeight(node())));
}

float NativeContainerView::GetPreferredHeightForWidth(float width) const {
  float nan = std::numeric_limits<float>::quiet_NaN();
  YGNodeCalculateLayout(node(), width, nan, YGDirectionLTR);
  return YGNodeLayoutGetHeight(node());
}

float NativeContainerView::GetPreferredWidthForHeight(float height) const {
  float nan = std::numeric_limits<float>::quiet_NaN();
  YGNodeCalculateLayout(node(), nan, height, YGDirectionLTR);
  return YGNodeLayoutGetWidth(node());
}

void NativeContainerView::AddChildView(scoped_refptr<NativeView> view) {
  if (!view)
    return;
  if (view->GetParent() == this)
    return;
  AddChildViewAt(std::move(view), ChildCount());
}

void NativeContainerView::AddChildViewAt(scoped_refptr<NativeView> view,
                                         int index) {
  if (!view)
    return;
  if (view == this || index < 0 || index > ChildCount())
    return;

  if (view->GetParent()) {
    LOG(ERROR) << "The view already has a parent.";
    return;
  }

  YGNodeInsertChild(node(), view->node(), index);
  view->SetParent(this);

  PlatformAddChildView(view.get());
  children_.insert(children_.begin() + index, std::move(view));

  DCHECK_EQ(static_cast<int>(YGNodeGetChildCount(node())), ChildCount());

  Layout();
}

void NativeContainerView::RemoveChildView(NativeView* view) {
  if (!view)
    return;
  const auto i(std::find(children_.begin(), children_.end(), view));
  if (i == children_.end())
    return;

  view->SetParent(nullptr);
  YGNodeRemoveChild(node(), view->node());

  PlatformRemoveChildView(view);
  children_.erase(i);

  DCHECK_EQ(static_cast<int>(YGNodeGetChildCount(node())), ChildCount());

  Layout();
}

void NativeContainerView::SetTopChildView(NativeView* view) {
  if (!view || view == this)
    return;
  const auto i(std::find(children_.begin(), children_.end(), view));
  if (i == children_.end())
    return;

  YGNodeRemoveChild(node(), view->node());
  YGNodeInsertChild(node(), view->node(), ChildCount());

  PlatformSetTopView(view);
  auto view_it = children_.erase(i);
  children_.insert(children_.begin() + children_.size(), std::move(*view_it));

  DCHECK_EQ(static_cast<int>(YGNodeGetChildCount(node())), ChildCount());

  Layout();
}

void NativeContainerView::SetChildBoundsFromCSS() {
  dirty_ = false;
  if (!IsVisible())
    return;
  for (int i = 0; i < ChildCount(); ++i) {
    NativeView* child = ChildAt(i);
    if (child->IsVisible()) {
      auto cbounds = GetYGNodeBounds(child->node());
      child->SetBounds(cbounds);
    }
  }
}

}  // namespace electron
