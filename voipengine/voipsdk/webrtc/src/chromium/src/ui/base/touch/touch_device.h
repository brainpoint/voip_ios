// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_TOUCH_TOUCH_DEVICE_H_
#define UI_BASE_TOUCH_TOUCH_DEVICE_H_

#include "ui/base/ui_base_export.h"

#if defined(OS_ANDROID)
#include <jni.h>
#endif


namespace ui {

// Returns true if a touch device is available.
UI_BASE_EXPORT bool IsTouchDevicePresent();

// Returns the maximum number of simultaneous touch contacts supported
// by the device. In the case of devices with multiple digitizers (e.g.
// multiple touchscreens), the value MUST be the maximum of the set of
// maximum supported contacts by each individual digitizer.
// For example, suppose a device has 3 touchscreens, which support 2, 5,
// and 10 simultaneous touch contacts, respectively. This returns 10.
// http://www.w3.org/TR/pointerevents/#widl-Navigator-maxTouchPoints
UI_BASE_EXPORT int MaxTouchPoints();

// Bit field values indicating available pointer types. Identical to
// blink::WebSettings::PointerType enums, enforced by compile-time assertions
// in content/public/common/web_preferences.cc .
enum PointerType {
  POINTER_TYPE_NONE = 1 << 0,
  POINTER_TYPE_FIRST = POINTER_TYPE_NONE,
  POINTER_TYPE_COARSE = 1 << 1,
  POINTER_TYPE_FINE = 1 << 2,
  POINTER_TYPE_LAST = POINTER_TYPE_FINE
};

// Bit field values indicating available hover types. Identical to
// blink::WebSettings::HoverType enums, enforced by compile-time assertions
// in content/public/common/web_preferences.cc .
enum HoverType {
  HOVER_TYPE_NONE = 1 << 0,
  HOVER_TYPE_FIRST = HOVER_TYPE_NONE,
  HOVER_TYPE_ON_DEMAND = 1 << 1,
  HOVER_TYPE_HOVER = 1 << 2,
  HOVER_TYPE_LAST = HOVER_TYPE_HOVER
};

#if defined(OS_ANDROID)
bool RegisterTouchDeviceAndroid(JNIEnv* env);
#endif

}  // namespace ui

#endif  // UI_BASE_TOUCH_TOUCH_DEVICE_H_
