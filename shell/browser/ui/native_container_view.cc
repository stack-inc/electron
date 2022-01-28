// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_container_view.h"

#include <algorithm>
#include <limits>
#include <utility>

#include "base/logging.h"
#include "shell/browser/browser.h"
#include "ui/gfx/geometry/rect_conversions.h"
#include "ui/gfx/geometry/rect_f.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/size_conversions.h"
#include "ui/gfx/geometry/size_f.h"

namespace electron {

NativeContainerView::NativeContainerView() {
  PlatformInit();
}

NativeContainerView::NativeContainerView(const char* an_empty_constructor) {}

NativeContainerView::~NativeContainerView() {
  PlatformDestroy();
}

bool NativeContainerView::IsContainer() const {
  return true;
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

void NativeContainerView::AddChildView(scoped_refptr<NativeView> view) {
  if (!GetNative() || !view)
    return;
  if (view->GetParent() == this)
    return;
  AddChildViewAt(std::move(view), ChildCount());
}

void NativeContainerView::AddChildViewAt(scoped_refptr<NativeView> view,
                                         int index) {
  if (!GetNative() || !view)
    return;
  if (view == this || index < 0 || index > ChildCount())
    return;

  if (view->GetParent()) {
    LOG(ERROR) << "The view already has a parent.";
    return;
  }

  view->SetParent(this);

  PlatformAddChildView(view.get());
  children_.insert(children_.begin() + index, std::move(view));
}

void NativeContainerView::RemoveChildView(NativeView* view) {
  if (!GetNative() || !view)
    return;
  const auto i(std::find(children_.begin(), children_.end(), view));
  if (i == children_.end())
    return;

  view->SetParent(nullptr);

  PlatformRemoveChildView(view);
  children_.erase(i);
}

void NativeContainerView::SetTopChildView(NativeView* view) {
  if (!GetNative() || !view || view == this)
    return;
  const auto i(std::find(children_.begin(), children_.end(), view));
  if (i == children_.end())
    return;

  PlatformSetTopView(view);
  auto view_it = children_.erase(i);
  children_.insert(children_.begin() + children_.size(), std::move(*view_it));
}

}  // namespace electron
