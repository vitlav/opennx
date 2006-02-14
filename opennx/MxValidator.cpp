// $Id: MxValidator.cpp 823 2004-08-08 03:25:27Z felfert $

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "MxValidator.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#ifndef WX_PRECOMP
  #include "wx/defs.h"
#endif
#include <wx/config.h>
#include <wx/fontdlg.h>

#ifndef WX_PRECOMP
  #include "wx/utils.h"
  #include "wx/intl.h"
  #include "wx/dynarray.h"
  #include "wx/choice.h"
  #include "wx/combobox.h"
  #include "wx/radiobox.h"
  #include "wx/radiobut.h"
  #include "wx/checkbox.h"
  #include "wx/scrolbar.h"
  #include "wx/gauge.h"
  #include "wx/stattext.h"
  #include "wx/textctrl.h"
  #include "wx/button.h"
  #include "wx/listbox.h"
  #include "wx/slider.h"
#endif
 
  #include "wx/spinctrl.h"

#if wxUSE_SPINBTN 
  #include "wx/spinbutt.h"
#endif
#if wxUSE_CHECKLISTBOX 
  #include "wx/checklst.h"
#endif

#include "MxValidator.h"
#include "SessionProperties.h"

IMPLEMENT_DYNAMIC_CLASS(MxValidator, wxValidator)

BEGIN_EVENT_TABLE(MxValidator, wxValidator)
    EVT_CHAR(MxValidator::OnChar)
END_EVENT_TABLE()

MxValidator::~MxValidator()
{
}

MxValidator::MxValidator(MxValidatorType t, const MxValidator &v)
    : wxValidator()
    , m_type(t)
{
    Copy(v);
}

MxValidator::MxValidator(MxValidatorType t, bool *val)
    : wxValidator()
    , m_type(t)
{
    Init();
    m_pBool = val;
}

MxValidator::MxValidator(MxValidatorType t, int *val)
    : wxValidator()
    , m_type(t)
{
    Init();
    m_pInt = val;
}

MxValidator::MxValidator(MxValidatorType t, wxString *val)
    : wxValidator()
    , m_type(t)
{
    Init();
    m_pString = val;
}

MxValidator::MxValidator(MxValidatorType t, wxArrayInt *val)
    : wxValidator()
    , m_type(t)
{
    Init();
    m_pArrayInt = val;
}

MxValidator::MxValidator(const MxValidator &v)
    : wxValidator()
    , m_type(v.m_type)
{
    Copy(v);
}

MxValidator::MxValidator(bool *val)
    : wxValidator()
    , m_type(MXVAL_ANY)
{
    Init();
    m_pBool = val;
}

MxValidator::MxValidator(int *val)
    : wxValidator()
    , m_type(MXVAL_ANY)
{
    Init();
    m_pInt = val;
}

MxValidator::MxValidator(wxString *val)
    : wxValidator()
    , m_type(MXVAL_ANY)
{
    Init();
    m_pString = val;
}

MxValidator::MxValidator(wxArrayInt *val)
    : wxValidator()
    , m_type(MXVAL_ANY)
{
    Init();
    m_pArrayInt = val;
}

void MxValidator::Init()
{
    m_pBool = NULL;
    m_pInt = NULL;
    m_pString = NULL;
    m_pArrayInt = NULL;
    m_ktc = NULL;
}

bool MxValidator::Copy(const MxValidator& v)
{
    wxValidator::Copy(v);

    m_pBool = v.m_pBool;
    m_pInt = v.m_pInt;
    m_pString = v.m_pString;
    m_pArrayInt = v.m_pArrayInt;
    m_ktc = v.m_ktc;

    return TRUE;
}

bool MxValidator::TransferToWindow(void)
{
    if ( !m_validatorWindow )
        return FALSE;

    // bool controls
#if wxUSE_CHECKBOX
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxCheckBox)) )
    {
        wxCheckBox* pControl = (wxCheckBox*) m_validatorWindow;
        if (m_pBool)
        {
            pControl->SetValue(*m_pBool);
            return TRUE;
        }
    } else
#endif
#if wxUSE_RADIOBTN
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxRadioButton)) )
    {
        wxRadioButton* pControl = (wxRadioButton*) m_validatorWindow;
        if (m_pBool)
        {
            pControl->SetValue(*m_pBool) ;
            return TRUE;
        }
    } else
#endif

    // int controls
#if wxUSE_GAUGE
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxGauge)) )
    {
        wxGauge* pControl = (wxGauge*) m_validatorWindow;
        if (m_pInt)
        {
            pControl->SetValue(*m_pInt);
            return TRUE;
        }
    } else
#endif
#if wxUSE_RADIOBOX
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxRadioBox)) )
    {
        wxRadioBox* pControl = (wxRadioBox*) m_validatorWindow;
        if (m_pInt)
        {
            pControl->SetSelection(*m_pInt) ;
            return TRUE;
        }
    } else
#endif
#if wxUSE_SCROLLBAR
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxScrollBar)) )
    {
        wxScrollBar* pControl = (wxScrollBar*) m_validatorWindow;
        if (m_pInt)
        {
            pControl->SetThumbPosition(*m_pInt) ;
            return TRUE;
        }
    } else
#endif
#if wxUSE_SPINCTRL && !defined(__WXMOTIF__)
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxSpinCtrl)) )
    {
        wxSpinCtrl* pControl = (wxSpinCtrl*) m_validatorWindow;
        if (m_pInt)
        {
            pControl->SetValue(*m_pInt);
            return TRUE;
        }
    } else
#endif
#if wxUSE_SPINBTN
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxSpinButton)) )
    {
        wxSpinButton* pControl = (wxSpinButton*) m_validatorWindow;
        if (m_pInt)
        {
            pControl->SetValue(*m_pInt) ;
            return TRUE;
        }
    } else
#endif
#if wxUSE_SLIDER
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxSlider)) )
    {
        wxSlider* pControl = (wxSlider*) m_validatorWindow;
        if (m_pInt)
        {
            pControl->SetValue(*m_pInt) ;
            return TRUE;
        }
    } else
#endif

    // string controls
#if wxUSE_BUTTON
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxButton)) )
    {
        wxButton* pControl = (wxButton*) m_validatorWindow;
        if (m_pString)
        {
            pControl->SetLabel(*m_pString) ;
            return TRUE;
        }
    } else
#endif
#if wxUSE_COMBOBOX
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxComboBox)) )
    {
        wxComboBox* pControl = (wxComboBox*) m_validatorWindow;
        if (m_pInt)
        {
            pControl->SetSelection(*m_pInt) ;
            return TRUE;
        }
        else if (m_pString)
        {
            if (pControl->FindString(* m_pString) > -1)
            {
                pControl->SetStringSelection(* m_pString);
            }
            else
            {
                pControl->SetValue(* m_pString);
            }
            return TRUE;
        }
    } else
#endif
#if wxUSE_CHOICE
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxChoice)) )
    {
        wxChoice* pControl = (wxChoice*) m_validatorWindow;
        if (m_pInt)
        {
            pControl->SetSelection(*m_pInt) ;
            return TRUE;
        }
        else if (m_pString)
        {
            if (pControl->FindString(* m_pString) > -1)
            {
                pControl->SetStringSelection(* m_pString);
            }
            return TRUE;
        }
    } else
#endif
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxStaticText)) )
    {
        wxStaticText* pControl = (wxStaticText*) m_validatorWindow;
        if (m_pString)
        {
            pControl->SetLabel(*m_pString) ;
            return TRUE;
        }
    } else
#if wxUSE_TEXTCTRL
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxTextCtrl)) )
    {
        wxTextCtrl* pControl = (wxTextCtrl*) m_validatorWindow;
        if (m_pString)
        {
            pControl->SetValue(*m_pString) ;
            return TRUE;
        }
        else if (m_pInt)
        {
            wxString str;
            str.Printf(wxT("%d"), *m_pInt);
            pControl->SetValue(str);
            return TRUE;
        }
    } else
#endif
    // array controls
#if wxUSE_CHECKLISTBOX
    // NOTE: wxCheckListBox is a wxListBox, so wxCheckListBox MUST come first:
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxCheckListBox)) )
    {
        wxCheckListBox* pControl = (wxCheckListBox*) m_validatorWindow;
        if (m_pArrayInt)
        {
            // clear all selections
            size_t i,
                   count = pControl->GetCount();
            for ( i = 0 ; i < count; i++ )
                pControl->Check(i, FALSE);

            // select each item in our array
            count = m_pArrayInt->GetCount();
            for ( i = 0 ; i < count; i++ )
                pControl->Check(m_pArrayInt->Item(i));

            return TRUE;
        }
        else
            return FALSE;
    } else
#endif
#if wxUSE_LISTBOX
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxListBox)) )
    {
        wxListBox* pControl = (wxListBox*) m_validatorWindow;
        if (m_pArrayInt)
        {
            // clear all selections
            size_t i,
                   count = pControl->GetCount();
            for ( i = 0 ; i < count; i++ )
                pControl->Deselect(i);

            // select each item in our array
            count = m_pArrayInt->GetCount();
            for ( i = 0 ; i < count; i++ )
                pControl->SetSelection(m_pArrayInt->Item(i));

            return TRUE;
        }
    } else
#endif
        ;   // to match the last 'else' above

  // unrecognized control, or bad pointer
  return FALSE;
}

bool MxValidator::TransferFromWindow(void)
{
  if ( !m_validatorWindow )
    return FALSE;

  // bool controls
#if wxUSE_CHECKBOX
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxCheckBox)) )
  {
    wxCheckBox* pControl = (wxCheckBox*) m_validatorWindow;
        if (m_pBool)
    {
      *m_pBool = pControl->GetValue() ;
      return TRUE;
    }
  } else
#endif
#if wxUSE_RADIOBTN
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxRadioButton)) )
  {
    wxRadioButton* pControl = (wxRadioButton*) m_validatorWindow;
        if (m_pBool)
    {
      *m_pBool = pControl->GetValue() ;
      return TRUE;
    }
  } else
#endif
  // int controls
#if wxUSE_GAUGE
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxGauge)) )
  {
    wxGauge* pControl = (wxGauge*) m_validatorWindow;
        if (m_pInt)
    {
      *m_pInt = pControl->GetValue() ;
      return TRUE;
    }
  } else
#endif
#if wxUSE_RADIOBOX
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxRadioBox)) )
  {
    wxRadioBox* pControl = (wxRadioBox*) m_validatorWindow;
        if (m_pInt)
    {
      *m_pInt = pControl->GetSelection() ;
      return TRUE;
    }
  } else
#endif
#if wxUSE_SCROLLBAR
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxScrollBar)) )
  {
    wxScrollBar* pControl = (wxScrollBar*) m_validatorWindow;
        if (m_pInt)
    {
      *m_pInt = pControl->GetThumbPosition() ;
      return TRUE;
    }
  } else
#endif
#if wxUSE_SPINCTRL && !defined(__WXMOTIF__)
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxSpinCtrl)) )
    {
        wxSpinCtrl* pControl = (wxSpinCtrl*) m_validatorWindow;
        if (m_pInt)
        {
            *m_pInt=pControl->GetValue();
            return TRUE;
        }
    } else
#endif
#if wxUSE_SPINBTN
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxSpinButton)) )
  {
    wxSpinButton* pControl = (wxSpinButton*) m_validatorWindow;
        if (m_pInt)
    {
      *m_pInt = pControl->GetValue() ;
      return TRUE;
    }
  } else
#endif
#if wxUSE_SLIDER
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxSlider)) )
  {
    wxSlider* pControl = (wxSlider*) m_validatorWindow;
    if (m_pInt)
    {
      *m_pInt = pControl->GetValue() ;
      return TRUE;
    }
  } else
#endif
  // string controls
#if wxUSE_BUTTON
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxButton)) )
  {
    wxButton* pControl = (wxButton*) m_validatorWindow;
        if (m_pString)
    {
      *m_pString = pControl->GetLabel() ;
      return TRUE;
    }
  } else
#endif
#if wxUSE_COMBOBOX
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxComboBox)) )
  {
    wxComboBox* pControl = (wxComboBox*) m_validatorWindow;
    if (m_pInt)
    {
      *m_pInt = pControl->GetSelection() ;
      return TRUE;
    }
    else if (m_pString)
    {
        *m_pString = pControl->GetValue();
        return TRUE;
    }
  } else
#endif
#if wxUSE_CHOICE
 if (m_validatorWindow->IsKindOf(CLASSINFO(wxChoice)) )
  {
    wxChoice* pControl = (wxChoice*) m_validatorWindow;
        if (m_pInt)
    {
      *m_pInt = pControl->GetSelection() ;
      return TRUE;
    }
    else if (m_pString)
    {
        *m_pString = pControl->GetStringSelection();
        return TRUE;
    }
  } else
#endif
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxStaticText)) )
  {
    wxStaticText* pControl = (wxStaticText*) m_validatorWindow;
        if (m_pString)
    {
      *m_pString = pControl->GetLabel() ;
      return TRUE;
    }
  } else
#if wxUSE_TEXTCTRL
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxTextCtrl)) )
  {
    wxTextCtrl* pControl = (wxTextCtrl*) m_validatorWindow;
        if (m_pString)
    {
      *m_pString = pControl->GetValue() ;
      return TRUE;
    }
    else if (m_pInt)
    {
        *m_pInt = wxAtoi(pControl->GetValue());
        return TRUE;
    }
  } else
#endif
  // array controls
#if wxUSE_CHECKLISTBOX
  // NOTE: wxCheckListBox isa wxListBox, so wxCheckListBox MUST come first:
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxCheckListBox)) )
  {
    wxCheckListBox* pControl = (wxCheckListBox*) m_validatorWindow;
    if (m_pArrayInt)
    {
      // clear our array
      m_pArrayInt->Clear();

      // add each selected item to our array
      size_t i,
             count = pControl->GetCount();
      for ( i = 0; i < count; i++ )
      {
        if (pControl->IsChecked(i))
          m_pArrayInt->Add(i);
      }

      return TRUE;
    }
    else
      return FALSE;
  } else
#endif
#if wxUSE_LISTBOX
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxListBox)) )
  {
    wxListBox* pControl = (wxListBox*) m_validatorWindow;
    if (m_pArrayInt)
    {
      // clear our array
      m_pArrayInt->Clear();

      // add each selected item to our array
      size_t i,
             count = pControl->GetCount();
      for ( i = 0; i < count; i++ )
      {
        if (pControl->Selected(i))
          m_pArrayInt->Add(i);
      }

      return TRUE;
    }
  } else
#endif

  // unrecognized control, or bad pointer
    return FALSE;
  return FALSE;
}

void MxValidator::SetKeyTyped(KeyTypeCallback *cb)
{
    m_ktc = cb;
}

void MxValidator::OnChar(wxKeyEvent& event)
{
    if (m_validatorWindow) {
        int keyCode = event.GetKeyCode();

        // we don't filter special keys and Delete
        if (!(keyCode < WXK_SPACE || keyCode == WXK_DELETE || keyCode > WXK_START)) {
            bool chOk = true;
            switch (m_type) {
                case MXVAL_NUMERIC:
                    chOk = (wxIsdigit(keyCode) != 0);
                    break;
                case MXVAL_HOST:
                    chOk = (wxIsalnum(keyCode) || (keyCode == _T('.')) || (keyCode == _T('-')));
                    break;
                case MXVAL_FILENAME:
                    chOk = (wxFileName::GetForbiddenChars().Find(keyCode) == -1);
                    break;
            }
            if (!chOk) {
                if (!wxValidator::IsSilent())
                    wxBell();
                // eat message
                return;
            }
        }
    }

    if (m_ktc)
        m_ktc->KeyTyped();
    event.Skip();
}
