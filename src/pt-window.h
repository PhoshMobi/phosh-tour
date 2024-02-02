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

#define PT_TYPE_WINDOW (pt_window_get_type ())

G_DECLARE_FINAL_TYPE (PtWindow, pt_window, PT, WINDOW, AdwApplicationWindow)

G_END_DECLS
