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

#include <glib/gi18n.h>


struct _PhoshTourApplication {
  GtkApplication parent_instance;
};

G_DEFINE_TYPE (PhoshTourApplication, phosh_tour_application, ADW_TYPE_APPLICATION)


PhoshTourApplication *
phosh_tour_application_new (gchar *application_id, GApplicationFlags flags)
{
  return g_object_new (PHOSH_TOUR_TYPE_APPLICATION,
                       "application-id", application_id,
                       "flags", flags,
                       NULL);
}

static void
phosh_tour_application_activate (GApplication *app)
{
  GtkWindow *window;

  g_assert (GTK_IS_APPLICATION (app));

  window = gtk_application_get_active_window (GTK_APPLICATION (app));
  if (window == NULL)
    window = g_object_new (PHOSH_TOUR_TYPE_WINDOW, "application", app, NULL);

  gtk_window_present (window);
}


static void
phosh_tour_application_class_init (PhoshTourApplicationClass *klass)
{
  GApplicationClass *app_class = G_APPLICATION_CLASS (klass);

  app_class->activate = phosh_tour_application_activate;
}


static void
phosh_tour_application_show_about (GSimpleAction *action,
                                   GVariant      *parameter,
                                   gpointer       user_data)
{
  PhoshTourApplication *self = PHOSH_TOUR_APPLICATION (user_data);
  GtkWindow *window = NULL;
  const gchar *authors[] = {"Guido Günther", NULL};
  const gchar *artists[] = {"Sam Hewitt ", NULL};

  g_return_if_fail (PHOSH_TOUR_IS_APPLICATION (self));

  window = gtk_application_get_active_window (GTK_APPLICATION (self));

  gtk_show_about_dialog (window,
                         "artists", artists,
                         "authors", authors,
                         "license-type", GTK_LICENSE_GPL_3_0,
                         "logo-icon-name", "sm.puri.PhoshTour",
                         "program-name", PHOSH_TOUR_BRAND "Tour",
                         "title", _ ("About " PHOSH_TOUR_BRAND " "),
                         "translator-credits", _ ("translator-credits"),
                         "version", PACKAGE_VERSION,
                         "website", "https://gitlab.gnome.org/guidog/phosh-tour",
                         NULL);
}


static void
phosh_tour_application_init (PhoshTourApplication *self)
{
  g_autoptr (GSimpleAction) about_action = NULL;
  g_autoptr (GSimpleAction) quit_action = NULL;

  quit_action = g_simple_action_new ("quit", NULL);
  g_signal_connect_swapped (quit_action, "activate", G_CALLBACK (g_application_quit), self);
  g_action_map_add_action (G_ACTION_MAP (self), G_ACTION (quit_action));

  about_action = g_simple_action_new ("about", NULL);
  g_signal_connect (about_action, "activate", G_CALLBACK (phosh_tour_application_show_about), self);
  g_action_map_add_action (G_ACTION_MAP (self), G_ACTION (about_action));

  gtk_application_set_accels_for_action (GTK_APPLICATION (self),
                                         "app.quit",
                                         (const char *[]) {
    "<primary>q",
    NULL,
  });
}
