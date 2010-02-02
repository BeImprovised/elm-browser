/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * dbus-stuff.c
 * Copyright (C) chaitanya chandel 2010 <cchandel@yahoo.com>
 * 
 * ventura is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ventura is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dbus-stuff.h"
#include <stdio.h>

void occupy_display(void)
{	
	e_dbus_init();
	conn = e_dbus_bus_get(DBUS_BUS_SYSTEM);
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.freesmartphone.ousaged",
		"/org/freesmartphone/Usage",
		"org.freesmartphone.Usage",
		"RequestResource"
	);
	
	const char *resource = "Display";
	dbus_message_append_args (msg, DBUS_TYPE_STRING, &resource, DBUS_TYPE_INVALID);

	e_dbus_message_send(conn, msg, dbus_reply_cb, -1, NULL);
	dbus_message_unref(msg);
}

void release_display(void)
{
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.freesmartphone.ousaged",
		"/org/freesmartphone/Usage",
		"org.freesmartphone.Usage",
		"ReleaseResource"
	);
	
	const char *resource = "Display";
	dbus_message_append_args (msg, DBUS_TYPE_STRING, &resource, DBUS_TYPE_INVALID);

	e_dbus_message_send(conn, msg, dbus_reply_cb, -1, NULL);
	dbus_message_unref(msg);
}

void dbus_reply_cb(void *data, DBusMessage *replymsg, DBusError *error)
{
	if (dbus_error_is_set(error)) {
		printf("Error: %s - %s\n", error->name, error->message);
	}
}