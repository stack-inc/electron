// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_NATIVE_BROWSER_VIEW_MAC_H_
#define SHELL_BROWSER_NATIVE_BROWSER_VIEW_MAC_H_

#import <Cocoa/Cocoa.h>
#include <vector>

#include "base/mac/scoped_nsobject.h"
#include "shell/browser/native_browser_view.h"

namespace electron {

class NativeBrowserViewMac : public NativeBrowserView {
 public:
  explicit NativeBrowserViewMac(
      InspectableWebContents* inspectable_web_contents);
  ~NativeBrowserViewMac() override;

  void SetAutoResizeFlags(uint8_t flags) override;
  void SetBounds(const gfx::Rect& bounds) override;
  gfx::Rect GetBounds() override;
  gfx::Rect GetLastBounds() override;
  void SetBackgroundColor(SkColor color) override;

  void SetZIndex(int z_index) override;
  int GetZIndex() override;

  void SetRoundedCorners(const RoundedCornersOptions& options) override;
  void SetClippingInsets(const ClippingInsetOptions& options) override;

  void UpdateDraggableRegions(
      const std::vector<mojom::DraggableRegionPtr>& regions) override;

  void UpdateDraggableRegions(
      const std::vector<gfx::Rect>& drag_exclude_rects) override;

  gfx::Rect last_bounds_;

  DISALLOW_COPY_AND_ASSIGN(NativeBrowserViewMac);
};

}  // namespace electron

#endif  // SHELL_BROWSER_NATIVE_BROWSER_VIEW_MAC_H_
