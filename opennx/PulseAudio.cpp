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

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "PulseAudio.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "PulseAudio.h"
#include "MyDynlib.h"

#include <wx/log.h>
#include <wx/utils.h>
#include <wx/regex.h>

#ifdef APP_OPENNX
# include "opennxApp.h"
#endif
#ifdef APP_PULSETEST
# include "pulseTest.h"
#endif

#include "trace.h"
ENABLE_TRACE;

#ifdef HAVE_PULSE_PULSEAUDIO_H
#include <pulse/pulseaudio.h>

typedef pa_threaded_mainloop* (*Tpa_threaded_mainloop_new)(void);
typedef pa_mainloop_api* (*Tpa_threaded_mainloop_get_api)(pa_threaded_mainloop*);
typedef int (*Tpa_threaded_mainloop_start)(pa_threaded_mainloop *);
typedef void (*Tpa_threaded_mainloop_stop)(pa_threaded_mainloop *);
typedef void (*Tpa_threaded_mainloop_free)(pa_threaded_mainloop *);
typedef pa_context* (*Tpa_context_new)(pa_mainloop_api *, const char *);
typedef void (*Tpa_context_set_state_callback)(pa_context *, pa_context_notify_cb_t, void *);
typedef int (*Tpa_context_connect)(pa_context *, const char *, pa_context_flags_t, const pa_spawn_api *);
typedef void (*Tpa_context_disconnect)(pa_context *);
typedef pa_operation* (*Tpa_context_drain)(pa_context *, pa_context_notify_cb_t, void *);
typedef pa_context_state_t (*Tpa_context_get_state)(pa_context *);
typedef pa_operation* (*Tpa_context_get_module_info_list)(pa_context *, pa_module_info_cb_t, void *);
typedef pa_operation* (*Tpa_context_load_module)(pa_context *, const char*, const char *, pa_context_index_cb_t, void *);
typedef pa_operation* (*Tpa_context_unload_module)(pa_context *, uint32_t, pa_context_success_cb_t, void *);
typedef int (*Tpa_context_errno)(pa_context *);
typedef void (*Tpa_context_unref)(pa_context *);
typedef void (*Tpa_operation_unref)(pa_operation *);
typedef const char* (*Tpa_strerror)(int);
typedef void (*Tpa_xfree)(void *p);


#define LOADPTR(name) p = dll->GetSymbol(wxT(#name)); if (NULL == p) return 0; P##name = (T##name)p
#define FPTR(name) static T##name P##name = NULL

FPTR(pa_threaded_mainloop_new);
FPTR(pa_threaded_mainloop_get_api);
FPTR(pa_threaded_mainloop_start);
FPTR(pa_threaded_mainloop_stop);
FPTR(pa_threaded_mainloop_free);
FPTR(pa_context_new);
FPTR(pa_context_set_state_callback);
FPTR(pa_context_connect);
FPTR(pa_context_disconnect);
FPTR(pa_context_drain);
FPTR(pa_context_get_state);
FPTR(pa_context_unref);
FPTR(pa_operation_unref);
FPTR(pa_context_get_module_info_list);
FPTR(pa_context_load_module);
FPTR(pa_context_unload_module);
FPTR(pa_context_errno);
FPTR(pa_strerror);
FPTR(pa_xfree);

static int _set_pasyms(MyDynamicLibrary *dll) {
    void *p;
    LOADPTR(pa_threaded_mainloop_new);
    LOADPTR(pa_threaded_mainloop_get_api);
    LOADPTR(pa_threaded_mainloop_start);
    LOADPTR(pa_threaded_mainloop_stop);
    LOADPTR(pa_threaded_mainloop_free);
    LOADPTR(pa_context_new);
    LOADPTR(pa_context_set_state_callback);
    LOADPTR(pa_context_connect);
    LOADPTR(pa_context_disconnect);
    LOADPTR(pa_context_drain);
    LOADPTR(pa_context_get_state);
    LOADPTR(pa_context_unref);
    LOADPTR(pa_operation_unref);
    LOADPTR(pa_context_get_module_info_list);
    LOADPTR(pa_context_load_module);
    LOADPTR(pa_context_unload_module);
    LOADPTR(pa_context_errno);
    LOADPTR(pa_strerror);
    LOADPTR(pa_xfree);
    return 1;
}

class pawrapper {
    private:
        typedef enum {
            NONE,
            LIST,
            LOAD_MODULE,
            UNLOAD_MODULE,
            SET_SINK_VOLUME,
            SET_SOURCE_VOLUME,
            SET_SINK_INPUT_VOLUME,
        } eAction;

    public:
        pawrapper()
            : m_bConnected(false), m_pLoop(NULL), m_pApi(NULL), m_pContext(NULL)
        {
            m_pLoop = Ppa_threaded_mainloop_new();
            m_pApi = Ppa_threaded_mainloop_get_api(m_pLoop);
            m_pContext = Ppa_context_new(m_pApi, "OpenNX");
            Ppa_context_set_state_callback(m_pContext, context_state_callback_if, this);
            if (0 <= Ppa_context_connect(m_pContext, NULL /* server */, PA_CONTEXT_NOAUTOSPAWN, NULL)) {
                Ppa_threaded_mainloop_start(m_pLoop);
                while (!m_bConnected)
                    ::wxGetApp().Yield(true);
            }
        }

        ~pawrapper()
        {
            if (m_bConnected)
                Ppa_context_disconnect(m_pContext);
            Ppa_threaded_mainloop_stop(m_pLoop);
            Ppa_threaded_mainloop_free(m_pLoop);
            if (NULL != m_pContext)
                Ppa_context_unref(m_pContext);
        }

        bool findmodule(const wxChar *name, wxString &args, unsigned int &index)
        {
            m_sStr = name;
            m_bSearch = true;
            m_bFound = false;
            m_bComplete = false;
            m_bError = false;
            Ppa_operation_unref(Ppa_context_get_module_info_list(m_pContext, get_module_info_callback_if, this));
            bool ret = waitcmd();
            if (ret && m_bFound) {
               args = m_sStr;
               index = m_iIndex;
            }
            return ret && m_bFound;
        }

        bool loadmodule(const wxString name, const wxString args)
        {
            m_bComplete = false;
            m_bError = false;
            Ppa_operation_unref(Ppa_context_load_module(m_pContext, name.mb_str(), args.mb_str(), index_callback_if, this));
            return waitcmd();
        }

        bool unloadmodule(int index)
        {
            m_bComplete = false;
            m_bError = false;
            Ppa_operation_unref(Ppa_context_unload_module(m_pContext, index, simple_callback_if, this));
            return waitcmd();
        }

        bool isConnected()
        {
            return m_bConnected;
        }

    private:

        bool waitcmd() {
            while (!(m_bError || m_bComplete)) {
                ::wxGetApp().Yield(true);
            }
            return !m_bError;
        }

#if 0
        void runcmd(eAction a)
        {
            m_bComplete = false;
            m_bError = false;
            switch (a) {
                case SET_SINK_VOLUME:
                    {
                        pa_cvolume v;

                        pa_cvolume_set(&v, 1, volume);
                        Ppa_operation_unref(pa_context_set_sink_volume_by_name(m_pContext, sink_name, &v, simple_callback_if, NULL));
                        break;
                    }
                case SET_SOURCE_VOLUME:
                    {
                        pa_cvolume v;

                        pa_cvolume_set(&v, 1, volume);
                        Ppa_operation_unref(pa_context_set_source_volume_by_name(m_pContext, source_name, &v, simple_callback_if, NULL));
                        break;
                    }
                case SET_SINK_INPUT_VOLUME:
                    {
                        pa_cvolume v;

                        pa_cvolume_set(&v, 1, volume);
                        Ppa_operation_unref(pa_context_set_sink_input_volume(m_pContext, sink_input_idx, &v, simple_callback_if, NULL));
                        break;
                    }
            }
        }
#endif

        void drain(void) {
            pa_operation *o = Ppa_context_drain(m_pContext, context_drain_complete_if, this);
            if (NULL == o) {
                ::myLogTrace(MYTRACETAG, wxT("drain_complete"));
                m_bComplete = true;
            } else
                Ppa_operation_unref(o);
        }

        void context_state_callback(pa_context *c)
        {
            if (NULL == c)
                return;
            switch (Ppa_context_get_state(c)) {
                case PA_CONTEXT_CONNECTING:
                    ::myLogTrace(MYTRACETAG, wxT("PA_CONTEXT_CONNECTING"));
                    break;
                case PA_CONTEXT_AUTHORIZING:
                    ::myLogTrace(MYTRACETAG, wxT("PA_CONTEXT_AUTHORIZING"));
                    break;
                case PA_CONTEXT_SETTING_NAME:
                    ::myLogTrace(MYTRACETAG, wxT("PA_CONTEXT_SETTING_NAME"));
                    break;

                case PA_CONTEXT_READY:
                    ::myLogTrace(MYTRACETAG, wxT("PA_CONTEXT_READY"));
                    m_bConnected = true;
                    break;
                case PA_CONTEXT_TERMINATED:
                    ::myLogTrace(MYTRACETAG, wxT("PA_CONTEXT_TERMINATED"));
                    break;
                case PA_CONTEXT_FAILED:
                    ::myLogTrace(MYTRACETAG, wxT("PA_CONTEXT_FAILED"));
                    m_bError = true;
                    m_pApi->quit(m_pApi, 0);
                    break;
                default:
                    ::myLogTrace(MYTRACETAG, wxT("PA_CONTEXT_default"));
                    m_bError = true;
                    m_pApi->quit(m_pApi, 0);
                    break;
            }
        }

        void get_module_info_callback(pa_context *c, const pa_module_info *i, int is_last)
        {
            char t[32];
            char *pl;

            if (is_last < 0) {
                ::myLogTrace(MYTRACETAG, wxT("Failed to get module information: %s"), Ppa_strerror(Ppa_context_errno(c)));
                m_bError = true;
                m_pApi->quit(m_pApi, 0);
                return;
            }
            if (is_last) {
                drain();
                return;
            }
            if (NULL != i) {
                wxString name(i->name, wxConvUTF8);
                wxString args(i->argument ? i->argument : "", wxConvUTF8);
                ::myLogTrace(MYTRACETAG, wxT("module[%u] %s %s"),
                        i->index, name.c_str(), args.c_str());
                if (m_bSearch) {
                    if (name.IsSameAs(m_sStr)) {
                        m_bSearch = false;
                        m_bFound = true;
                        m_iIndex = i->index;
                        m_sStr = args;
                    }
                }
            }
        }

        void index_callback(pa_context *c, uint32_t idx)
        {
            if (idx == PA_INVALID_INDEX) {
                ::myLogTrace(MYTRACETAG, wxT("Index failure: %s"), Ppa_strerror(Ppa_context_errno(c)));
                m_bError = true;
                m_pApi->quit(m_pApi, 0);
                return;
            }
            m_iIndex = idx;
            drain();
        }

        void simple_callback(pa_context *c, int success)
        {
            if (!success) {
                m_bError = true;
                ::myLogTrace(MYTRACETAG, wxT("Simple failure: %s"), Ppa_strerror(Ppa_context_errno(c)));
                m_pApi->quit(m_pApi, 0);
                return;
            }
            drain();
        }

        void context_drain_complete(pa_context *)
        {
            ::myLogTrace(MYTRACETAG, wxT("context_drain_complete"));
            m_bComplete = true;
        }

        static void simple_callback_if(pa_context *c, int success, void *udata) {
            if (NULL == udata)
                return;
            static_cast<pawrapper *>(udata)->simple_callback(c, success);
        }

        static void index_callback_if(pa_context *c, uint32_t idx, void *udata) {
            if (NULL == udata)
                return;
            static_cast<pawrapper *>(udata)->index_callback(c, idx);
        }

        static void context_drain_complete_if(pa_context *c, void *udata) {
            if (NULL == udata)
                return;
            static_cast<pawrapper *>(udata)->context_drain_complete(c);
        }

        static void get_module_info_callback_if(pa_context *c, const pa_module_info *i, int last, void *udata) {
            if (NULL == udata)
                return;
            static_cast<pawrapper *>(udata)->get_module_info_callback(c, i, last);
        }

        static void context_state_callback_if(pa_context *c, void *udata) {
            if (NULL == udata)
                return;
            static_cast<pawrapper *>(udata)->context_state_callback(c);
        }

        bool m_bConnected;
        bool m_bComplete;
        bool m_bError;
        bool m_bSearch;
        bool m_bFound;
        unsigned int m_iIndex;
        pa_threaded_mainloop *m_pLoop;
        pa_mainloop_api *m_pApi;
        pa_context *m_pContext;
        wxString m_sStr;

};

#endif // HAVE_PULSE_PULSEAUDIO_H

    PulseAudio::PulseAudio()
: pa(NULL), dll(NULL), m_bPulseAvailable(false)
{
#ifdef HAVE_PULSE_PULSEAUDIO_H
    wxLogNull ignoreErrors;
    dll = new MyDynamicLibrary();
    if (dll->Load(wxT("libpulse"))) {
        ::myLogTrace(MYTRACETAG, wxT("libpulse loaded"));
        if (0 != _set_pasyms(dll)) {
            ::myLogTrace(MYTRACETAG, wxT("libpulse functions loaded"));
            pa = new pawrapper();
            if (pa->isConnected()) {
                m_bPulseAvailable = true;
                ::myLogTrace(MYTRACETAG, wxT("connected to pulseaudio daemon"));
            }
        }
    }
#else
    ::myLogTrace(MYTRACETAG, wxT("No pulseaudio support"));
#endif
}

PulseAudio::~PulseAudio()
{
#ifdef HAVE_PULSE_PULSEAUDIO_H
    delete pa;
    delete dll;
#endif
}

bool PulseAudio::IsAvailable()
{
    return m_bPulseAvailable;
}

bool PulseAudio::ActivateEsound(int port)
{
    if (!m_bPulseAvailable)
        return false;
#ifdef HAVE_PULSE_PULSEAUDIO_H
    wxString ma;
    unsigned int mi;
    if (pa->findmodule(wxT("module-esound-protocol-tcp"), ma, mi)) {
        ::myLogTrace(MYTRACETAG, wxT("found esdmod, idx=%u args='%s'"), mi, ma.c_str());
        long mport = 16001;
        wxString laddr(wxT("0.0.0.0"));
        wxRegEx rePort(wxT("port=(\\d+)"), wxRE_ADVANCED);
        if (rePort.Matches(ma)) {
            rePort.GetMatch(ma, 1).ToLong(&mport);
            ::myLogTrace(MYTRACETAG, wxT("matched port arg p=%d"), mport);
        }
        wxRegEx reListen(wxT("listen=(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})"), wxRE_ADVANCED);
        if (reListen.Matches(ma)) {
            laddr = reListen.GetMatch(ma, 1);
            ::myLogTrace(MYTRACETAG, wxT("matched listen arg a=%s"), laddr.c_str());
        }
        if ((mport == port) && (laddr.IsSameAs(wxT("0.0.0.0")) || laddr.IsSameAs(wxT("127.0.0.1"))))
            return true;
        ::myLogTrace(MYTRACETAG, wxT("unloading"));
        if (!pa->unloadmodule(mi))
            return false;
    }
    ::myLogTrace(MYTRACETAG, wxT("loading"));
    return pa->loadmodule(wxT("module-esound-protocol-tcp"),
            wxString::Format(wxT("port=%d listen=127.0.0.1"), port));
#endif
}
