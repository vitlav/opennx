// $Id$
//
// Copyright (C) 2006 The OpenNX team
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

#ifndef _TRACELOGDIALOG_H_
#define _TRACELOGDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "TraceLogDialog.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "TraceLogDialog_symbols.h"
#include "wx/xrc/xmlres.h"
#include "wx/statline.h"
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
#define ID_TRACELOGDIALOG 10106
#define SYMBOL_TRACELOGDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxCLIP_CHILDREN|wxTAB_TRAVERSAL
#define SYMBOL_TRACELOGDIALOG_TITLE _("Trace Log")
#define SYMBOL_TRACELOGDIALOG_IDNAME ID_TRACELOGDIALOG
#define SYMBOL_TRACELOGDIALOG_SIZE wxSize(600, 300)
#define SYMBOL_TRACELOGDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * TraceLogDialog class declaration
 */

class TraceLogDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( TraceLogDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    TraceLogDialog();
    TraceLogDialog( wxWindow* parent, wxWindowID id = SYMBOL_TRACELOGDIALOG_IDNAME, const wxString& caption = SYMBOL_TRACELOGDIALOG_TITLE, const wxPoint& pos = SYMBOL_TRACELOGDIALOG_POSITION, const wxSize& size = SYMBOL_TRACELOGDIALOG_SIZE, long style = SYMBOL_TRACELOGDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TRACELOGDIALOG_IDNAME, const wxString& caption = SYMBOL_TRACELOGDIALOG_TITLE, const wxPoint& pos = SYMBOL_TRACELOGDIALOG_POSITION, const wxSize& size = SYMBOL_TRACELOGDIALOG_SIZE, long style = SYMBOL_TRACELOGDIALOG_STYLE );

    /// Destructor
    ~TraceLogDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    void AddEntry(pid_t pid, wxDateTime stamp, wxString &txt);

////@begin TraceLogDialog event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CLOSE
    void OnCLOSEClick( wxCommandEvent& event );

////@end TraceLogDialog event handler declarations

////@begin TraceLogDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end TraceLogDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin TraceLogDialog member variables
////@end TraceLogDialog member variables
};

#endif
    // _TRACELOGDIALOG_H_
