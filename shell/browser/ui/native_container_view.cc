// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/native_container_view.h"

#include "content/public/browser/web_contents.h"
#include "shell/browser/api/electron_api_web_contents.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/views/background.h"

#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
#include "shell/browser/ui/views/native_container_view_impl.h"
#endif

#if defined(OS_MAC)
#include "shell/browser/ui/cocoa/native_container_view_impl.h"
#endif

namespace electron {

NativeContainerView::NativeContainerView(
    NativeContainerViewImpl* container_view_impl)
    : container_view_impl_(container_view_impl) {}

NativeContainerView::~NativeContainerView() = default;

#if defined(OS_MAC)
NativeView* NativeContainerView::GetView() const {
  if (container_view_impl_)
    return container_view_impl_->GetView();
  return nullptr;
}
#elif defined(TOOLKIT_VIEWS)
views::View* NativeContainerView::GetView() const {
  if (container_view_impl_)
    return container_view_impl_->GetView();
  return nullptr;
}
#endif

NativeContainerViewImpl* NativeContainerView::GetViewImpl() {
  return container_view_impl_;
}

void NativeContainerView::SetAutoResizeFlags(uint8_t flags) {
  if (container_view_impl_)
    container_view_impl_->SetAutoResizeFlags(flags);
}

void NativeContainerView::SetBounds(const gfx::Rect& bounds) {
  if (container_view_impl_)
    container_view_impl_->SetBounds(bounds);
}

gfx::Rect NativeContainerView::GetBounds() {
  if (container_view_impl_)
    return container_view_impl_->GetBounds();
  return gfx::Rect();
}

void NativeContainerView::SetBackgroundColor(SkColor color) {
  if (container_view_impl_)
    container_view_impl_->SetBackgroundColor(color);
}

void NativeContainerView::AddBrowserView(NativeBrowserView* view) {
  if (!view)
    return;

  add_browser_view(view);

  if (container_view_impl_)
    container_view_impl_->AddBrowserView(view);
}

void NativeContainerView::RemoveBrowserView(NativeBrowserView* view) {
  if (!view)
    return;

  if (container_view_impl_)
    container_view_impl_->RemoveBrowserView(view);

  remove_browser_view(view);
}

void NativeContainerView::SetTopBrowserView(NativeBrowserView* view) {
  if (!view)
    return;

  remove_browser_view(view);
  add_browser_view(view);

  if (container_view_impl_)
    container_view_impl_->SetTopBrowserView(view);
}

void NativeContainerView::AddContainerView(NativeContainerView* view) {
  if (!view)
    return;

  add_container_view(view);
  view->set_parent_view(this);
  view->SetOwnerWindow(GetOwnerWindow());

  if (container_view_impl_)
    container_view_impl_->AddContainerView(view->GetViewImpl());
}

void NativeContainerView::RemoveContainerView(NativeContainerView* view) {
  if (!view)
    return;

  if (container_view_impl_)
    container_view_impl_->RemoveContainerView(view->GetViewImpl());

  remove_container_view(view);
  view->set_parent_view(nullptr);
  view->set_owner_window(nullptr);
}

void NativeContainerView::SetTopContainerView(NativeContainerView* view) {
  if (!view)
    return;

  remove_container_view(view);
  add_container_view(view);
  view->set_parent_view(this);
  view->SetOwnerWindow(GetOwnerWindow());

  if (container_view_impl_)
    container_view_impl_->SetTopContainerView(view->GetViewImpl());
}

#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
void NativeContainerView::SetLayoutManager(views::LayoutManager* layout_manager) {
  if (container_view_impl_)
    container_view_impl_->SetLayoutManager(layout_manager);
}
#endif

void NativeContainerView::SetStyleProperty(const std::string& name, const std::string& value) {
#if defined(OS_MAC)
  if (container_view_impl_)
    container_view_impl_->SetStyleProperty(name, value);
#endif
}

void NativeContainerView::SetStyleProperty(const std::string& name, float value) {
#if defined(OS_MAC)
  if (container_view_impl_)
    container_view_impl_->SetStyleProperty(name, value);
#endif
}

void NativeContainerView::Layout() {
#if defined(OS_MAC)
  if (container_view_impl_)
    container_view_impl_->Layout();
#endif
}

void NativeContainerView::SetOwnerWindowForChildren(NativeWindow* window) {
  for (NativeBrowserView* view : browser_views_) {
    auto* vwc = view->web_contents();
    auto* api_web_contents = api::WebContents::From(vwc);
    if (api_web_contents)
      api_web_contents->SetOwnerWindow(window);
  }

  for (NativeContainerView* view : container_views_)
    view->SetOwnerWindowForChildren(window);
}

void NativeContainerView::SetOwnerWindow(NativeWindow* window) {
  SetOwnerWindowForChildren(window);
  owner_window_ = window;
}

NativeWindow* NativeContainerView::GetOwnerWindow() {
  NativeContainerView* view = this;
  while (view->parent_view_)
    view = view->parent_view_;
  return view->owner_window_;
}

void NativeContainerView::TriggerBeforeunloadEvents() {
  for (NativeBrowserView* view : browser_views_) {
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

  for (NativeContainerView* view : container_views_)
    view->TriggerBeforeunloadEvents();
}

#if defined(OS_MAC)
void NativeContainerView::UpdateDraggableRegions() {
  for (NativeBrowserView* view : browser_views_)
    view->UpdateDraggableRegions(view->GetDraggableRegions());

  for (NativeContainerView* view : container_views_)
    view->UpdateDraggableRegions();
}
#endif

}  // namespace electron
