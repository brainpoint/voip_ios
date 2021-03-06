// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_SIGNIN_CORE_BROWSER_TEST_SIGNIN_CLIENT_H_
#define COMPONENTS_SIGNIN_CORE_BROWSER_TEST_SIGNIN_CLIENT_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/scoped_temp_dir.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "components/signin/core/browser/signin_client.h"
#include "net/url_request/url_request_test_util.h"

#if defined(OS_IOS)
#include "ios/public/test/fake_profile_oauth2_token_service_ios_provider.h"
#endif

class PrefService;

// An implementation of SigninClient for use in unittests. Instantiates test
// versions of the various objects that SigninClient is required to provide as
// part of its interface.
class TestSigninClient : public SigninClient {
 public:
  TestSigninClient();
  TestSigninClient(PrefService* pref_service);
  ~TestSigninClient() override;

  // SigninClient implementation that is specialized for unit tests.

  // Returns NULL.
  // NOTE: This should be changed to return a properly-initalized PrefService
  // once there is a unit test that requires it.
  PrefService* GetPrefs() override;

  // Returns a pointer to a loaded database.
  scoped_refptr<TokenWebData> GetDatabase() override;

  // Returns true.
  bool CanRevokeCredentials() override;

  // Returns empty string.
  std::string GetSigninScopedDeviceId() override;

  // Does nothing.
  void OnSignedOut() override;

  // Returns the empty string.
  std::string GetProductVersion() override;

  // Returns a TestURLRequestContextGetter or an manually provided
  // URLRequestContextGetter.
  net::URLRequestContextGetter* GetURLRequestContext() override;

  // For testing purposes, can override the TestURLRequestContextGetter created
  // in the default constructor.
  void SetURLRequestContext(net::URLRequestContextGetter* request_context);

#if defined(OS_IOS)
  virtual ios::ProfileOAuth2TokenServiceIOSProvider* GetIOSProvider() override;
#endif

  // Returns true.
  bool ShouldMergeSigninCredentialsIntoCookieJar() override;

  // Registers |callback| and returns the subscription.
  // Note that |callback| will never be called.
  scoped_ptr<SigninClient::CookieChangedSubscription> AddCookieChangedCallback(
      const GURL& url,
      const std::string& name,
      const net::CookieStore::CookieChangedCallback& callback) override;

#if defined(OS_IOS)
  ios::FakeProfileOAuth2TokenServiceIOSProvider* GetIOSProviderAsFake();
#endif

  // SigninClient overrides:
  void SetSigninProcess(int host_id) override;
  void ClearSigninProcess() override;
  bool IsSigninProcess(int host_id) const override;
  bool HasSigninProcess() const override;
  bool IsFirstRun() const override;
  base::Time GetInstallDate() override;

 private:
  // Loads the token database.
  void LoadDatabase();

  base::ScopedTempDir temp_dir_;
  scoped_refptr<net::URLRequestContextGetter> request_context_;
  scoped_refptr<TokenWebData> database_;
  int signin_host_id_;
  PrefService* pref_service_;

#if defined(OS_IOS)
  scoped_ptr<ios::FakeProfileOAuth2TokenServiceIOSProvider> iosProvider_;
#endif

  DISALLOW_COPY_AND_ASSIGN(TestSigninClient);
};

#endif  // COMPONENTS_SIGNIN_CORE_BROWSER_TEST_SIGNIN_CLIENT_H_
