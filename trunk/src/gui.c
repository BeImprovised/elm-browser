/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */
 
#include "gui.h"
#include "callbacks.h"

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
		old_x = ev->canvas.x;
		old_y = ev->canvas.y;
		evas_object_focus_set(view, EINA_FALSE);
	}
}

void event_mouse_move(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	Evas_Event_Mouse_Move *ev = event_info;
	int dx=0, dy=0;
	
	if (mouse_down == 0) return;
	else {
		dx = old_x - ev->cur.output.x;
		dy = old_y - ev->cur.output.y;

		ewk_webpage_object_scroll(page, dx, dy);

		old_x = ev->cur.output.x;
		old_y = ev->cur.output.y;
	}
}

void event_mouse_up(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	Evas_Event_Mouse_Down *ev = event_info;
	
	if (ev->button == 1) {
		mouse_down = 0;
		old_x = 0;
		old_y = 0;
		evas_object_focus_set(view, EINA_TRUE);
	}
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
   ewk_webview_object_navigation_back(view);
}

void nav_fwd(void *data, Evas_Object *obj, void *event_info)
{
   ewk_webview_object_navigation_forward(view);
}

void nav_reload(void *data, Evas_Object *obj, void *event_info)
{
   ewk_webview_object_navigation_reload(view);
}

void show_menu(void *data, Evas_Object *obj, void *event_info)
{
	elm_menu_move(data, 50, 300);
	evas_object_show(data);
}

void show_url_win(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *notify = data;

	evas_object_hide(menu);
	elm_entry_select_all(en);
	evas_object_show(notify);
}

void goto_url(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *en = data;
	char url[255];

	//TODO: Add error window for exception
	sprintf(url, "http://%s", elm_entry_entry_get(en));
	ewk_webview_object_load_url(view, url);
	evas_object_hide(url_notify);
}

void cancel_button_clicked(void *data, Evas_Object *obj, void *event_info)
{
	evas_object_hide(data);
}

void create_gui(Evas_Object *win)
{

	Elm_Menu_Item *item;
    Evas_Object *bg, *ly, *bx, *ic, *lb;
	
	//init webkit-efl
    ewk_init(e);

	//add a background
	/*bg = elm_bg_add(win);
    evas_object_size_hint_weight_set(bg, 1.0, 1.0);
    evas_object_size_hint_align_set(bg, -1.0, -1.0);
	elm_win_resize_object_add(win, bg);
    evas_object_show(bg);*/	
	
    //add the webview
    view = ewk_webview_object_add(e);
    evas_object_size_hint_weight_set(view, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(view, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_win_resize_object_add(win, view);
	evas_object_event_callback_add(view, EVAS_CALLBACK_MOUSE_DOWN, event_mouse_down, view);
	evas_object_event_callback_add(view, EVAS_CALLBACK_MOUSE_UP, event_mouse_up, view);
	evas_object_event_callback_add(view, EVAS_CALLBACK_MOUSE_MOVE, event_mouse_move, view);
	ewk_callback_title_changed_add(view, title_changed, NULL);
	ewk_callback_load_progress_add(view, load_progress, NULL);
	ewk_callback_load_started_add(view, load_started, NULL);
	ewk_callback_load_finished_add(view, load_finished, NULL);

	//set webview defaults
	ewk_webview_object_theme_set(view, "/usr/share/browser/default.edj");
    ewk_webview_object_load_url(view, "http://www.google.com/m");
	ewk_webview_object_zoom_factor_set(view, 1.0);
	
	//for scrolling
	page = ewk_webview_object_webpage_get(view);
	frame = ewk_webpage_object_mainframe_get(page);

	//add a layout
	ly = elm_layout_add(win);
	elm_layout_file_set(ly, "/usr/share/browser/browser.edj", "layout");
	evas_object_size_hint_weight_set(ly, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, ly);
	evas_object_show(ly);

	//add the view
	elm_layout_content_set(ly, "view", view);
	evas_object_show(view);

	//add a notify object
	url_notify = elm_notify_add(win);
	elm_notify_repeat_events_set(url_notify, EINA_FALSE);
	evas_object_size_hint_weight_set(url_notify, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(url_notify, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_notify_orient_set(url_notify, ELM_NOTIFY_ORIENT_LEFT);

	//add a box for notify
	bx = elm_box_add(win);
	elm_notify_content_set(url_notify, bx);
	evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, 0.0);
	evas_object_size_hint_align_set(bx, EVAS_HINT_FILL, 0.0);
	elm_box_horizontal_set(bx, 1);
	evas_object_show(bx);

	//add a label
	lb = elm_label_add(win);
	elm_label_label_set(lb, "http://");
	elm_box_pack_end(bx, lb);
	evas_object_show(lb);

	//add an entry
	en = elm_entry_add(win);
    elm_entry_single_line_set(en, 1);
	elm_entry_entry_set(en, "                                ");
	evas_object_size_hint_weight_set(en, EVAS_HINT_EXPAND, 0.0);
	evas_object_size_hint_align_set(en, EVAS_HINT_FILL, 0.5);
	elm_box_pack_end(bx, en);
	evas_object_show(en);
	    
	//add go button
	bt = elm_button_add(win);
	elm_button_label_set(bt, "Go");
	evas_object_smart_callback_add(bt, "clicked", goto_url, en);
	elm_box_pack_end(bx, bt);
	evas_object_show(bt);	

	//add cancel button
	bt = elm_button_add(win);
	elm_button_label_set(bt, "Cancel");
	evas_object_smart_callback_add(bt, "clicked", cancel_button_clicked, url_notify);
	elm_box_pack_end(bx, bt);
	evas_object_show(bt);
	    
	//add a menu
	menu = elm_menu_add(win);
	item = elm_menu_item_add(menu, NULL, NULL, "Reload", nav_reload, NULL);
	item = elm_menu_item_add(menu, NULL, NULL, "URL", show_url_win, url_notify);

	//add menu button
	menu_bt = elm_button_add(win);
	elm_object_style_set(menu_bt, "anchor");
	elm_button_label_set(menu_bt, "Menu");
	elm_layout_content_set(ly, "menu", menu_bt);
	evas_object_show(menu_bt);
	evas_object_smart_callback_add(menu_bt, "clicked", show_menu, menu);

	//add an icon
	ic = elm_icon_add(win);
	elm_icon_file_set(ic, "/usr/share/browser/icon_left_arrow.png", NULL);
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

	//add back button
	bt = elm_button_add(win);
	elm_object_style_set(bt, "anchor");
	elm_button_icon_set(bt, ic);
	elm_layout_content_set(ly, "back", bt);
	evas_object_show(bt);
	evas_object_show(ic);
	evas_object_smart_callback_add(bt, "clicked", nav_back, menu);
	    
	//add an icon
	ic = elm_icon_add(win);
	elm_icon_file_set(ic, "/usr/share/browser/icon_right_arrow.png", NULL);
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

	//add fwd button
	bt = elm_button_add(win);
	elm_object_style_set(bt, "anchor");
	elm_button_icon_set(bt, ic);
	elm_layout_content_set(ly, "fwd", bt);
	evas_object_show(bt);
	evas_object_show(ic);
	evas_object_smart_callback_add(bt, "clicked", nav_fwd, menu);
	    
	evas_object_focus_set(view, EINA_TRUE);
	    
    // make window full screen
    evas_object_resize(win, 480, 600);
}