// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_COPRESENCE_HANDLERS_GCM_HANDLER_IMPL_H_
#define COMPONENTS_COPRESENCE_HANDLERS_GCM_HANDLER_IMPL_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "components/copresence/handlers/gcm_handler.h"
#include "components/gcm_driver/gcm_app_handler.h"
#include "components/gcm_driver/gcm_client.h"

namespace gcm {
class GCMDriver;
}

namespace copresence {

class DirectiveHandler;

// This class handles GCM messages from the Copresence server.
class GCMHandlerImpl final : public GCMHandler,
                             public gcm::GCMAppHandler,
                             public base::SupportsWeakPtr<GCMHandlerImpl> {
 public:
  // Callback to report that registration has completed.
  // Returns an empty ID if registration failed.
  using RegistrationCallback = base::Callback<void(const std::string&)>;

  static const char kCopresenceAppId[];
  static const char kCopresenceSenderId[];
  static const char kGcmMessageKey[];

  // |gcm_driver| is required, but may disappear if we get a ShutdownHandler()
  // call first. |directive_handler| must outlive us. The caller owns both.
  GCMHandlerImpl(gcm::GCMDriver* gcm_driver,
                 DirectiveHandler* directive_handler);
  ~GCMHandlerImpl() override;

  // GCMHandler override.
  void GetGcmId(const RegistrationCallback& callback) override;

 private:
  friend class GCMHandlerTest;

  // GCMAppHandler overrides
  void ShutdownHandler() override;
  void OnMessage(const std::string& app_id,
                 const gcm::GCMClient::IncomingMessage& message) override;
  void OnMessagesDeleted(const std::string& app_id) override;
  void OnSendError(
      const std::string& /* app_id */,
      const gcm::GCMClient::SendErrorDetails& /* send_error_details */)
      override;
  void OnSendAcknowledged(const std::string& /* app_id */,
                          const std::string& /* message_id */) override;
  bool CanHandle(const std::string& app_id) const override;

  // GCM Registration has finished. Notify clients as appropriate.
  void RegistrationComplete(const std::string& registration_id,
                            gcm::GCMClient::Result result);

  gcm::GCMDriver* driver_;
  DirectiveHandler* const directive_handler_;

  // TODO(ckehoe): Change this to a CancelableCallback.
  base::Callback<void(const std::string&,
                      gcm::GCMClient::Result)> registration_callback_;

  std::string gcm_id_;
  std::vector<RegistrationCallback> pending_id_requests_;

  DISALLOW_COPY_AND_ASSIGN(GCMHandlerImpl);
};

}  // namespace copresence

#endif  // COMPONENTS_COPRESENCE_HANDLERS_GCM_HANDLER_IMPL_H_
