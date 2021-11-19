#include "shell/browser/ui/native_container.h"

#include <algorithm>
#include <limits>
#include <utility>

#include "base/logging.h"
#include "shell/browser/browser.h"
#include "third_party/yoga/Yoga.h"

namespace electron {

namespace {

// Whether a Container is a root CSS node.
inline bool IsRootYGNode(NativeContainer* view) {
  return !YGNodeGetParent(view->node()) || !view->IsContainer();
}

// Get bounds from the CSS node.
inline gfx::RectF GetYGNodeBounds(YGNodeRef node) {
  return gfx::RectF(YGNodeLayoutGetLeft(node), YGNodeLayoutGetTop(node),
               YGNodeLayoutGetWidth(node), YGNodeLayoutGetHeight(node));
}

}  // namespace

// static
const char NativeContainer::kClassName[] = "Container";

NativeContainer::NativeContainer() {
  PlatformInit();
}

NativeContainer::NativeContainer(const char* an_empty_constructor) {
}

NativeContainer::~NativeContainer() {
  PlatformDestroy();
}

const char* NativeContainer::GetClassName() const {
  return kClassName;
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
  gfx::SizeF size(GetBounds().size());
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

gfx::SizeF NativeContainer::GetPreferredSize() const {
  float nan = std::numeric_limits<float>::quiet_NaN();
  YGNodeCalculateLayout(node(), nan, nan, YGDirectionLTR);
  return gfx::SizeF(YGNodeLayoutGetWidth(node()), YGNodeLayoutGetHeight(node()));
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

void NativeContainer::AddChildView(scoped_refptr<NativeView> view) {
  DCHECK(view);
  if (view->GetParent() == this)
    return;
  AddChildViewAt(std::move(view), ChildCount());
}

void NativeContainer::AddChildViewAt(scoped_refptr<NativeView> view, int index) {
  DCHECK(view);
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

void NativeContainer::RemoveChildView(NativeView* view) {
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

}  // namespace electron
