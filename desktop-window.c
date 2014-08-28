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

#include "desktop-window.h"

#include <X11/Xatom.h>
#include <gdk/gdkx.h>
#include <glib/gi18n.h>

struct NautilusDesktopWindowDetails {
	gulong size_changed_id;
};

G_DEFINE_TYPE (NautilusDesktopWindow, nautilus_desktop_window, 
	       GTK_TYPE_WINDOW);

static void
nautilus_desktop_window_constructed (GObject *obj)
{
	AtkObject *accessible;
	NautilusDesktopWindow *window = NAUTILUS_DESKTOP_WINDOW (obj);

	G_OBJECT_CLASS (nautilus_desktop_window_parent_class)->constructed (obj);

	/* Set the accessible name so that it doesn't inherit the cryptic desktop URI. */
	accessible = gtk_widget_get_accessible (GTK_WIDGET (window));

	if (accessible) {
		atk_object_set_name (accessible, _("Desktop"));
	}
}

static void
nautilus_desktop_window_init (NautilusDesktopWindow *window)
{
	window->details = G_TYPE_INSTANCE_GET_PRIVATE (window, NAUTILUS_TYPE_DESKTOP_WINDOW,
						       NautilusDesktopWindowDetails);

	gtk_window_move (GTK_WINDOW (window), 0, 0);

	/* shouldn't really be needed given our semantic type
	 * of _NET_WM_TYPE_DESKTOP, but why not
	 */
	gtk_window_set_resizable (GTK_WINDOW (window),
				  FALSE);
	gtk_window_set_decorated (GTK_WINDOW (window),
				  FALSE);

	g_object_set_data (G_OBJECT (window), "is_desktop_window", 
			   GINT_TO_POINTER (1));
	gtk_window_set_title (GTK_WINDOW (window), _("Desktop"));
	gtk_window_set_icon_name (GTK_WINDOW (window), "user-desktop");
}

static void
nautilus_desktop_window_screen_size_changed (GdkScreen             *screen,
					     NautilusDesktopWindow *window)
{
	int width_request, height_request;

	width_request = gdk_screen_get_width (screen);
	height_request = gdk_screen_get_height (screen);
	
	g_object_set (window,
		      "width_request", width_request,
		      "height_request", height_request,
		      NULL);
}

GtkWidget *
nautilus_desktop_window_new (GdkScreen      *screen)
{
	GtkWidget *window;
	int width_request, height_request;
        GdkRGBA transparent = {0, 0, 0, 0};

	width_request = gdk_screen_get_width (screen);
	height_request = gdk_screen_get_height (screen);

	window = g_object_new (NAUTILUS_TYPE_DESKTOP_WINDOW,
			       "width_request", width_request,
			       "height_request", height_request,
			       "screen", screen,
			       NULL);

	/* Special sawmill setting*/
	gtk_window_set_wmclass (GTK_WINDOW (window), "desktop_window", "Nautilus");

        gtk_widget_override_background_color (window, 0, &transparent);


	return window;
}

static gboolean
nautilus_desktop_window_delete_event (GtkWidget *widget,
				      GdkEventAny *event)
{
	/* Returning true tells GTK+ not to delete the window. */
	return TRUE;
}

static void
map (GtkWidget *widget)
{
	/* Chain up to realize our children */
	GTK_WIDGET_CLASS (nautilus_desktop_window_parent_class)->map (widget);
	gdk_window_lower (gtk_widget_get_window (widget));
}

static void
unrealize (GtkWidget *widget)
{
	NautilusDesktopWindow *window;
	NautilusDesktopWindowDetails *details;

	window = NAUTILUS_DESKTOP_WINDOW (widget);
	details = window->details;

	if (details->size_changed_id != 0) {
		g_signal_handler_disconnect (gtk_window_get_screen (GTK_WINDOW (window)),
					     details->size_changed_id);
		details->size_changed_id = 0;
	}

	GTK_WIDGET_CLASS (nautilus_desktop_window_parent_class)->unrealize (widget);
}

static void
set_wmspec_desktop_hint (GdkWindow *window)
{
	GdkAtom atom;

	atom = gdk_atom_intern ("_NET_WM_WINDOW_TYPE_DESKTOP", FALSE);
        
	gdk_property_change (window,
			     gdk_atom_intern ("_NET_WM_WINDOW_TYPE", FALSE),
			     gdk_x11_xatom_to_atom (XA_ATOM), 32,
			     GDK_PROP_MODE_REPLACE, (guchar *) &atom, 1);
}

static void
realize (GtkWidget *widget)
{
	NautilusDesktopWindow *window;
	NautilusDesktopWindowDetails *details;

	window = NAUTILUS_DESKTOP_WINDOW (widget);
	details = window->details;

	/* Make sure we get keyboard events */
	gtk_widget_set_events (widget, gtk_widget_get_events (widget) 
			      | GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);
			      
	/* Do the work of realizing. */
	GTK_WIDGET_CLASS (nautilus_desktop_window_parent_class)->realize (widget);

	/* This is the new way to set up the desktop window */
	set_wmspec_desktop_hint (gtk_widget_get_window (widget));

	details->size_changed_id =
		g_signal_connect (gtk_window_get_screen (GTK_WINDOW (window)), "size-changed",
				  G_CALLBACK (nautilus_desktop_window_screen_size_changed), window);
}

static void
nautilus_desktop_window_class_init (NautilusDesktopWindowClass *klass)
{
	GtkWidgetClass *wclass = GTK_WIDGET_CLASS (klass);
	GObjectClass *oclass = G_OBJECT_CLASS (klass);

	oclass->constructed = nautilus_desktop_window_constructed;

	wclass->realize = realize;
	wclass->unrealize = unrealize;
	wclass->map = map;
	wclass->delete_event = nautilus_desktop_window_delete_event;

	g_type_class_add_private (klass, sizeof (NautilusDesktopWindowDetails));
}
