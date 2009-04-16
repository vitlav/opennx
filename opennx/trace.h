#ifndef _TRACE_H_
#define _TRACE_H_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vector>
#include <string>
#include <wx/filename.h>

extern std::vector<std::string> _allTraceTags;

#define ENABLE_TRACE \
    static wxString MYTRACETAG(wxFileName::FileName(wxT(__FILE__)).GetName()); \
    static void __attribute__((constructor)) _trinit() { \
        _allTraceTags.push_back(std::string(__FILE__)); \
    }

#define DECLARE_TRACETAGS \
    std::vector<std::string> _allTraceTags; \
    static wxArrayString allTraceTags; \
    static void initWxTraceTags() { \
        std::vector<std::string>::iterator i; \
       for (i = _allTraceTags.begin(); i != _allTraceTags.end(); i++) { \
           wxString tag(i->c_str(), wxConvUTF8); \
           allTraceTags.Add(wxFileName::FileName(tag).GetName()); \
       } \
    }

#endif
