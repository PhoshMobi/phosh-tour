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

#define PHOSH_TOUR_TYPE_APPLICATION (phosh_tour_application_get_type ())

G_DECLARE_FINAL_TYPE (PhoshTourApplication, phosh_tour_application, PHOSH_TOUR, APPLICATION, AdwApplication)

PhoshTourApplication *phosh_tour_application_new (gchar *application_id,
                                                  GApplicationFlags flags);
GtkWidget *phosh_tour_application_get_device_panel  (PhoshTourApplication *self);

G_END_DECLS
