/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

/* 
 * Copyright (C) 2006 Peter Enseleit
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more av.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author:  Peter Enseleit <penseleit@gmail.com>
 *          Roberto Majadas <telemaco@openshine.com>
 */

#include "config.h"
#include "../nautilus-sendto-plugin.h"
#include <string.h>
#include <glib/gi18n-lib.h>

static GHashTable *hash = NULL;

static 
gboolean init (NstPlugin *plugin)
{
	gchar *t_cmd;

	printf ("Init thunderbird plugin\n");
	hash = g_hash_table_new (g_str_hash, g_str_equal);

	t_cmd = g_find_program_in_path ("mozilla-thunderbird");
	if (t_cmd == NULL)
		return FALSE;
	return TRUE;
}

static
GtkWidget* get_contacts_widget (NstPlugin *plugin)
{
		
	GtkWidget *entry;
	
	entry = gtk_entry_new ();
	
	return entry;
}

static
gboolean send_files (NstPlugin *plugin, GtkWidget *contact_widget,
			GList *file_list)
{
	gchar *t_cmd, *cmd, *send_to;
	GList *l;
	GString *mailto;

	send_to = (gchar *) gtk_entry_get_text (GTK_ENTRY(contact_widget));
		
	if (send_to == NULL || strlen (send_to) == 0)
	{
		mailto = g_string_new("-compose ");
	}
	else
	{
		mailto = g_string_new("-compose ");
		g_string_append_printf (mailto, "to=%s,", send_to);		
	}
	
	t_cmd = g_find_program_in_path ("mozilla-thunderbird");
	if (t_cmd == NULL)
		return FALSE;
	
	g_string_append_printf (mailto," attachment='\"%s\"'",file_list->data);
	for (l = file_list->next ; l; l=l->next){				
		g_string_append_printf (mailto,",'\"%s\"'",l->data);
	}
	cmd = g_strdup_printf ("%s %s", t_cmd, mailto->str);
	g_spawn_command_line_async (cmd, NULL);
	g_free (cmd);
	g_string_free (mailto, TRUE);
	g_free (t_cmd);
	return TRUE;
}

static 
gboolean destroy (NstPlugin *plugin){
	return TRUE;
}

static 
NstPluginInfo plugin_info = {
	"stock_mail",
	"thunderbird",
	N_("Email (Thunderbird)"),
	FALSE,
	init,
	get_contacts_widget,
	NULL,
	send_files,
	destroy
}; 

NST_INIT_PLUGIN (plugin_info)
