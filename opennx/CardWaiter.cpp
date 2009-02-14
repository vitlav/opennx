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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "CardWaitThread.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "wx/dynlib.h"
#include <wx/event.h>
#include <wx/thread.h>

#include "CardWaiter.h"
#include "CardWaiterDialog.h"
#include <opensc/opensc.h>

DEFINE_LOCAL_EVENT_TYPE(wxEVT_CARDINSERTED);

class CardWaitThread : public wxThreadHelper
{
    public:
        CardWaitThread(wxEvtHandler *handler = NULL);
        virtual ~CardWaitThread();

        virtual wxThread::ExitCode Entry();

        bool IsOk() { return m_bOk; }
        int GetReader() { return m_iFoundID; }
        void SetHandler(wxEvtHandler *handler) { m_pEvtHandler = handler; }

    private:
        wxEvtHandler *m_pEvtHandler;
        bool m_bOk;
        bool m_bTerminate;
        bool m_bFirstLoopDone;
        int m_iFoundID;
};

CardWaitThread::CardWaitThread(wxEvtHandler *handler)
    : wxThreadHelper()
    , m_pEvtHandler(handler)
    , m_bOk(false)
    , m_bTerminate(false)
    , m_bFirstLoopDone(false)
    , m_iFoundID(-1)
{
    if (Create(
#ifdef __OPENBSD__
            32768
#endif
          ) == wxTHREAD_NO_ERROR) {
        GetThread()->Run();
        while ((!m_bFirstLoopDone) && GetThread()->IsRunning())
            wxThread::Sleep(100);
    }
}

CardWaitThread::~CardWaitThread()
{
    m_pEvtHandler = NULL;
    if (m_bOk) {
        m_bTerminate = true;
        GetThread()->Delete();
        while (m_bOk)
            wxThread::Sleep(100);
    }
}

typedef int (*Tsc_context_create)(sc_context_t **ctx, const sc_context_param_t *parm);
typedef int (*Tsc_release_context)(sc_context_t *ctx);
typedef unsigned int (*Tsc_ctx_get_reader_count)(sc_context_t *ctx);
typedef sc_reader_t *(*Tsc_ctx_get_reader)(sc_context_t *ctx, unsigned int i);
typedef int (*Tsc_wait_for_event)(sc_reader_t **readers, int *slots, size_t nslots,
                      unsigned int event_mask,
                      int *reader, unsigned int *event, int timeout);
typedef int (*Tsc_ctx_detect_readers)(sc_context_t *ctx);

    wxThread::ExitCode
CardWaitThread::Entry()
{
    sc_context *ctx;
    wxDynamicLibrary dll(wxT("libopensc"));

    wxDYNLIB_FUNCTION(Tsc_context_create, sc_context_create, dll);
    if (!pfnsc_context_create)
        return 0;
    wxDYNLIB_FUNCTION(Tsc_release_context, sc_release_context, dll);
    if (!pfnsc_release_context)
        return 0;
    wxDYNLIB_FUNCTION(Tsc_ctx_get_reader_count, sc_ctx_get_reader_count, dll);
    if (!pfnsc_ctx_get_reader_count)
        return 0;
    wxDYNLIB_FUNCTION(Tsc_ctx_get_reader, sc_ctx_get_reader, dll);
    if (!pfnsc_ctx_get_reader)
        return 0;
    wxDYNLIB_FUNCTION(Tsc_wait_for_event, sc_wait_for_event, dll);
    if (!pfnsc_wait_for_event)
        return 0;
    wxDYNLIB_FUNCTION(Tsc_ctx_detect_readers, sc_ctx_detect_readers, dll);
    if (!pfnsc_ctx_detect_readers)
        return 0;

    if (SC_SUCCESS != pfnsc_context_create(&ctx, NULL))
        return 0;
    m_bOk = true;
    while (!m_thread->TestDestroy()) {
        if (m_bTerminate)
            break;
        int found_id = -1;
    	sc_reader_t *reader = NULL;
    	sc_card_t *card = NULL;
        int r;
		sc_reader_t *readers[16];
		int slots[16];
		unsigned int i;
		int j, k, found;
		unsigned int event;

        memset(readers, 0, sizeof(readers));
        memset(slots, 0, sizeof(slots));
        pfnsc_ctx_detect_readers(ctx);
		for (i = k = 0; i < pfnsc_ctx_get_reader_count(ctx); i++) {
			reader = pfnsc_ctx_get_reader(ctx, i);
			for (j = 0; j < reader->slot_count; j++, k++) {
				readers[k] = reader;
				slots[k] = j;
			}
		}
		if (0 == pfnsc_wait_for_event(readers, slots, k,
				SC_EVENT_CARD_INSERTED, &found, &event, 400)) {
            for (i = 0; i < pfnsc_ctx_get_reader_count(ctx); i++) {
                reader = pfnsc_ctx_get_reader(ctx, i);
                if (reader == readers[found]) {
                    found_id = i;
                }
            }
        }
        if (found_id != m_iFoundID) {
            m_iFoundID = found_id;
            if (m_iFoundID != -1) {
                if (m_pEvtHandler) {
                    wxCommandEvent ev(wxEVT_CARDINSERTED, wxID_ANY);
                    ev.SetInt(m_iFoundID);
                    m_pEvtHandler->AddPendingEvent(ev);
                }
            }
        }
        m_bFirstLoopDone = true;
    }
    pfnsc_release_context(ctx);
    m_bOk = false;
    return 0;
}

CardWaiter::CardWaiter(wxWindow *parent)
{
    this->parent = parent;
}

int CardWaiter::WaitForCard() {
    CardWaiterDialog d(parent);
    CardWaitThread t(&d);
    if (t.IsOk() && (t.GetReader() != -1))
        return t.GetReader();
    return d.ShowModal();
}
