// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/touch/touch_device.h"

#include "base/android/context_utils.h"
#include "base/android/jni_array.h"
#include "base/logging.h"
#include "jni/TouchDevice_jni.h"

using base::android::AttachCurrentThread;
using base::android::GetApplicationContext;

namespace ui {

TouchScreensAvailability GetTouchScreensAvailability() {
  return TouchScreensAvailability::ENABLED;
}

int MaxTouchPoints() {
  return Java_TouchDevice_maxTouchPoints(AttachCurrentThread(),
                                         GetApplicationContext());
}

std::pair<int, int> GetAvailablePointerAndHoverTypes() {
  JNIEnv* env = AttachCurrentThread();
  std::vector<int> pointer_and_hover_types;
  base::android::JavaIntArrayToIntVector(
      env, Java_TouchDevice_availablePointerAndHoverTypes(
               env, GetApplicationContext())
               .obj(),
      &pointer_and_hover_types);
  DCHECK_EQ(pointer_and_hover_types.size(), 2u);
  return std::make_pair(pointer_and_hover_types[0], pointer_and_hover_types[1]);
}

PointerType GetPrimaryPointerType(int available_pointer_types) {
  if (available_pointer_types & POINTER_TYPE_COARSE)
    return POINTER_TYPE_COARSE;
  if (available_pointer_types & POINTER_TYPE_FINE)
    return POINTER_TYPE_FINE;
  DCHECK_EQ(available_pointer_types, POINTER_TYPE_NONE);
  return POINTER_TYPE_NONE;
}

HoverType GetPrimaryHoverType(int available_hover_types) {
  if (available_hover_types & HOVER_TYPE_ON_DEMAND)
    return HOVER_TYPE_ON_DEMAND;
  if (available_hover_types & HOVER_TYPE_HOVER)
    return HOVER_TYPE_HOVER;
  DCHECK_EQ(available_hover_types, HOVER_TYPE_NONE);
  return HOVER_TYPE_NONE;
}

}  // namespace ui