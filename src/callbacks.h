/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * callbacks.h
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

//#include <Elementary.h>
#include <EWebKit.h>

extern Evas_Object *view, *page, *frame, *win, *bt, *url_en, *name_en, *add_bookmark_win;
extern Evas *e;
extern EWebKit_Hit_Test_Contents contents;
extern int x, y, mouse_down, old_x, old_y;

Evas_Object *url_notify, *menu, *menu_bt, *en;

void zoom_out(void);
void zoom_in(void);
void event_mouse_down(void *data, Evas *e, Evas_Object *obj, void *event_info);
void event_mouse_move(void *data, Evas *e, Evas_Object *obj, void *event_info);
void event_mouse_up(void *data, Evas *e, Evas_Object *obj, void *event_info);
void title_changed(void *data, Evas_Object *obj, void *event_info);
void load_progress(void *data, Evas_Object *obj, void *event_info);
void load_started(void *data, Evas_Object *obj, void *event_info);
void load_finished(void *data, Evas_Object *obj, void *event_info);
void nav_back(void *data, Evas_Object *obj, void *event_info);
void nav_fwd(void *data, Evas_Object *obj, void *event_info);
void nav_stop(void *data, Evas_Object *obj, void *event_info);
void nav_reload(void *data, Evas_Object *obj, void *event_info);
void show_menu(void *data, Evas_Object *obj, void *event_info);
void show_url_win(void *data, Evas_Object *obj, void *event_info);
void goto_url(void *data, Evas_Object *obj, void *event_info);
void cancel_button_clicked(void *data, Evas_Object *obj, void *event_info);
void save_bookmark(void *data, Evas_Object *obj, void *event_info);
