/*
 * Copyright (C) 2022 Purism SPC
 *               2023-2024 Guido Günther
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Guido Günther <agx@sigxcpu.org>
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define PT_TYPE_APPLICATION (pt_application_get_type ())

G_DECLARE_FINAL_TYPE (PtApplication, pt_application, PT, APPLICATION, AdwApplication)

PtApplication *pt_application_new (gchar *application_id, GApplicationFlags flags);
GtkWidget     *pt_application_get_device_panel (PtApplication *self);

G_END_DECLS
