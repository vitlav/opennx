// $Id: MxValidator.h 821 2004-08-08 00:32:08Z felfert $

#ifndef _MXVALIDATOR_H_
#define _MXVALIDATOR_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "MxValidator.h"
#endif

#include "wx/validate.h"

class SessionProperties;

class KeyTypeCallback
{
public:
    virtual void KeyTyped() = 0;
};

class MxValidator : public wxValidator
{
    DECLARE_DYNAMIC_CLASS(MxValidator)

public:

    typedef enum {
        MXVAL_ANY,
        MXVAL_NUMERIC,
        MXVAL_HOST,
        MXVAL_FILENAME,
    } MxValidatorType;


    MxValidator() { };
    MxValidator(bool*);
    MxValidator(int*);
    MxValidator(wxString*);
    MxValidator(wxArrayInt*);
    MxValidator(const MxValidator&);
    
    MxValidator(MxValidatorType, bool*);
    MxValidator(MxValidatorType, int*);
    MxValidator(MxValidatorType, wxString*);
    MxValidator(MxValidatorType, wxArrayInt*);
    MxValidator(MxValidatorType, const MxValidator&);
    
    virtual wxObject *Clone() const { return new MxValidator(*this); }
    bool Copy(const MxValidator& val);

    // Called when the value in the window must be validated.
    // This function can pop up an error message.
    virtual bool Validate(wxWindow * WXUNUSED(parent)) { return TRUE; }
    
    // Called to transfer data to the window
    virtual bool TransferToWindow();
    
    // Called to transfer data to the window
    virtual bool TransferFromWindow();
    
    void SetKeyTyped(KeyTypeCallback *);

    ~MxValidator();

    void OnChar(wxKeyEvent& event);

    DECLARE_EVENT_TABLE()

protected:
  void Init();

  bool*       m_pBool;
  int*        m_pInt;
  wxString*   m_pString;
  wxArrayInt* m_pArrayInt;
  KeyTypeCallback *m_ktc;

private:

    MxValidatorType m_type;
};

#endif
// _MXVALIDATOR_H_
