// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DEVTOOLS_DEVTOOLS_FRONTEND_HOST_IMPL_H_
#define CONTENT_BROWSER_DEVTOOLS_DEVTOOLS_FRONTEND_HOST_IMPL_H_

#include "content/public/browser/devtools_frontend_host.h"
#include "content/public/browser/web_contents_observer.h"

namespace content {

class DevToolsFrontendHostImpl : public DevToolsFrontendHost,
                                 public WebContentsObserver {
 public:
  DevToolsFrontendHostImpl(WebContents* frontend_web_contents,
                           DevToolsFrontendHost::Delegate* delegate);
  ~DevToolsFrontendHostImpl() override;

 private:
  // WebContentsObserver overrides.
  bool OnMessageReceived(const IPC::Message& message,
                         RenderFrameHost* render_frame_host) override;

  void OnDispatchOnInspectorBackend(const std::string& message);
  void OnDispatchOnEmbedder(const std::string& message);

  DevToolsFrontendHost::Delegate* delegate_;
  DISALLOW_COPY_AND_ASSIGN(DevToolsFrontendHostImpl);
};

}  // namespace content

#endif  // CONTENT_BROWSER_DEVTOOLS_DEVTOOLS_FRONTEND_HOST_IMPL_H_
