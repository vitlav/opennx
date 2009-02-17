#ifndef _TRACE_H_
#define _TRACE_H_

#include <wx/arrstr.h>
#include <wx/filename.h>

extern wxArrayString *allTraceTags;

#define ENABLE_TRACE \
    static wxString MYTRACETAG(wxFileName::FileName(wxT(__FILE__)).GetName()); \
    static void __attribute__((constructor)) _trinit() { \
        if (!allTraceTags) \
            allTraceTags = new wxArrayString(); \
        allTraceTags->Add(MYTRACETAG); \
    }

#define DECLARE_TRACETAGS wxArrayString *allTraceTags = NULL;

#endif
