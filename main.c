/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * GNOME Background: Shows desktop background
 *
 * Copyright (C) 2014 Matija Skala <mskala@gmx.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Authors: Matija Skala <mskala@gmx.com>
 */


#include "desktop-background.h"
#include "desktop-window.h"

int main(int argc, char** argv)
{
	gtk_init(&argc, &argv);
	GdkScreen* screen = gdk_screen_get_default ();
	GtkWidget* desktop = nautilus_desktop_window_new (screen);
	/*NautilusDesktopBackground* background = */nautilus_desktop_background_new (desktop);
	gtk_widget_show (desktop);
	gtk_main();
	return 0;
}
