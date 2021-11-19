#include "shell/browser/ui/native_scroll.h"

#include <utility>

#include "shell/browser/ui/native_container.h"
#include "ui/gfx/geometry/size_conversions.h"

namespace electron {

// static
const char NativeScroll::kClassName[] = "Scroll";

NativeScroll::NativeScroll() {
  PlatformInit();
  SetContentView(new NativeContainer);
#if 0
  on_scroll.SetDelegate(this, kOnScroll);
#endif
}

NativeScroll::~NativeScroll() {
}

void NativeScroll::SetContentView(scoped_refptr<NativeView> view) {
  if (content_view_)
    content_view_->SetParent(nullptr);
  PlatformSetContentView(view.get());
  content_view_ = std::move(view);
  content_view_->SetParent(this);
}

NativeView* NativeScroll::GetContentView() const {
  return content_view_.get();
}

gfx::SizeF NativeScroll::GetContentSize() const {
  return GetContentView()->GetBounds().size();
}

const char* NativeScroll::GetClassName() const {
  return kClassName;
}

}  // namespace electron
