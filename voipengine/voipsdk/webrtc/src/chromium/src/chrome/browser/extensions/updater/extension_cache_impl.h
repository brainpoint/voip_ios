// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_UPDATER_EXTENSION_CACHE_IMPL_H_
#define CHROME_BROWSER_EXTENSIONS_UPDATER_EXTENSION_CACHE_IMPL_H_

#include <set>
#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/browser/updater/extension_cache.h"

template <typename T> struct DefaultSingletonTraits;

namespace extensions {

class LocalExtensionCache;

// Singleton call that caches extensions .crx files to share them between
// multiple users and profiles on the machine.
class ExtensionCacheImpl : public ExtensionCache,
                           public content::NotificationObserver {
 public:
  ExtensionCacheImpl();
  virtual ~ExtensionCacheImpl();

  // Implementation of ExtensionCache.
  virtual void Start(const base::Closure& callback) override;
  virtual void Shutdown(const base::Closure& callback) override;
  virtual void AllowCaching(const std::string& id) override;
  virtual bool GetExtension(const std::string& id,
                            base::FilePath* file_path,
                            std::string* version) override;
  virtual void PutExtension(const std::string& id,
                            const base::FilePath& file_path,
                            const std::string& version,
                            const PutExtensionCallback& callback) override;

  // Implementation of content::NotificationObserver:
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) override;

 private:
  // Callback that is called when local cache is ready.
  void OnCacheInitialized();

  // Cache implementation that uses local cache dir.
  scoped_ptr<LocalExtensionCache> cache_;

  // Set of extensions that can be cached.
  std::set<std::string> allowed_extensions_;

  // List of callbacks that should be called when the cache is ready.
  std::vector<base::Closure> init_callbacks_;

  // Observes failures to install CRX files.
  content::NotificationRegistrar notification_registrar_;

  // Weak factory for callbacks.
  base::WeakPtrFactory<ExtensionCacheImpl> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionCacheImpl);
};

}  // namespace extensions

#endif  // CHROME_BROWSER_EXTENSIONS_UPDATER_EXTENSION_CACHE_IMPL_H_
