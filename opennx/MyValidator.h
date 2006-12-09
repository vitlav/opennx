// $Id$

#ifndef _MYVALIDATOR_H_
#define _MYVALIDATOR_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "MyValidator.h"
#endif

#include "wx/validate.h"

class KeyTypeCallback
{
public:
    virtual void KeyTyped() = 0;
};

class MyValidator : public wxValidator
{
    DECLARE_DYNAMIC_CLASS(MyValidator)

public:

    typedef enum {
        MYVAL_ANY,
        MYVAL_NUMERIC,
        MYVAL_HOST,
        MYVAL_FILENAME,
    } MyValidatorType;


    MyValidator() { };
    MyValidator(bool*);
    MyValidator(int*);
    MyValidator(wxString*);
    MyValidator(wxArrayInt*);
    MyValidator(const MyValidator&);
    
    MyValidator(MyValidatorType, bool*);
    MyValidator(MyValidatorType, int*);
    MyValidator(MyValidatorType, wxString*);
    MyValidator(MyValidatorType, wxArrayInt*);
    MyValidator(MyValidatorType, const MyValidator&);
    
    virtual wxObject *Clone() const { return new MyValidator(*this); }
    bool Copy(const MyValidator& val);

    // Called when the value in the window must be validated.
    // This function can pop up an error message.
    virtual bool Validate(wxWindow * WXUNUSED(parent)) { return TRUE; }
    
    // Called to transfer data to the window
    virtual bool TransferToWindow();
    
    // Called to transfer data to the window
    virtual bool TransferFromWindow();
    
    void SetKeyTyped(KeyTypeCallback *);

    ~MyValidator();

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

    MyValidatorType m_type;
};

#endif
// _MYVALIDATOR_H_
