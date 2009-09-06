// $Id$
//
// Copyright (C) 2006 The OpenNX Team
// Author: Fritz Elfert
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this program; if not, write to the
// Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

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
