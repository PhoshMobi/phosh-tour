/*
 * Copyright (C) 2022 Purism SPC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Guido Günther <agx@sigxcpu.org>
 */

#include "phosh-tour-config.h"
#include "phosh-tour-application.h"
#include "phosh-tour-window.h"
#include "pt-page.h"

#include <glib/gi18n.h>


struct _PhoshTourWindow {
  AdwApplicationWindow parent_instance;

  AdwCarousel         *main_carousel;
};

G_DEFINE_TYPE (PhoshTourWindow, phosh_tour_window, ADW_TYPE_APPLICATION_WINDOW)


static void
goto_page (PhoshTourWindow *self, int num)
{
  int n_pages = adw_carousel_get_n_pages (self->main_carousel);
  GtkWidget *page;

  if (num < 0)
    return;

  if (num >= n_pages)
    return;

  page = adw_carousel_get_nth_page (self->main_carousel, num);
  adw_carousel_scroll_to (self->main_carousel, page, TRUE);
}


static void
on_btn_next_clicked (PhoshTourWindow *self)
{
  int num = adw_carousel_get_position (self->main_carousel);

  goto_page (self, num + 1);
}

static void
on_btn_previous_clicked (PhoshTourWindow *self)
{
  int num = adw_carousel_get_position (self->main_carousel);

  goto_page (self, num - 1);
}

static gboolean
get_btn_next_visible (GObject *object, double position, int n_pages)
{
  if ((position + 0.5) > (n_pages - 1))
    return FALSE;

  return TRUE;
}


static gboolean
get_btn_previous_visible (GObject *object, double position)
{
  if (position < 0.5)
    return FALSE;

  return TRUE;
}


static void
phosh_tour_window_class_init (PhoshTourWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  g_type_ensure (PT_TYPE_PAGE);

  gtk_widget_class_set_template_from_resource (widget_class,
                                               "/mobi/phosh/PhoshTour/ui/phosh-tour-window.ui");
  gtk_widget_class_bind_template_child (widget_class, PhoshTourWindow, main_carousel);
  gtk_widget_class_bind_template_callback (widget_class, on_btn_next_clicked);
  gtk_widget_class_bind_template_callback (widget_class, on_btn_previous_clicked);
  gtk_widget_class_bind_template_callback (widget_class, get_btn_next_visible);
  gtk_widget_class_bind_template_callback (widget_class, get_btn_previous_visible);
}

static void
phosh_tour_window_init (PhoshTourWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}
