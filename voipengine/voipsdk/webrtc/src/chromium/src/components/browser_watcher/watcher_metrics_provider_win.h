// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_BROWSER_WATCHER_WATCHER_METRICS_PROVIDER_WIN_H_
#define COMPONENTS_BROWSER_WATCHER_WATCHER_METRICS_PROVIDER_WIN_H_

#include "base/macros.h"
#include "base/strings/string16.h"
#include "components/metrics/metrics_provider.h"

namespace browser_watcher {

// Provides stability data captured by the Chrome Watcher, namely the browser
// process exit codes.
class WatcherMetricsProviderWin : public metrics::MetricsProvider {
 public:
  static const char kBrowserExitCodeHistogramName[];

  explicit WatcherMetricsProviderWin(const base::char16* registry_path);
  ~WatcherMetricsProviderWin();

  // metrics::MetricsProvider implementation.
  virtual void ProvideStabilityMetrics(
      metrics::SystemProfileProto* system_profile_proto) override;

 private:
  base::string16 registry_path_;

  DISALLOW_COPY_AND_ASSIGN(WatcherMetricsProviderWin);
};

}  // namespace browser_watcher

#endif  // COMPONENTS_BROWSER_WATCHER_WATCHER_METRICS_PROVIDER_WIN_H_
