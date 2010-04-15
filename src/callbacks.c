/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * callbacks.c
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

#include "callbacks.h"
#include "gui.h"
#include "dbsqlite.h"

void zoom_out(void)
{
   float zoom = ewk_webview_object_zoom_factor_get(view);
   zoom -= 0.25;
   if (zoom < 0.25) zoom = 0.25;
   ewk_webview_object_zoom_factor_set(view, zoom);
}

void zoom_in(void)
{
   float zoom = ewk_webview_object_zoom_factor_get(view);
   zoom += 0.25;
   if (zoom > 4.0) zoom = 4.0;
   ewk_webview_object_zoom_factor_set(view, zoom);
}

void event_mouse_down(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	Evas_Event_Mouse_Down *ev = event_info;
	
	if (ev->button == 1) {
		if (ev->flags == EVAS_BUTTON_DOUBLE_CLICK) zoom_in();
		if (ev->flags == EVAS_BUTTON_TRIPLE_CLICK) zoom_out();
		mouse_down = 1;
		new_x = old_x = ev->canvas.x;
		new_y = old_y = ev->canvas.y;
		evas_object_focus_set(view, EINA_FALSE);
	}
}

void event_mouse_move(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	Evas_Event_Mouse_Move *ev = event_info;
	/*int dx=0, dy=0;*/
	
	if (mouse_down == 0) return;
	else {
          /*
		dx = old_x - ev->cur.output.x;
		dy = old_y - ev->cur.output.y;

		ewk_webpage_object_scroll(page, dx, dy);

		old_x = ev->cur.output.x;
		old_y = ev->cur.output.y;
                */
		new_x = ev->cur.output.x;
		new_y = ev->cur.output.y;
	}
}

void event_mouse_up(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	Evas_Event_Mouse_Down *ev = event_info;
	int dx=0, dy=0;
	
	if (ev->button == 1) {
		dx = old_x - new_x;
		dy = old_y - new_y;

		ewk_webpage_object_scroll(page, dx, dy);

		mouse_down = 0;
		old_x = 0;
		old_y = 0;
		evas_object_focus_set(view, EINA_TRUE);
	}
}

void url_changed(void *data, Evas_Object *obj, void *event_info)
{
	EWebKit_Event_Url_Changed *ev = event_info;
	printf("%s\n", ev->url);
}

void title_changed(void *data, Evas_Object *obj, void *event_info)
{
	const char *tit = ewk_webframe_object_title_get(frame);
	elm_win_title_set(win, tit);
}

void load_progress(void *data, Evas_Object *obj, void *event_info)
{
	char str[35];
	    
	EWebKit_Event_Load_Progress *ev = event_info;
	sprintf(str, "%d%%", ev->progress);
	elm_button_label_set(menu_bt, str); 
}

void load_started(void *data, Evas_Object *obj, void *event_info)
{
	elm_button_label_set(menu_bt, "Loading ..");
}

void load_finished(void *data, Evas_Object *obj, void *event_info)
{
	elm_button_label_set(menu_bt, "Menu");
}

void nav_back(void *data, Evas_Object *obj, void *event_info)
{
   if (ewk_webview_object_navigation_can_go_back(view)) ewk_webview_object_navigation_back(view);
}

void nav_fwd(void *data, Evas_Object *obj, void *event_info)
{
   if (ewk_webview_object_navigation_can_go_forward(view)) ewk_webview_object_navigation_forward(view);
}

void nav_up(void *data, Evas_Object *obj, void *event_info)
{
   int w,h;
   ewk_webpage_object_viewport_size_get(page, &w, &h);
   ewk_webpage_object_scroll(page, 0, -0.8*h);
}

void nav_left(void *data, Evas_Object *obj, void *event_info)
{
   int w,h;
   ewk_webpage_object_viewport_size_get(page, &w, &h);
   ewk_webpage_object_scroll(page, -0.95*w, 0);
}

void nav_right(void *data, Evas_Object *obj, void *event_info)
{
   int w,h;
   ewk_webpage_object_viewport_size_get(page, &w, &h);
   ewk_webpage_object_scroll(page, 0.95*w, 0);
}

void nav_down(void *data, Evas_Object *obj, void *event_info)
{
   int w,h;
   ewk_webpage_object_viewport_size_get(page, &w, &h);
   ewk_webpage_object_scroll(page, 0, 0.8*h);
}

void nav_stop(void *data, Evas_Object *obj, void *event_info)
{
   ewk_webview_object_navigation_stop(view);
}

void nav_reload(void *data, Evas_Object *obj, void *event_info)
{
   ewk_webview_object_navigation_reload(view);
}

void show_controls(void *data, Evas_Object *obj, void *event_info)
{
   evas_object_show(menu_bt);
   evas_object_show(back_bt);
   evas_object_show(fwd_bt);
   evas_object_show(up_bt);
   evas_object_show(down_bt);
   evas_object_show(left_bt);
   evas_object_show(right_bt);
   evas_object_hide(controls_bt);
}

void hide_controls(void *data, Evas_Object *obj, void *event_info)
{
   evas_object_hide(menu_bt);
   evas_object_hide(back_bt);
   evas_object_hide(fwd_bt);
   evas_object_hide(up_bt);
   evas_object_hide(down_bt);
   evas_object_hide(left_bt);
   evas_object_hide(right_bt);
   evas_object_show(controls_bt);
}

void show_menu(void *data, Evas_Object *obj, void *event_info)
{
	elm_menu_move(data, 50, 50);
	evas_object_show(data);
}

void show_url_win(void *data, Evas_Object *obj, void *event_info)
{
	strcpy(url_pre, "http://");
	Evas_Object *notify = data;

	evas_object_hide(menu);
	elm_entry_select_all(en);
	evas_object_show(notify);
}

void set_http(void *data, Evas_Object *obj, void *event_info)
{
	strcpy(url_pre, "http://");
}

void set_https(void *data, Evas_Object *obj, void *event_info)
{
	strcpy(url_pre, "https://");
}

void goto_url(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *en = data;
	char url[255];

	//TODO: Add error window for exception
	sprintf(url, "%s%s", url_pre, elm_entry_entry_get(en));
	printf("%s\n", url);
	ewk_webview_object_load_url(view, url);
	evas_object_hide(url_notify);
}

void cancel_button_clicked(void *data, Evas_Object *obj, void *event_info)
{
	evas_object_del(data);
}

void save_bookmark(void *data, Evas_Object *obj, void *event_info)
{
	char temp[255], ty[255];

	sprintf(temp, "%s", elm_entry_entry_get(name_en));
	sprintf(ty, "%s", elm_entry_entry_get(url_en));
	add_bookmark_db(temp, ty);
	evas_object_del(add_bookmark_win);
}
