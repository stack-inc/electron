// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/views/native_container_view_impl.h"

#include <utility>

#include "base/memory/ptr_util.h"
#include "shell/browser/native_browser_view.h"
#include "shell/browser/ui/inspectable_web_contents_view.h"
#include "shell/browser/ui/native_container_view.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/views/background.h"
#include "ui/views/layout/layout_manager.h"

namespace electron {

NativeContainerViewImpl::NativeContainerViewImpl(views::View* view)
    : view_(view) {
  DCHECK(view_);
}

NativeContainerViewImpl::~NativeContainerViewImpl() {
  if (delete_view_)
    delete view_;
}


views::View* NativeContainerViewImpl::GetView() {
  return view_;
}

void NativeContainerViewImpl::SetAutoResizeFlags(uint8_t flags) {
  auto_resize_flags_ = flags;
  ResetAutoResizeProportions();
}

void NativeContainerViewImpl::SetAutoResizeProportions(
    const gfx::Size& window_size) {
  if ((auto_resize_flags_ & AutoResizeFlags::kAutoResizeHorizontal) &&
      !auto_horizontal_proportion_set_) {
    auto view_bounds = view_->bounds();
    auto_horizontal_proportion_width_ =
        static_cast<float>(window_size.width()) /
        static_cast<float>(view_bounds.width());
    auto_horizontal_proportion_left_ = static_cast<float>(window_size.width()) /
                                       static_cast<float>(view_bounds.x());
    auto_horizontal_proportion_set_ = true;
  }
  if ((auto_resize_flags_ & AutoResizeFlags::kAutoResizeVertical) &&
      !auto_vertical_proportion_set_) {
    auto view_bounds = view_->bounds();
    auto_vertical_proportion_height_ =
        static_cast<float>(window_size.height()) /
        static_cast<float>(view_bounds.height());
    auto_vertical_proportion_top_ = static_cast<float>(window_size.height()) /
                                    static_cast<float>(view_bounds.y());
    auto_vertical_proportion_set_ = true;
  }
}

void NativeContainerViewImpl::AutoResize(const gfx::Rect& new_window,
                                         int width_delta,
                                         int height_delta) {
  const auto flags = GetAutoResizeFlags();
  if (!(flags & kAutoResizeWidth)) {
    width_delta = 0;
  }
  if (!(flags & kAutoResizeHeight)) {
    height_delta = 0;
  }
  if (height_delta || width_delta) {
    auto new_view_size = view_->size();
    new_view_size.set_width(new_view_size.width() + width_delta);
    new_view_size.set_height(new_view_size.height() + height_delta);
    view_->SetSize(new_view_size);
  }
  auto new_view_bounds = view_->bounds();
  if (flags & kAutoResizeHorizontal) {
    new_view_bounds.set_width(new_window.width() /
                              auto_horizontal_proportion_width_);
    new_view_bounds.set_x(new_window.width() /
                          auto_horizontal_proportion_left_);
  }
  if (flags & kAutoResizeVertical) {
    new_view_bounds.set_height(new_window.height() /
                               auto_vertical_proportion_height_);
    new_view_bounds.set_y(new_window.height() / auto_vertical_proportion_top_);
  }
  if ((flags & kAutoResizeHorizontal) || (flags & kAutoResizeVertical)) {
    view_->SetBoundsRect(new_view_bounds);
  }
}

void NativeContainerViewImpl::ResetAutoResizeProportions() {
  if (auto_resize_flags_ & AutoResizeFlags::kAutoResizeHorizontal) {
    auto_horizontal_proportion_set_ = false;
  }
  if (auto_resize_flags_ & AutoResizeFlags::kAutoResizeVertical) {
    auto_vertical_proportion_set_ = false;
  }
}

void NativeContainerViewImpl::SetBounds(const gfx::Rect& bounds) {
  view_->SetBoundsRect(bounds);
  ResetAutoResizeProportions();
}

gfx::Rect NativeContainerViewImpl::GetBounds() {
  return view_->bounds();
}

void NativeContainerViewImpl::SetBackgroundColor(SkColor color) {
  view_->SetBackground(views::CreateSolidBackground(color));
  view_->SchedulePaint();
}

void NativeContainerViewImpl::AddBrowserView(NativeBrowserView* view) {
  if (!view)
    return;

  if (view->GetInspectableWebContentsView()) {
    view_->AddChildView(view->GetInspectableWebContentsView()->GetView());
    view_->Layout();
  }
}

void NativeContainerViewImpl::RemoveBrowserView(NativeBrowserView* view) {
  if (!view)
    return;

  if (view->GetInspectableWebContentsView()) {
    view_->RemoveChildView(view->GetInspectableWebContentsView()->GetView());
  view_->Layout();
  }
}

void NativeContainerViewImpl::SetTopBrowserView(NativeBrowserView* view) {
  if (!view)
    return;

  if (view->GetInspectableWebContentsView()) {
    view_->ReorderChildView(view->GetInspectableWebContentsView()->GetView(),
                            -1);
  view_->Layout();
  }
}

void NativeContainerViewImpl::AddContainerView(NativeContainerViewImpl* view) {
  if (!view)
    return;

  view->set_delete_view(false);
  view_->AddChildView(view->GetView());
  view_->Layout();
}

void NativeContainerViewImpl::RemoveContainerView(
    NativeContainerViewImpl* view) {
  if (!view)
    return;

  view->set_delete_view(true);
  view_->RemoveChildView(view->GetView());
  view_->Layout();
}

void NativeContainerViewImpl::SetTopContainerView(
    NativeContainerViewImpl* view) {
  if (!view)
    return;

  view_->ReorderChildView(view->GetView(), -1);
  view_->Layout();
}

void NativeContainerViewImpl::SetLayoutManager(views::LayoutManager* layout_manager) {
  if (!view_)
    return;

  view_->SetLayoutManager(base::WrapUnique(layout_manager));
}

// static
NativeContainerView* NativeContainerView::Create() {
  return new NativeContainerView(
      new NativeContainerViewImpl(new views::View()));
}

}  // namespace electron
