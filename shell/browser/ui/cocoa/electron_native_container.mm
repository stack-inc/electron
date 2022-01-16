// Copyright (c) 2022 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/cocoa/electron_native_container.h"

#include "base/mac/scoped_cftyperef.h"
#include "skia/ext/skia_utils_mac.h"
#include "ui/gfx/geometry/rect_f.h"

typedef struct CGContext* CGContextRef;

@implementation ElectronNativeContainer

- (electron::NativeViewPrivate*)nativeViewPrivate {
  return &private_;
}

- (void)setNativeBackgroundColor:(SkColor)color {
  background_color_ = color;
  [self setNeedsDisplay:YES];
}

- (BOOL)isFlipped {
  return YES;
}

- (void)drawRect:(NSRect)dirtyRect {
  electron::NativeContainer* shell =
      static_cast<electron::NativeContainer*>([self shell]);
  if (!shell)
    return;

  gfx::RectF dirty(dirtyRect);
  CGContextRef context = reinterpret_cast<CGContextRef>(
      [[NSGraphicsContext currentContext] graphicsPort]);
  CGContextSetRGBStrokeColor(context, SkColorGetR(background_color_) / 255.0f,
                             SkColorGetG(background_color_) / 255.0f,
                             SkColorGetB(background_color_) / 255.0f,
                             SkColorGetA(background_color_) / 255.0f);
  CGContextSetRGBFillColor(context, SkColorGetR(background_color_) / 255.0f,
                           SkColorGetG(background_color_) / 255.0f,
                           SkColorGetB(background_color_) / 255.0f,
                           SkColorGetA(background_color_) / 255.0f);
  CGContextFillRect(context, dirty.ToCGRect());
}

@end
