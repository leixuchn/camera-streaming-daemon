/*
 * This file is part of the Camera Streaming Daemon
 *
 * Copyright (C) 2017  Intel Corporation. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <glib-unix.h>
#include <glib.h>
#include <signal.h>

#include "glib_mainloop.h"
#include "log.h"

static GMainLoop *gmainloop = nullptr;

static gboolean exit_signal_handler(gpointer mainloop)
{
    g_main_loop_quit((GMainLoop *)mainloop);
    return true;
}

static void setup_signal_handlers()
{
    GSource *source = NULL;
    GMainContext *ctx = g_main_loop_get_context(gmainloop);

    source = g_unix_signal_source_new(SIGINT);
    g_source_set_callback(source, exit_signal_handler, gmainloop, NULL);
    g_source_attach(source, ctx);

    source = g_unix_signal_source_new(SIGTERM);
    g_source_set_callback(source, exit_signal_handler, gmainloop, NULL);
    g_source_attach(source, ctx);
}

GlibMainloop::GlibMainloop()
    : avahi_poll(nullptr)
{
    gmainloop = g_main_loop_new(NULL, FALSE);
    mainloop = this;
    setup_signal_handlers();
}

GlibMainloop::~GlibMainloop()
{
    mainloop = nullptr;
    if (avahi_poll) {
        avahi_glib_poll_free(avahi_poll);
        avahi_poll = nullptr;
    }

    g_main_loop_unref(gmainloop);
    gmainloop = nullptr;
}

void GlibMainloop::loop()
{
    g_main_loop_run(gmainloop);
}

const AvahiPoll *GlibMainloop::get_avahi_poll_api()
{
    if (!avahi_poll)
        avahi_poll = avahi_glib_poll_new(NULL, G_PRIORITY_DEFAULT);
    return avahi_glib_poll_get(avahi_poll);
}

void GlibMainloop::quit()
{
    g_main_loop_quit(gmainloop);
}
