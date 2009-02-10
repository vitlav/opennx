/////////////////////////////////////////////////////////////////////////////
// Name:        tracelogApp.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     19/10/2008 02:49:40
// RCS-ID:      
// Copyright:   (C) 2008 by Fritz Elfert
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _TRACELOGAPP_H_
#define _TRACELOGAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/xrc/xmlres.h"
#include "wx/image.h"
#include "TraceLogFrame.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

class wxTaskBarIcon;
class TraceLogFrame;
class DebugStringGrabber;

/*!
 * tracelogApp class declaration
 */

class tracelogApp: public wxApp
{    
    DECLARE_CLASS( tracelogApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    tracelogApp();

    void Init();
    const wxString &GetResourcePrefix() { return m_sResourcePrefix; }

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

    void Terminate();
    void OnDebugString(wxCommandEvent &event);

////@begin tracelogApp event handler declarations

////@end tracelogApp event handler declarations

////@begin tracelogApp member function declarations

////@end tracelogApp member function declarations

////@begin tracelogApp member variables
////@end tracelogApp member variables

private:
    wxString m_sResourcePrefix;
    wxTaskBarIcon *m_pTaskBarIcon;
    TraceLogFrame *m_pLogFrame;
    DebugStringGrabber *m_pGrabber;
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(tracelogApp)
////@end declare app

#endif
    // _TRACELOGAPP_H_
