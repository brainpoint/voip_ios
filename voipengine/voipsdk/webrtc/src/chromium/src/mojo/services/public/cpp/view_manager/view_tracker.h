// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SERVICES_PUBLIC_CPP_VIEW_MANAGER_VIEW_TRACKER_H_
#define MOJO_SERVICES_PUBLIC_CPP_VIEW_MANAGER_VIEW_TRACKER_H_

#include <set>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "mojo/services/public/cpp/view_manager/view_observer.h"

namespace mojo {

class ViewTracker : public ViewObserver {
 public:
  using Views = std::set<View*>;

  ViewTracker();
  ~ViewTracker() override;

  // Returns the set of views being observed.
  const std::set<View*>& views() const { return views_; }

  // Adds |view| to the set of Views being tracked.
  void Add(View* view);

  // Removes |view| from the set of views being tracked.
  void Remove(View* view);

  // Returns true if |view| was previously added and has not been removed or
  // deleted.
  bool Contains(View* view);

  // ViewObserver overrides:
  virtual void OnViewDestroying(View* view) override;

 private:
  Views views_;

  DISALLOW_COPY_AND_ASSIGN(ViewTracker);
};

}  // namespace mojo

#endif  // MOJO_SERVICES_PUBLIC_CPP_VIEW_MANAGER_VIEW_TRACKER_H_
