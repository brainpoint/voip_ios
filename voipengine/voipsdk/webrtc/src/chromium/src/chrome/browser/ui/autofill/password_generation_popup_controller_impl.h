// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_PASSWORD_GENERATION_POPUP_CONTROLLER_IMPL_H_
#define CHROME_BROWSER_UI_AUTOFILL_PASSWORD_GENERATION_POPUP_CONTROLLER_IMPL_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/autofill/password_generation_popup_controller.h"
#include "chrome/browser/ui/autofill/popup_controller_common.h"
#include "components/autofill/core/common/password_form.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/range/range.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/rect_f.h"

namespace content {
struct NativeWebKeyboardEvent;
class WebContents;
}

namespace password_manager {
class PasswordManager;
}

namespace autofill {

class PasswordGenerator;
class PasswordGenerationPopupObserver;
class PasswordGenerationPopupView;

// This class controls a PasswordGenerationPopupView. It is responsible for
// determining the location of the popup, handling keypress events while the
// popup is active, and notifying both the renderer and the password manager
// if the password is accepted.
class PasswordGenerationPopupControllerImpl
    : public PasswordGenerationPopupController {
 public:
  // Create a controller or return |previous| if it is suitable. Will hide
  // |previous| if it is not returned. |bounds| is the bounds of the element
  // that we are showing the dropdown for in screen space. |form| is the
  // identifier for the form that we are filling, and is used to notify
  // |password_manager| if the password is generated. |max_length| is used to
  // determine the length of the password shown. If not NULL, |observer| will
  // be notified of changes of the popup state.
  static base::WeakPtr<PasswordGenerationPopupControllerImpl> GetOrCreate(
      base::WeakPtr<PasswordGenerationPopupControllerImpl> previous,
      const gfx::RectF& bounds,
      const PasswordForm& form,
      int max_length,
      password_manager::PasswordManager* password_manager,
      PasswordGenerationPopupObserver* observer,
      content::WebContents* web_contents,
      gfx::NativeView container_view);
  ~PasswordGenerationPopupControllerImpl() override;

  // Create a PasswordGenerationPopupView if one doesn't already exist.
  // If |display_password| is true, a generated password is shown that can be
  // selected by the user. Otherwise just the text explaining generated
  // passwords is shown. Idempotent.
  void Show(bool display_password);

  // Hides the popup and destroys |this|.
  void HideAndDestroy();

  // Accessors.
  content::WebContents* web_contents() {
    return controller_common_.web_contents();
  }

 protected:
  PasswordGenerationPopupControllerImpl(
      const gfx::RectF& bounds,
      const PasswordForm& form,
      int max_length,
      password_manager::PasswordManager* password_manager,
      PasswordGenerationPopupObserver* observer,
      content::WebContents* web_contents,
      gfx::NativeView container_view);

  // Handle to the popup. May be NULL if popup isn't showing.
  PasswordGenerationPopupView* view_;

 private:
  // PasswordGenerationPopupController implementation:
  void Hide() override;
  void ViewDestroyed() override;
  void SetSelectionAtPoint(const gfx::Point& point) override;
  bool AcceptSelectedLine() override;
  void SelectionCleared() override;
  void PasswordAccepted() override;
  void OnSavedPasswordsLinkClicked() override;
  int GetMinimumWidth() override;
  gfx::NativeView container_view() override;
  const gfx::Rect& popup_bounds() const override;
  const gfx::RectF& element_bounds() const override;
  bool IsRTL() const override;
  bool display_password() const override;
  bool password_selected() const override;
  base::string16 password() const override;
  base::string16 SuggestedText() override;
  const base::string16& HelpText() override;
  const gfx::Range& HelpTextLinkRange() override;

  base::WeakPtr<PasswordGenerationPopupControllerImpl> GetWeakPtr();

  bool HandleKeyPressEvent(const content::NativeWebKeyboardEvent& event);

  // Set if the password is currently selected.
  void PasswordSelected(bool selected);

  // Accept password if it's selected.
  bool PossiblyAcceptPassword();

  // Get desired size of popup. Height depends on width because we do text
  // wrapping.
  void CalculateBounds();

  PasswordForm form_;
  password_manager::PasswordManager* password_manager_;

  // May be NULL.
  PasswordGenerationPopupObserver* observer_;

  // Controls how passwords are generated.
  scoped_ptr<PasswordGenerator> generator_;

  // Contains common popup functionality.
  PopupControllerCommon controller_common_;

  // Help text and the range in the text that corresponds to the saved passwords
  // link.
  base::string16 help_text_;
  gfx::Range link_range_;

  base::string16 current_password_;
  bool password_selected_;

  // If a password will be shown in this popup.
  bool display_password_;

  // Bounds for all the elements of the popup.
  gfx::Rect popup_bounds_;

  base::WeakPtrFactory<PasswordGenerationPopupControllerImpl> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(PasswordGenerationPopupControllerImpl);
};

}  // namespace autofill

#endif  // CHROME_BROWSER_UI_AUTOFILL_PASSWORD_GENERATION_POPUP_CONTROLLER_IMPL_H_
