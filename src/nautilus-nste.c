/*
 *  Nautilus-sendto 
 * 
 *  Copyright (C) 2004 Free Software Foundation, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Author: Roberto Majadas <roberto.majadas@openshine.com> 
 * 
 */

#include <config.h>
#include <string.h>
#include <glib/gi18n-lib.h>
#include <libgnomevfs/gnome-vfs-utils.h>
#include <libgnomevfs/gnome-vfs-file-info.h>
#include <libgnomevfs/gnome-vfs-ops.h>
#include <libnautilus-extension/nautilus-extension-types.h>
#include <libnautilus-extension/nautilus-file-info.h>
#include <libnautilus-extension/nautilus-menu-provider.h>
#include "nautilus-nste.h"


static GObjectClass *parent_class;

static void
sendto_callback (NautilusMenuItem *item,
	      gpointer          user_data)
{
	GList            *files, *scan;
	NautilusFileInfo *file;
	gchar            *uri;
	GString          *cmd;

	files = g_object_get_data (G_OBJECT (item), "files");
	file = files->data;

	cmd = g_string_new ("nautilus-sendto");

	for (scan = files; scan; scan = scan->next) {
		NautilusFileInfo *file = scan->data;

		uri = nautilus_file_info_get_uri (file);
		g_string_append_printf (cmd, " \"%s\"", uri);
		g_free (uri);
	}

	g_spawn_command_line_async (cmd->str, NULL);

	g_string_free (cmd, TRUE);
}	 

static GList *
nautilus_nste_get_file_items (NautilusMenuProvider *provider,
			      GtkWidget            *window,
			      GList                *files)
{
	GList    *items = NULL;
	GList    *scan;
	gboolean  one_item;
	NautilusMenuItem *item;

	if (files == NULL)
		return NULL;

	for (scan = files; scan; scan = scan->next) {
		NautilusFileInfo *file = scan->data;
		gchar            *scheme;
		gboolean          local;

		scheme = nautilus_file_info_get_uri_scheme (file);
		local = strncmp (scheme, "file", 4) == 0;
		g_free (scheme);

		if (!local)
			return NULL;
	}

	one_item = (files != NULL) && (files->next == NULL);
	if (one_item && 
	    !nautilus_file_info_is_directory ((NautilusFileInfo *)files->data)) {
		item = nautilus_menu_item_new ("NautilusNste::sendto",
					       _("Send to..."),
					       _("Send file by mail, instant message..."),
					       NULL);
	} else {
		item = nautilus_menu_item_new ("NautilusNste::sendto",
					       _("Send to..."),
					       _("Send files by mail, instant message..."),
					       NULL);
	}
	g_signal_connect (item, 
			  "activate",
			  G_CALLBACK (sendto_callback),
			  provider);
	g_object_set_data_full (G_OBJECT (item), 
				"files",
				nautilus_file_info_list_copy (files),
				(GDestroyNotify) nautilus_file_info_list_free);

	items = g_list_append (items, item);

	return items;
}


static void 
nautilus_nste_menu_provider_iface_init (NautilusMenuProviderIface *iface)
{
	iface->get_file_items = nautilus_nste_get_file_items;
}

static void 
nautilus_nste_init (NautilusNste *nste)
{
}

static void
nautilus_nste_class_init (NautilusNsteClass *class)
{
	parent_class = g_type_class_peek_parent (class);
}

G_DEFINE_TYPE(NautilusNste, nautilus_nste, G_TYPE_OBJECT)

