// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_API_SOCKETS_SOCKETS_MANIFEST_PERMISSION_H_
#define EXTENSIONS_COMMON_API_SOCKETS_SOCKETS_MANIFEST_PERMISSION_H_

#include <set>
#include <vector>

#include "extensions/common/install_warning.h"
#include "extensions/common/permissions/manifest_permission.h"
#include "extensions/common/permissions/socket_permission_entry.h"

namespace content {
struct SocketPermissionRequest;
}

namespace extensions {
class Extension;
}

namespace extensions {

class SocketsManifestPermission : public ManifestPermission {
 public:
  typedef std::set<SocketPermissionEntry> SocketPermissionEntrySet;
  SocketsManifestPermission();
  ~SocketsManifestPermission() override;

  // Tries to construct the info based on |value|, as it would have appeared in
  // the manifest. Sets |error| and returns an empty scoped_ptr on failure.
  static scoped_ptr<SocketsManifestPermission> FromValue(
      const base::Value& value,
      base::string16* error);

  bool CheckRequest(const Extension* extension,
                    const content::SocketPermissionRequest& request) const;

  void AddPermission(const SocketPermissionEntry& entry);

  // extensions::ManifestPermission overrides.
  std::string name() const override;
  std::string id() const override;
  bool HasMessages() const override;
  PermissionMessages GetMessages() const override;
  bool FromValue(const base::Value* value) override;
  scoped_ptr<base::Value> ToValue() const override;
  ManifestPermission* Diff(const ManifestPermission* rhs) const override;
  ManifestPermission* Union(const ManifestPermission* rhs) const override;
  ManifestPermission* Intersect(const ManifestPermission* rhs) const override;

  const SocketPermissionEntrySet& entries() const { return permissions_; }

 private:
  bool AddAnyHostMessage(PermissionMessages& messages) const;
  void AddSubdomainHostMessage(PermissionMessages& messages) const;
  void AddSpecificHostMessage(PermissionMessages& messages) const;
  void AddNetworkListMessage(PermissionMessages& messages) const;

  SocketPermissionEntrySet permissions_;
};

}  // namespace extensions

#endif  // EXTENSIONS_COMMON_API_SOCKETS_SOCKETS_MANIFEST_PERMISSION_H_
