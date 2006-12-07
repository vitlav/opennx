/////////////////////////////////////////////////////////////////////////////
// Name:        TextViewer.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     29/03/2006 15:51:47
// RCS-ID:      
// Copyright:   (C) 2006 InnoviData GmbH
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _TEXTVIEWER_H_
#define _TEXTVIEWER_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "TextViewer.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "TextViewer_symbols.h"
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
#define ID_TEXTVIEWER 10020
#define SYMBOL_TEXTVIEWER_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxDIALOG_NO_PARENT|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxDIALOG_MODAL
#define SYMBOL_TEXTVIEWER_TITLE _("Text Viewer")
#define SYMBOL_TEXTVIEWER_IDNAME ID_TEXTVIEWER
#define SYMBOL_TEXTVIEWER_SIZE wxDefaultSize
#define SYMBOL_TEXTVIEWER_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * TextViewer class declaration
 */

class TextViewer: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( TextViewer )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    TextViewer( );
    TextViewer( wxWindow* parent, wxWindowID id = SYMBOL_TEXTVIEWER_IDNAME, const wxString& caption = SYMBOL_TEXTVIEWER_TITLE, const wxPoint& pos = SYMBOL_TEXTVIEWER_POSITION, const wxSize& size = SYMBOL_TEXTVIEWER_SIZE, long style = SYMBOL_TEXTVIEWER_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TEXTVIEWER_IDNAME, const wxString& caption = SYMBOL_TEXTVIEWER_TITLE, const wxPoint& pos = SYMBOL_TEXTVIEWER_POSITION, const wxSize& size = SYMBOL_TEXTVIEWER_SIZE, long style = SYMBOL_TEXTVIEWER_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

    bool LoadFile(const wxString &file);

private:
////@begin TextViewer event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CLOSE
    void OnCloseClick( wxCommandEvent& event );

////@end TextViewer event handler declarations

////@begin TextViewer member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end TextViewer member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin TextViewer member variables
    wxTextCtrl* m_pTextCtrl;
    wxString m_sFileName;
////@end TextViewer member variables
};

#endif
    // _TEXTVIEWER_H_
