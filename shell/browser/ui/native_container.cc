// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_container.h"

#include <algorithm>
#include <limits>
#include <utility>

#include "base/logging.h"
#include "content/public/browser/web_contents.h"
#include "shell/browser/api/electron_api_web_contents.h"
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
inline bool IsRootYGNode(NativeContainer* view) {
  return !YGNodeGetParent(view->node()) || !view->IsContainer();
}

// Get bounds from the CSS node.
inline gfx::Rect GetYGNodeBounds(YGNodeRef node) {
  return ToNearestRect(gfx::RectF(YGNodeLayoutGetLeft(node),
                                  YGNodeLayoutGetTop(node),
                                  YGNodeLayoutGetWidth(node),
                                  YGNodeLayoutGetHeight(node)));
}

}  // namespace

NativeContainer::NativeContainer() {
  PlatformInit();
}

NativeContainer::NativeContainer(const char* an_empty_constructor) {
}

NativeContainer::~NativeContainer() {
  PlatformDestroy();
}

void NativeContainer::Layout() {
  if (!Browser::Get()->use_yoga())
    return;
  // For child CSS node, tell parent to do the layout.
  if (!IsRootYGNode(this)) {
    dirty_ = true;
    static_cast<NativeContainer*>(GetParent())->Layout();
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

bool NativeContainer::IsContainer() const {
  return true;
}

void NativeContainer::OnSizeChanged() {
  NativeView::OnSizeChanged();
  if (!Browser::Get()->use_yoga())
    return;
  if (IsRootYGNode(this))
    Layout();
  else
    SetChildBoundsFromCSS();
}

gfx::Size NativeContainer::GetPreferredSize() const {
  float nan = std::numeric_limits<float>::quiet_NaN();
  YGNodeCalculateLayout(node(), nan, nan, YGDirectionLTR);
  return ToRoundedSize(
      gfx::SizeF(YGNodeLayoutGetWidth(node()), YGNodeLayoutGetHeight(node())));
}

float NativeContainer::GetPreferredHeightForWidth(float width) const {
  float nan = std::numeric_limits<float>::quiet_NaN();
  YGNodeCalculateLayout(node(), width, nan, YGDirectionLTR);
  return YGNodeLayoutGetHeight(node());
}

float NativeContainer::GetPreferredWidthForHeight(float height) const {
  float nan = std::numeric_limits<float>::quiet_NaN();
  YGNodeCalculateLayout(node(), nan, height, YGDirectionLTR);
  return YGNodeLayoutGetWidth(node());
}

void NativeContainer::AddChildView(NativeBrowserView* view) {
  if (!view)
    return;
  AddChildViewAt(view, BrowserChildCount());
}

void NativeContainer::AddChildView(scoped_refptr<NativeView> view) {
  if (!view)
    return;
  if (view->GetParent() == this)
    return;
  AddChildViewAt(std::move(view), ChildCount());
}

void NativeContainer::AddChildViewAt(NativeBrowserView* view, int index) {
  if (!view)
    return;
  if (index < 0 || index > ChildCount())
    return;

  //YGNodeInsertChild(node(), view->node(), index);
  //view->SetParent(this);
//{
  //view->SetOwnerWindow(GetOwnerWindow());
//}

  PlatformAddChildView(view);
  browser_children_.insert(browser_children_.begin() + index, view);

  //DCHECK_EQ(static_cast<int>(YGNodeGetChildCount(node())), ChildCount());

  Layout();
}

void NativeContainer::AddChildViewAt(scoped_refptr<NativeView> view, int index) {
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
//{
  //?view->SetOwnerWindow(GetOwnerWindow());
//}

  PlatformAddChildView(view.get());
  children_.insert(children_.begin() + index, std::move(view));

  DCHECK_EQ(static_cast<int>(YGNodeGetChildCount(node())), ChildCount());

  Layout();
}


void NativeContainer::RemoveChildView(NativeBrowserView* view) {
  if (!view)
    return;
  const auto i(std::find(browser_children_.begin(), browser_children_.end(), view));
  if (i == browser_children_.end())
    return;

  //view->SetParent(nullptr);
//{
  //view->set_owner_window(nullptr);
//}
  //YGNodeRemoveChild(node(), view->node());

  PlatformRemoveChildView(view);
  browser_children_.erase(i);

  //DCHECK_EQ(static_cast<int>(YGNodeGetChildCount(node())), ChildCount());

  Layout();
}

void NativeContainer::RemoveChildView(NativeView* view) {
  if (!view)
    return;
  const auto i(std::find(children_.begin(), children_.end(), view));
  if (i == children_.end())
    return;

  view->SetParent(nullptr);
//{
  //?view->SetWindow(nullptr);
//}
  YGNodeRemoveChild(node(), view->node());

  PlatformRemoveChildView(view);
  children_.erase(i);

  DCHECK_EQ(static_cast<int>(YGNodeGetChildCount(node())), ChildCount());

  Layout();
}

void NativeContainer::SetTopChildView(NativeBrowserView* view) {
  if (!view)
    return;

  const auto i(std::find(browser_children_.begin(), browser_children_.end(), view));
  if (i == browser_children_.end())
    return;

  //view->SetParent(nullptr);
//{
  //view->set_owner_window(nullptr);
//}
  //YGNodeRemoveChild(node(), view->node());

  PlatformRemoveChildView(view);
  auto view_it = browser_children_.erase(i);

  //DCHECK_EQ(static_cast<int>(YGNodeGetChildCount(node())), ChildCount());

  //YGNodeInsertChild(node(), view->node(), index);
  //view->SetParent(this);
//{
  //view->SetOwnerWindow(GetOwnerWindow());
//}

  PlatformAddChildView(*view_it);
  browser_children_.push_back(std::move(*view_it));

  //DCHECK_EQ(static_cast<int>(YGNodeGetChildCount(node())), ChildCount());

  Layout();
}

void NativeContainer::SetTopChildView(NativeView* view) {
  if (!view || view == this)
    return;
  if (view->GetParent() != this)
    return;

  const auto i(std::find(children_.begin(), children_.end(), view));
  if (i == children_.end())
    return;

  view->SetParent(nullptr);
//{
  //?view->SetWindow(nullptr);
//}
  YGNodeRemoveChild(node(), view->node());

  PlatformRemoveChildView(view);
  auto view_it = children_.erase(i);

  DCHECK_EQ(static_cast<int>(YGNodeGetChildCount(node())), ChildCount());

  YGNodeInsertChild(node(), view->node(), ChildCount());
  view->SetParent(this);
//{
  //?view->SetOwnerWindow(GetOwnerWindow());
//}

  PlatformAddChildView((*view_it).get());
  children_.push_back(std::move(*view_it));

  DCHECK_EQ(static_cast<int>(YGNodeGetChildCount(node())), ChildCount());

  Layout();
}

void NativeContainer::SetChildBoundsFromCSS() {
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

void NativeContainer::SetOwnerWindowForChildren(NativeWindow* window) {
  for (NativeBrowserView* view : browser_children_) {
    auto* vwc = view->web_contents();
    auto* api_web_contents = api::WebContents::From(vwc);
    if (api_web_contents)
      api_web_contents->SetOwnerWindow(window);
  }

  for (auto view : children_)
    if (view->IsContainer())
      static_cast<NativeContainer*>(view.get())->SetOwnerWindowForChildren(window);
}

void NativeContainer::SetOwnerWindow(NativeWindow* window) {
  SetOwnerWindowForChildren(window);
  SetWindow(window);
}

NativeWindow* NativeContainer::GetOwnerWindow() {
  NativeView* view = this;
  while (view->GetParent())
    view = view->GetParent();
  return view->GetWindow();
}

void NativeContainer::TriggerBeforeunloadEvents() {
  for (NativeBrowserView* view : browser_children_) {
    auto* vwc = view->web_contents();
    auto* api_web_contents = api::WebContents::From(vwc);

    // Required to make beforeunload handler work.
    if (api_web_contents)
      api_web_contents->NotifyUserActivation();

    if (vwc) {
      if (vwc->NeedToFireBeforeUnloadOrUnloadEvents()) {
        vwc->DispatchBeforeUnload(false /* auto_cancel */);
      }
    }
  }

  for (auto view : children_)
    if (view->IsContainer())
      static_cast<NativeContainer*>(view.get())->TriggerBeforeunloadEvents();
}

#if defined(OS_MAC)
void NativeContainer::UpdateDraggableRegions() {
  for (NativeBrowserView* view : browser_children_)
    view->UpdateDraggableRegions(view->GetDraggableRegions());

  for (auto view : children_)
    if (view->IsContainer())
      static_cast<NativeContainer*>(view.get())->UpdateDraggableRegions();
}
#endif

}  // namespace electron
