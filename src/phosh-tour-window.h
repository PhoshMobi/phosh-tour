/*
 * Copyright (C) 2022 Purism SPC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Guido Günther <agx@sigxcpu.org>
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define PHOSH_TOUR_TYPE_WINDOW (phosh_tour_window_get_type ())

G_DECLARE_FINAL_TYPE (PhoshTourWindow, phosh_tour_window, PHOSH_TOUR, WINDOW, AdwApplicationWindow)

G_END_DECLS
