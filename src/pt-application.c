/*
 * Copyright (C) 2022 Purism SPC
 *               2023-2024 Guido Günther
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Guido Günther <agx@sigxcpu.org>
 */

#include "phosh-tour-config.h"

#include "pt-application.h"
#include "pt-window.h"

#include <glib/gi18n.h>

#define DESC _("- A graphical tour introducing your device")

struct _PtApplication {
  GtkApplication parent_instance;

  gboolean run_once;
};

G_DEFINE_TYPE (PtApplication, pt_application, ADW_TYPE_APPLICATION)


static const GOptionEntry entries[] = {
  { "version", '\0', G_OPTION_FLAG_NONE, G_OPTION_ARG_NONE,
    NULL, "Get the current version", NULL,
  },
  { "run-once", '\0', 0, G_OPTION_ARG_NONE,
    NULL, "Run the tour only once and then exit", NULL
  },
  G_OPTION_ENTRY_NULL
};


PtApplication *
pt_application_new (char *application_id, GApplicationFlags flags)
{
  return g_object_new (PT_TYPE_APPLICATION,
                       "application-id", application_id,
                       "flags", flags,
                       NULL);
}


static gboolean
pt_application_check_and_create_run_once (void)
{
  g_autofree char *config_path = NULL;
  g_autoptr(GError) error = NULL;
  g_autoptr(GFile) file = NULL;
  g_autoptr(GFileOutputStream) output_stream = NULL;

  config_path = g_build_filename (g_get_user_config_dir (), "phosh-tour", "run-once", NULL);

  if (g_file_test (config_path, G_FILE_TEST_EXISTS)) {
    g_debug ("Phosh tour already ran once.");
    return TRUE;
  }

  if (g_mkdir_with_parents (g_path_get_dirname (config_path), 0755) != 0) {
    g_warning ("Error creating directory for run-once file.");
    return FALSE;
  }

  file = g_file_new_for_path (config_path);
  output_stream = g_file_create (file, G_FILE_CREATE_NONE, NULL, &error);
  if (!output_stream) {
    g_warning ("Error creating run-once file: %s", error->message);
    return FALSE;
  }
  if (!g_output_stream_close (G_OUTPUT_STREAM (output_stream), NULL, &error)) {
    g_warning ("Error closing run-once file: %s", error->message);
    return FALSE;
  }

  g_debug ("Running Phosh tour for the first time.");

  return FALSE;
}


static void
pt_application_activate (GApplication *app)
{
  GtkWindow *window;
  PtApplication *self = PT_APPLICATION (app);

  g_assert (GTK_IS_APPLICATION (app));

  if (self->run_once) {
    if (pt_application_check_and_create_run_once ()) {
      g_debug ("Phosh tour already completed once.");
      g_application_quit (app);
      return;
    } else {
      g_debug ("Running Phosh tour for the first time.");
    }
  }

  window = gtk_application_get_active_window (GTK_APPLICATION (app));
  if (window == NULL)
    window = g_object_new (PT_TYPE_WINDOW, "application", app, NULL);

  gtk_window_present (window);
}


static int
pt_application_handle_local_options (GApplication *app, GVariantDict *options)
{
  PtApplication *self = PT_APPLICATION (app);

  if (g_variant_dict_contains (options, "version")) {
    g_print ("%s %s %s\n", PHOSH_TOUR_APP, PHOSH_TOUR_VERSION, DESC);

    return 0;
  }

  if (g_variant_dict_contains (options, "run-once")) {
    self->run_once = TRUE;
    g_debug ("Running the tour with --run-once option.\n");
  }

  return G_APPLICATION_CLASS (pt_application_parent_class)->handle_local_options (app, options);
}


static void
pt_application_class_init (PtApplicationClass *klass)
{
  GApplicationClass *app_class = G_APPLICATION_CLASS (klass);

  app_class->activate = pt_application_activate;
  app_class->handle_local_options = pt_application_handle_local_options;
}


static void
pt_application_show_about (GSimpleAction *action,
                                   GVariant      *parameter,
                                   gpointer       user_data)
{
  PtApplication *self = PT_APPLICATION (user_data);
  GtkWindow *window = NULL;
  const char *authors[] = {"Guido Günther", NULL};
  const char *artists[] = {"Sam Hewitt ", NULL};

  g_return_if_fail (PT_IS_APPLICATION (self));

  window = gtk_application_get_active_window (GTK_APPLICATION (self));

  gtk_show_about_dialog (window,
                         "artists", artists,
                         "authors", authors,
                         "license-type", GTK_LICENSE_GPL_3_0,
                         "logo-icon-name", "mobi.phosh.PhoshTour",
                         "program-name", PHOSH_TOUR_BRAND "Tour",
                         "title", _("About " PHOSH_TOUR_BRAND " "),
                         "translator-credits", _ ("translator-credits"),
                         "version", PHOSH_TOUR_VERSION,
                         "website", "https://gitlab.gnome.org/World/Phosh/phosh-tour",
                         NULL);
}


static void
pt_application_init (PtApplication *self)
{
  g_autoptr (GSimpleAction) about_action = NULL;
  g_autoptr (GSimpleAction) quit_action = NULL;

  quit_action = g_simple_action_new ("quit", NULL);
  g_signal_connect_swapped (quit_action, "activate", G_CALLBACK (g_application_quit), self);
  g_action_map_add_action (G_ACTION_MAP (self), G_ACTION (quit_action));

  about_action = g_simple_action_new ("about", NULL);
  g_signal_connect (about_action, "activate", G_CALLBACK (pt_application_show_about), self);
  g_action_map_add_action (G_ACTION_MAP (self), G_ACTION (about_action));

  gtk_application_set_accels_for_action (GTK_APPLICATION (self),
                                         "app.quit",
                                         (const char *[]) {
    "<primary>q",
    NULL,
  });

  g_application_set_option_context_parameter_string (G_APPLICATION (self), DESC);
  g_application_add_main_option_entries (G_APPLICATION (self), entries);
}
