/*
 * Copyright (C) 2026 Phosh Developers
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <pt-page.h>

G_BEGIN_DECLS

#define PT_TYPE_DISTRO_PAGE (pt_distro_page_get_type ())

G_DECLARE_FINAL_TYPE (PtDistroPage, pt_distro_page, PT, DISTRO_PAGE, PtPage)

PtDistroPage *pt_distro_page_new (void);
gboolean  pt_distro_page_has_id (PtDistroPage *self, const char *id);

G_END_DECLS
