/*
 * Copyright (C) 2026 Phosh Developers
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Achill Gilgenast <achill@achill.org>
 */

#define G_LOG_DOMAIN "pt-distro-page"

#include "phosh-tour-config.h"

#include "pt-distro-page.h"

#define GMOBILE_USE_UNSTABLE_API
#include <gmobile.h>

/**
 * PtDistroPage:
 *
 * A tour page for a specific distro. A page is considered useful for
 * a certain distro if one of the distro ids on the page matches the
 * distro identifier.
 */

enum {
  PROP_0,
  PROP_DISTRO_IDS,
  PROP_LAST_PROP
};
static GParamSpec *props[PROP_LAST_PROP];

struct _PtDistroPage {
  PtPage parent;

  GStrv  distro_ids;
};

static void pt_distro_page_buildable_init (GtkBuildableIface *iface);

G_DEFINE_TYPE_WITH_CODE (PtDistroPage, pt_distro_page, PT_TYPE_PAGE,
                         G_IMPLEMENT_INTERFACE (GTK_TYPE_BUILDABLE,
                                                pt_distro_page_buildable_init))


static void
set_distro_ids (PtDistroPage *self, const char *const *ids)
{
  GStrvBuilder *builder = g_strv_builder_new ();

  /* Strip whitespace and empty elements */
  for (int i = 0; ids[i]; i++) {
    g_autofree char *id = NULL;

    id = g_strstrip (g_strdup (ids[i]));
    if (id[0] == '\0')
      continue;

    g_strv_builder_add (builder, id);
  }

  g_clear_pointer (&self->distro_ids, g_strfreev);
  self->distro_ids = g_strv_builder_end (builder);
}


static void
pt_distro_page_set_property (GObject      *object,
                             guint         property_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
  PtDistroPage *self = PT_DISTRO_PAGE (object);

  switch (property_id) {
  case PROP_DISTRO_IDS:
    g_strfreev (self->distro_ids);
    self->distro_ids = g_value_dup_boxed (value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    break;
  }
}


static void
pt_distro_page_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  PtDistroPage *self = PT_DISTRO_PAGE (object);

  switch (property_id) {
  case PROP_DISTRO_IDS:
    g_value_set_boxed (value, self->distro_ids);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    break;
  }
}


static void
pt_distro_page_finalize (GObject *object)
{
  PtDistroPage *self = PT_DISTRO_PAGE (object);

  g_clear_pointer (&self->distro_ids, g_strfreev);

  G_OBJECT_CLASS (pt_distro_page_parent_class)->finalize (object);
}


static GtkBuildableIface *parent_buildable_iface;

static void
pt_distro_page_buildable_set_buildable_property (GtkBuildable *buildable,
                                                 GtkBuilder   *builder,
                                                 const char   *name,
                                                 const GValue *value)
{
  PtDistroPage *self = PT_DISTRO_PAGE (buildable);

  if (g_str_equal (name, "distro-ids")) {
    set_distro_ids (self, g_value_get_boxed (value));
    return;
  }

  if (parent_buildable_iface->set_buildable_property)
    parent_buildable_iface->set_buildable_property (buildable, builder, name, value);
  else
    g_object_set_property (G_OBJECT (self), name, value);
}


static void
pt_distro_page_buildable_init (GtkBuildableIface *iface)
{
  parent_buildable_iface = g_type_interface_peek_parent (iface);
  iface->set_buildable_property = pt_distro_page_buildable_set_buildable_property;
}


static void
pt_distro_page_class_init (PtDistroPageClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = pt_distro_page_get_property;
  object_class->set_property = pt_distro_page_set_property;
  object_class->finalize = pt_distro_page_finalize;

  /**
   * PtDistroPage:distro-ids:
   *
   * The OS info IDs this page applies to.
   */
  props[PROP_DISTRO_IDS] =
    g_param_spec_boxed ("distro-ids", "", "",
                        G_TYPE_STRV,
                        G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties (object_class, PROP_LAST_PROP, props);
}


static void
pt_distro_page_init (PtDistroPage *self)
{
}


PtDistroPage *
pt_distro_page_new (void)
{
  return g_object_new (PT_TYPE_DISTRO_PAGE, NULL);
}


gboolean
pt_distro_page_has_id (PtDistroPage *self, const char *os_id)
{
  g_return_val_if_fail (PT_IS_DISTRO_PAGE (self), FALSE);
  g_return_val_if_fail (!gm_strv_is_null_or_empty (self->distro_ids), FALSE);

  if (gm_str_is_null_or_empty (os_id))
    return FALSE;

  return g_strv_contains ((const char * const *)self->distro_ids, os_id);
}
