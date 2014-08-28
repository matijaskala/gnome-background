/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Nautilus
 *
 * Copyright (C) 2000 Eazel, Inc.
 *
 * Nautilus is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Nautilus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Darin Adler <darin@bentspoon.com>
 */

/* nautilus-desktop-window.h
 */

#ifndef NAUTILUS_DESKTOP_WINDOW_H
#define NAUTILUS_DESKTOP_WINDOW_H

#include <gtk/gtk.h>

#define NAUTILUS_TYPE_DESKTOP_WINDOW nautilus_desktop_window_get_type()
#define NAUTILUS_DESKTOP_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), NAUTILUS_TYPE_DESKTOP_WINDOW, NautilusDesktopWindow))
#define NAUTILUS_DESKTOP_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), NAUTILUS_TYPE_DESKTOP_WINDOW, NautilusDesktopWindowClass))
#define NAUTILUS_IS_DESKTOP_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NAUTILUS_TYPE_DESKTOP_WINDOW))
#define NAUTILUS_IS_DESKTOP_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), NAUTILUS_TYPE_DESKTOP_WINDOW))
#define NAUTILUS_DESKTOP_WINDOW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), NAUTILUS_TYPE_DESKTOP_WINDOW, NautilusDesktopWindowClass))

typedef struct NautilusDesktopWindowDetails NautilusDesktopWindowDetails;

typedef struct {
	GtkWindow parent_spot;
	NautilusDesktopWindowDetails *details;
} NautilusDesktopWindow;

typedef struct {
	GtkWindowClass parent_spot;
} NautilusDesktopWindowClass;

GType                  nautilus_desktop_window_get_type            (void);
GtkWidget             *nautilus_desktop_window_new                 (GdkScreen             *screen);
gboolean               nautilus_desktop_window_loaded              (NautilusDesktopWindow *window);

#endif /* NAUTILUS_DESKTOP_WINDOW_H */
