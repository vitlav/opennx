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

#ifndef _LOGNULL_H_
#define _LOGNULL_H_

#include <wx/log.h>

/**
 * For supressing error dialogs (when performing
 * custom error handling), wxWidgets provides the
 * wxLogNull class. Unfortunately, this class disables
 * *ALL* logging. In a debug build however, we still
 * want trace- and debug logging. Therefore, we use our
 * own LogNull class, which retains output of these
 * log levels.
 */
class LogNull : public wxLog
{
    private:
        wxLog *m_pOldLog;

    public:
        LogNull() : wxLog() {
            m_pOldLog = wxLog::SetActiveTarget(this);
        }
        ~LogNull() {
            wxLog::SetActiveTarget(m_pOldLog);
        }

    protected:
        void DoLog(wxLogLevel lv, const wxChar *msg, time_t t)
        {
#ifdef __WXDEBUG__
            switch (lv) {
                case wxLOG_Trace:
                case wxLOG_Debug:
                    ((LogNull *)m_pOldLog)->DoLog(lv, msg, t);
                    break;
                default:
                    break;
            }
#else
            wxUnusedVar(lv);
            wxUnusedVar(msg);
            wxUnusedVar(t);
#endif
        }
};

#endif
