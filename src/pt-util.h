/*
 * Copyright (C) 2022 Purism SPC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Guido Günther <agx@sigxcpu.org>
 */
#pragma once

#include <glib.h>

G_BEGIN_DECLS

#define STR_IS_NULL_OR_EMPTY(x) ((x) == NULL || (x)[0] == '\0')

guint pt_string_replace (GString *string, const gchar *find, const gchar *replace, guint limit);

G_END_DECLS
