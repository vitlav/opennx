// $Id: WinShare.h 795 2004-08-03 18:30:08Z felfert $
#ifndef _WINSHARE_H_
#define _WINSHARE_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "WinShare.h"
#endif

#include <wx/dynarray.h>
#include <wx/string.h>
#include <wx/object.h>

class DllData;

class SharedResource : public wxObject
{
public:
    typedef enum {
        SHARE_UNKNOWN,
        SHARE_DISK,
        SHARE_PRINTER,
    } ShareType;

    SharedResource() : wxObject() {}
    virtual ~SharedResource() {}

    wxString name;
    wxString description;
    ShareType sharetype;
};

WX_DECLARE_OBJARRAY(SharedResource, ArrayOfShares);

class WinShare {
public:
    WinShare();
    ~WinShare();

    ArrayOfShares GetShares();

private:
    DllData *dllPrivate;
    ArrayOfShares m_shares;
};

#endif
// _WINSHARE_H_
