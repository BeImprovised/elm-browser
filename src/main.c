/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) chaitanya chandel 2010 <cchandel@yahoo.com>
 * 
 * browser is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * browser is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "callbacks.h"
#include "gui.h"
#include "dbsqlite.h"

//globals
Ecore_X_Window my_win, focus_win;
Ecore_Timer *timer = NULL, *timer1 = NULL;
Evas_Object *win, *view, *page, *frame, *bt;
Evas *e;
EWebKit_Hit_Test_Contents contents;
int x=0, y=0, mouse_down=0, old_x=0, old_y=0, rotate=0, show_images=0, version=0, full_screen=0, req=0;
char start_page[255], user_agent[255], browser_dir[255], home_dir[255], req_page[255];

static void
my_win_del(void *data, Evas_Object *obj, void *event_info)
{
	save_state();
	//e_dbus_connection_close(conn);
	sqlite3_close(browser);

    //close webkit
    ewk_shutdown();

	elm_exit();
}

EAPI int
elm_main(int argc, char **argv)
{
	if (argc > 1) {
		strcpy(req_page, argv[1]);
		req = 1;
	}
	//restore state
	printf("restoring state\n");
	restore_state();

	//set up win
	printf("showing window\n");
	win = elm_win_add(NULL, "elm-eve", ELM_WIN_BASIC);
	elm_win_title_set(win, "Ventura");
	evas_object_smart_callback_add(win, "delete-request", my_win_del, NULL);

    e = evas_object_evas_get(win);

	// show the window
	create_gui(win);
	evas_object_show(win);

	if(rotate) {
		elm_win_rotation_set(win, 270);
		elm_win_borderless_set(win, 1);
	}

	//init dbus 
	//init_dbus_stuff();

	elm_run();
	ecore_main_loop_quit();
	elm_shutdown();

	return 0;
}
ELM_MAIN()
