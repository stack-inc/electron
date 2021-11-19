#ifndef SHELL_BROWSER_UI_NATIVE_SCROLL_H_
#define SHELL_BROWSER_UI_NATIVE_SCROLL_H_

#include <tuple>

#include "shell/browser/ui/native_view.h"

namespace electron {

class NativeScroll : public NativeView {
 public:
  NativeScroll();

  // View class name.
  static const char kClassName[];

  void SetContentView(scoped_refptr<NativeView> view);
  NativeView* GetContentView() const;

  void SetContentSize(const gfx::SizeF& size);
  gfx::SizeF GetContentSize() const;
  void SetScrollPosition(float horizon, float vertical);
  std::tuple<float, float> GetScrollPosition() const;
  std::tuple<float, float> GetMaximumScrollPosition() const;

#if !defined(OS_WIN)
  void SetOverlayScrollbar(bool overlay);
  bool IsOverlayScrollbar() const;
#endif

  enum class Policy {
    Always,
    Never,
    Automatic,
  };
  void SetScrollbarPolicy(Policy h_policy, Policy v_policy);
  std::tuple<Policy, Policy> GetScrollbarPolicy() const;

#if defined(OS_MAC)
  enum class Elasticity {
    Automatic = 0,  // NSScrollElasticityAutomatic = 0
    None = 1,       // NSScrollElasticityNone = 1
    Allowed = 2,    // NSScrollElasticityAllowed = 2
  };
  void SetScrollElasticity(Elasticity h, Elasticity v);
  std::tuple<Elasticity, Elasticity> GetScrollElasticity() const;
#endif

  // NativeView:
  const char* GetClassName() const override;

#if 0
  // Events.
  Signal<bool(Scroll*)> on_scroll;
#endif

 protected:
  ~NativeScroll() override;

  // Following platform implementations should only be called by wrappers.
  void PlatformInit();
  void PlatformSetContentView(NativeView* container);

  enum { kOnScroll };

#if 0
  // SignalDelegate:
  void OnConnect(int identifier) override;
#endif

 private:
  scoped_refptr<NativeView> content_view_;
};

}  // namespace electron

#endif  // SHELL_BROWSER_UI_NATIVE_SCROLL_H_
