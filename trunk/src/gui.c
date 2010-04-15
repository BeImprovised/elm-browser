/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gui.c
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
 
#include "gui.h"
#include "callbacks.h"
#include "dbsqlite.h"

Evas_Object *url_notify, *menu, *menu_bt, *back_bt, *fwd_bt, *right_bt, *left_bt, *up_bt, *down_bt, *controls_bt, *en, *set_page_win, *rotate_ck;
Evas_Object *show_image_ck, *user_agent_en, *start_page_en, *bookmark_win;
Evas_Object *add_bookmark_win, *name_en, *url_en, *full_screen_ck;

Elm_Genlist_Item_Class itc_gl;

void toggle_full_screen(void)
{
	if(full_screen) {
		elm_win_fullscreen_set(win, 0);
		full_screen = 0;
	}
	else {
		elm_win_fullscreen_set(win, 1);
		full_screen = 1;
	}
}

void apply_settings(void *data, Evas_Object *obj, void *event_info)
{
	int i=0, len=0, set=0, rot=2, f_s=2;
	char ty[255];
	
	//get the values
	rot = elm_check_state_get(rotate_ck);
	show_images = elm_check_state_get(show_image_ck);
	f_s = elm_check_state_get(full_screen_ck);
	sprintf(start_page, "%s", elm_entry_entry_get(start_page_en));
	sprintf(ty, "%s", elm_entry_entry_get(user_agent_en));

	//apply immediate changes
	if (rot) {
		elm_win_rotation_set(win, 270);
		//to refresh window
		elm_win_borderless_set(win, 1);
		rotate = 1;
	}
	if (!rot && rotate) {
		elm_win_rotation_set(win, 0);
		//to refresh window
		elm_win_borderless_set(win, 1);
		rotate = 0;
	}

	//full screen
	if(f_s) {
		elm_win_fullscreen_set(win, 1);
		full_screen = 1;
	}
	if (!f_s && full_screen) {
		elm_win_fullscreen_set(win, 0);
		full_screen = 0;
	}
	
	if (!show_images) {
		//webview->page()->settings()->setLoadsImagesAutomatically(0);
	}
	
	//check for spaces or null in ty
	len = strlen(ty);
	if (len) {
		for (i=0; i< len; i++) {
			if (ty[i] == ' ') continue;
			else set = 1;
		}
	}
	//if ty contains many spaces
	if (!set) strcpy(ty, "");
	strcpy(user_agent, ty);

	evas_object_del(set_page_win);
}

void close_win(void *data, Evas_Object *obj, void *event_info)
{
	evas_object_del(data);
}

void goto_bookmark(void *data, Evas_Object *obj, void *event_info)
{
	
	char *temp, ty[255];
	Elm_Genlist_Item *item;
	
	item = elm_genlist_selected_item_get(data);
	temp = (char *)elm_genlist_item_data_get(item);

	get_bookmark_url(temp, ty);
	ewk_webview_object_load_url(view, ty);
	evas_object_del(bookmark_win);
}

char *gl_label_get(const void *data, Evas_Object *obj, const char *part)
{
	char *temp, *ty = (char *)data;

	temp = strdup(ty);
	return (temp);
}

Evas_Object *gl_icon_get(const void *data, Evas_Object *obj, const char *part)
{
	return NULL;
}

Eina_Bool gl_state_get(const void *data, Evas_Object *obj, const char *part)
{
   return 0;
}

void gl_del(const void *data, Evas_Object *obj)
{
}

void show_bookmarks(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *bg, *vbox, *bx, *gl;
	
	bookmark_win = elm_win_add(NULL, "bookmarks", ELM_WIN_BASIC);
	elm_win_title_set(bookmark_win, "Bookmarks");
	elm_win_autodel_set(bookmark_win, 1);
	
	//add background
	bg = elm_bg_add(bookmark_win);
	elm_win_resize_object_add(bookmark_win, bg);
	evas_object_size_hint_weight_set(bg, 1.0, 1.0);
	evas_object_show(bg);

	//add vbox
	vbox = elm_box_add(bookmark_win);
	evas_object_size_hint_weight_set(vbox, 1.0, 1.0);
	elm_win_resize_object_add(bookmark_win, vbox);
	evas_object_show(vbox);
	
	//add a genlist
	gl = elm_genlist_add(bookmark_win);
	elm_object_scale_set(gl, 1.0);
	elm_scroller_bounce_set(gl, 0 ,0);
	evas_object_size_hint_weight_set(gl, 1.0, 1.0);
	evas_object_size_hint_align_set(gl, -1.0, -1.0);
	elm_box_pack_end(vbox, gl);
	evas_object_show(gl);
	evas_object_smart_callback_add(gl, "clicked", goto_bookmark, gl);

	//genlist class defs
	itc_gl.item_style     	= "default";
	itc_gl.func.label_get 	= gl_label_get;
	itc_gl.func.icon_get  	= gl_icon_get;
	itc_gl.func.state_get 	= gl_state_get;
	itc_gl.func.del       	= gl_del;
	
	//add bx
	bx = elm_box_add(bookmark_win);
    elm_box_horizontal_set(bx, 1);
	elm_box_homogenous_set(bx, 1);
	evas_object_size_hint_weight_set(bx, 1.0, 0.0);
	evas_object_size_hint_align_set(bx, -1.0, 1.0);	
	elm_box_pack_end(vbox, bx);
	evas_object_show(bx);

	//add Apply button
	bt = elm_button_add(bookmark_win);
	elm_button_label_set(bt, "Go");
	evas_object_size_hint_weight_set(bt, 1.0, 0.0);
	evas_object_size_hint_align_set(bt, -1.0, 0.0);	
	elm_box_pack_end(bx, bt);
	evas_object_show(bt);
	evas_object_smart_callback_add(bt, "clicked", goto_bookmark, gl);

	//add Done button
	bt = elm_button_add(bookmark_win);
	elm_button_label_set(bt, "Cancel");
	evas_object_size_hint_weight_set(bt, 1.0, 0.0);
	evas_object_size_hint_align_set(bt, -1.0, 0.0);    
	elm_box_pack_end(bx, bt);
	evas_object_show(bt);
	evas_object_smart_callback_add(bt, "clicked", close_win, bookmark_win);

	//populate the bookmarks list
	populate_bookmarks(gl);
	
	evas_object_resize(bookmark_win, 480, 600);
	evas_object_show(bookmark_win);
	if (rotate) elm_win_rotation_set(bookmark_win, 270);
}

void show_settings_page(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *bg, *vbox, *tb, *lb, *bx, *sc;
	
	set_page_win = elm_win_add(NULL, "set", ELM_WIN_BASIC);
	elm_win_title_set(set_page_win, "Settings");
	elm_win_autodel_set(set_page_win, 1);
	
	//add background
	bg = elm_bg_add(set_page_win);
	elm_win_resize_object_add(set_page_win, bg);
	evas_object_size_hint_weight_set(bg, 1.0, 1.0);
	evas_object_show(bg);

	//add vbox
	vbox = elm_box_add(set_page_win);
	evas_object_size_hint_weight_set(vbox, 1.0, 1.0);
	evas_object_size_hint_align_set(vbox, -1.0, -1.0);
	elm_win_resize_object_add(set_page_win, vbox);
	evas_object_show(vbox);

	//add table
	tb = elm_table_add(set_page_win);
	elm_win_resize_object_add(set_page_win, tb);
	evas_object_size_hint_weight_set(tb, 1.0, 1.0);
	evas_object_size_hint_align_set(tb, -1.0, -1.0);
	elm_box_pack_end(vbox, tb);
	evas_object_show(tb);

	//add labels
	lb = elm_label_add(set_page_win);
	elm_label_label_set(lb, "Rotate");
	evas_object_size_hint_align_set(lb, 0.0, 0.5);
	elm_table_pack(tb, lb, 0, 0, 1, 1);
	evas_object_show(lb);

	lb = elm_label_add(set_page_win);
	elm_label_label_set(lb, "Full Screen");
	evas_object_size_hint_align_set(lb, 0.0, 0.5);
	elm_table_pack(tb, lb, 0, 1, 1, 1);
	evas_object_show(lb);
	
	lb = elm_label_add(set_page_win);
	elm_label_label_set(lb, "Show Images");
	evas_object_size_hint_align_set(lb, 0.0, 0.5);
	elm_table_pack(tb, lb, 0, 2, 1, 1);
	evas_object_show(lb);

	lb = elm_label_add(set_page_win);
	elm_label_label_set(lb, "Start Page");
	evas_object_size_hint_align_set(lb, 0.0, 0.5);
	elm_table_pack(tb, lb, 0, 3, 1, 1);
	evas_object_show(lb);

	lb = elm_label_add(set_page_win);
	elm_label_label_set(lb, "User Agent");
	evas_object_size_hint_align_set(lb, 0.0, 0.5);
	elm_table_pack(tb, lb, 0, 4, 1, 1);
	evas_object_show(lb);

	//add rotate checkbox
	rotate_ck = elm_check_add(set_page_win);
	elm_check_state_set(rotate_ck, rotate);
	elm_table_pack(tb, rotate_ck, 1, 0, 1, 1);
	evas_object_show(rotate_ck);

	//add full screen checkbox
	full_screen_ck = elm_check_add(set_page_win);
	elm_check_state_set(full_screen_ck, full_screen);
	elm_table_pack(tb, full_screen_ck, 1, 1, 1, 1);
	evas_object_show(full_screen_ck);
	
	//add show images checkbox
	show_image_ck = elm_check_add(set_page_win);
	elm_check_state_set(show_image_ck, show_images);
	elm_table_pack(tb, show_image_ck, 1, 2, 1, 1);
	evas_object_show(show_image_ck);
	
	//add start page entry
	start_page_en = elm_entry_add(set_page_win);
    elm_entry_single_line_set(start_page_en, 1);
	elm_entry_entry_set(start_page_en, start_page);
	elm_table_pack(tb, start_page_en, 1, 3, 1, 1);
	evas_object_show(start_page_en);

	//add scroller for user agent entry
	sc = elm_scroller_add(set_page_win);
	elm_scroller_content_min_limit(sc, 0, 1);
	elm_scroller_policy_set(sc, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
	elm_scroller_bounce_set(sc, 0, 0);
	evas_object_size_hint_weight_set(sc, EVAS_HINT_EXPAND, 0.0);
	evas_object_size_hint_align_set(sc, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_table_pack(tb, sc, 1, 4, 1, 1);
	
	//add user agent entry
	user_agent_en = elm_entry_add(set_page_win);
    elm_entry_single_line_set(user_agent_en, 1);
	if(strlen(user_agent) > 0) elm_entry_entry_set(user_agent_en, user_agent);
	else elm_entry_entry_set(user_agent_en, "Firefox/3.5");
	evas_object_size_hint_weight_set(user_agent_en, EVAS_HINT_EXPAND, 0.0);
	evas_object_size_hint_align_set(user_agent_en, EVAS_HINT_FILL, 0.5);
	elm_scroller_content_set(sc, user_agent_en);
	evas_object_show(user_agent_en);
	evas_object_show(sc);

	//add bx
	bx = elm_box_add(set_page_win);
    elm_box_horizontal_set(bx, 1);
	elm_box_homogenous_set(bx, 1);
	evas_object_size_hint_weight_set(bx, 1.0, 1.0);
	evas_object_size_hint_align_set(bx, -1.0, 1.0);	
	elm_box_pack_end(vbox, bx);
	evas_object_show(bx);

	//add Apply button
	bt = elm_button_add(set_page_win);
	elm_button_label_set(bt, "Apply");
	evas_object_size_hint_weight_set(bt, 1.0, 1.0);
	evas_object_size_hint_align_set(bt, -1.0, 0.0);	
	elm_box_pack_end(bx, bt);
	evas_object_show(bt);
	evas_object_smart_callback_add(bt, "clicked", apply_settings, NULL);

	//add Done button
	bt = elm_button_add(set_page_win);
	elm_button_label_set(bt, "Cancel");
	evas_object_size_hint_weight_set(bt, 1.0, 1.0);
	evas_object_size_hint_align_set(bt, -1.0, 0.0);    
	elm_box_pack_end(bx, bt);
	evas_object_show(bt);
	evas_object_smart_callback_add(bt, "clicked", close_win, set_page_win);
	    
	evas_object_resize(set_page_win, 480, 600);
	evas_object_show(set_page_win);
	if (rotate) elm_win_rotation_set(set_page_win, 270);
}

void add_bookmark(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *bg, *vbox, *tb, *lb, *bx, *inwin;
	
	add_bookmark_win = elm_win_add(NULL, "add", ELM_WIN_BASIC);
	elm_win_title_set(add_bookmark_win, "Add Bookmark");
	elm_win_autodel_set(add_bookmark_win, 1);
	
	//add background
	bg = elm_bg_add(add_bookmark_win);
	elm_win_resize_object_add(add_bookmark_win, bg);
	evas_object_size_hint_weight_set(bg, 1.0, 1.0);
	evas_object_show(bg);

	//add inwin
	inwin = elm_win_inwin_add(add_bookmark_win);
	elm_object_style_set(inwin, "minimal_vertical");
	elm_win_resize_object_add(add_bookmark_win, inwin);
	evas_object_show(inwin);

	//add vbox
	vbox = elm_box_add(add_bookmark_win);
	evas_object_size_hint_weight_set(vbox, 1.0, 1.0);
	evas_object_size_hint_align_set(vbox, -1.0, -1.0);
	elm_win_inwin_content_set(inwin, vbox);
	evas_object_show(vbox);

	//add table
	tb = elm_table_add(add_bookmark_win);
	elm_win_resize_object_add(add_bookmark_win, tb);
	evas_object_size_hint_weight_set(tb, 1.0, 1.0);
	evas_object_size_hint_align_set(tb, -1.0, -1.0);
	elm_box_pack_end(vbox, tb);
	evas_object_show(tb);

	//add labels
	lb = elm_label_add(add_bookmark_win);
	elm_label_label_set(lb, "Name");
	evas_object_size_hint_align_set(lb, 0.0, 0.5);
	elm_table_pack(tb, lb, 0, 0, 1, 1);
	evas_object_show(lb);

	lb = elm_label_add(add_bookmark_win);
	elm_label_label_set(lb, "URL");
	evas_object_size_hint_align_set(lb, 0.0, 0.5);
	elm_table_pack(tb, lb, 0, 1, 1, 1);
	evas_object_show(lb);

	//add name entry
	name_en = elm_entry_add(add_bookmark_win);
    elm_entry_single_line_set(name_en, 1);
	elm_entry_entry_set(name_en, ewk_webframe_object_title_get(frame));
	elm_table_pack(tb, name_en, 1, 0, 1, 1);
	evas_object_show(name_en);

	//add url entry
	url_en = elm_entry_add(add_bookmark_win);
    elm_entry_single_line_set(url_en, 1);
	elm_entry_entry_set(url_en, ewk_webframe_object_url_get(frame));
	elm_table_pack(tb, url_en, 1, 1, 1, 1);
	evas_object_show(url_en);

	//add bx
	bx = elm_box_add(add_bookmark_win);
    elm_box_horizontal_set(bx, 1);
	elm_box_homogenous_set(bx, 1);
	evas_object_size_hint_weight_set(bx, 1.0, 1.0);
	evas_object_size_hint_align_set(bx, -1.0, 1.0);	
	elm_box_pack_end(vbox, bx);
	evas_object_show(bx);

	//add save button
	bt = elm_button_add(add_bookmark_win);
	elm_button_label_set(bt, "Save");
	evas_object_size_hint_weight_set(bt, 1.0, 1.0);
	evas_object_size_hint_align_set(bt, -1.0, 0.0);	
	elm_box_pack_end(bx, bt);
	evas_object_show(bt);
	evas_object_smart_callback_add(bt, "clicked", save_bookmark, NULL);

	//add Done button
	bt = elm_button_add(add_bookmark_win);
	elm_button_label_set(bt, "Cancel");
	evas_object_size_hint_weight_set(bt, 1.0, 1.0);
	evas_object_size_hint_align_set(bt, -1.0, 0.0);    
	elm_box_pack_end(bx, bt);
	evas_object_show(bt);
	evas_object_smart_callback_add(bt, "clicked", close_win, add_bookmark_win);
	    
	evas_object_resize(add_bookmark_win, 480, 600);
	evas_object_show(add_bookmark_win);
	if (rotate) elm_win_rotation_set(add_bookmark_win, 270);
}

void create_gui(Evas_Object *win)
{

	Elm_Menu_Item *item;
    Evas_Object *ly, *bx, *ic, *hs;
	
	//init webkit-efl
    ewk_init(e);
	
    //add the webview
    view = ewk_webview_object_add(e);
    evas_object_size_hint_weight_set(view, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(view, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_win_resize_object_add(win, view);
	evas_object_event_callback_add(view, EVAS_CALLBACK_MOUSE_DOWN, event_mouse_down, view);
	evas_object_event_callback_add(view, EVAS_CALLBACK_MOUSE_UP, event_mouse_up, view);
	evas_object_event_callback_add(view, EVAS_CALLBACK_MOUSE_MOVE, event_mouse_move, view);
	ewk_callback_url_changed_add(view, url_changed, NULL);
	ewk_callback_title_changed_add(view, title_changed, NULL);
	ewk_callback_load_progress_add(view, load_progress, NULL);
	ewk_callback_load_started_add(view, load_started, NULL);
	ewk_callback_load_finished_add(view, load_finished, NULL);

	//set webview defaults
	ewk_webview_object_theme_set(view, "/usr/share/webkit-1.0/theme/default.edj");
	if (req) ewk_webview_object_load_url(view, req_page);
	else ewk_webview_object_load_url(view, start_page);
	ewk_webview_object_zoom_factor_set(view, 1.0);
	
	//for scrolling
	page = ewk_webview_object_webpage_get(view);
	frame = ewk_webpage_object_mainframe_get(page);

	//add a layout
	ly = elm_layout_add(win);
	elm_layout_file_set(ly, "/usr/share/ventura/ventura.edj", "layout");
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

	//add a hoversel
	hs = elm_hoversel_add(win);
	elm_hoversel_hover_parent_set(hs, win);
	elm_hoversel_label_set(hs, "http://");
	elm_hoversel_item_add(hs, "http://", NULL, ELM_ICON_NONE, set_http, NULL);
	elm_hoversel_item_add(hs, "https://", NULL, ELM_ICON_NONE, set_https, NULL);
	elm_box_pack_end(bx, hs);
	evas_object_show(hs);

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
	    //Bookmarks
	//	item = elm_menu_item_add(menu, NULL, NULL, "Bookmarks", NULL, NULL);
	elm_menu_item_add(menu, NULL, NULL, "Bookmarks", show_bookmarks, NULL);
    elm_menu_item_add(menu, NULL, NULL, "+Bookmark", add_bookmark, NULL);
	    //page menu
	item = elm_menu_item_add(menu, NULL, NULL, "Page", NULL, NULL);
	elm_menu_item_add(menu, item, NULL, "Reload", nav_reload, NULL);
	elm_menu_item_add(menu, item, NULL, "Stop", nav_stop, NULL);
	    //url
	elm_menu_item_add(menu, NULL, NULL, "URL", show_url_win, url_notify);
	    //full screen
	elm_menu_item_add(menu, NULL, NULL, "Full Screen", (void *)toggle_full_screen, NULL);
	    //zoom out
	elm_menu_item_add(menu, NULL, NULL, "Zoom Out", (void *)zoom_out, NULL);
	    //Settings
	elm_menu_item_add(menu, NULL, NULL, "Settings", show_settings_page, NULL);
	elm_menu_item_add(menu, NULL, NULL, "Hide Controls", hide_controls, ly);

	//add menu button
	menu_bt = elm_button_add(win);
	elm_object_style_set(menu_bt, "anchor");
	elm_button_label_set(menu_bt, "Menu");
	elm_layout_content_set(ly, "menu", menu_bt);
	evas_object_show(menu_bt);
	evas_object_smart_callback_add(menu_bt, "clicked", show_menu, menu);

	//add an icon
	ic = elm_icon_add(win);
	elm_icon_file_set(ic, "/usr/share/ventura/icon_left_arrow.png", NULL);
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

	//add back button
	back_bt = elm_button_add(win);
	elm_object_style_set(back_bt, "anchor");
	elm_button_icon_set(back_bt, ic);
	elm_layout_content_set(ly, "back", back_bt);
	evas_object_show(back_bt);
	evas_object_show(ic);
	evas_object_smart_callback_add(back_bt, "clicked", nav_back, menu);
	    
	//add an icon
	ic = elm_icon_add(win);
	elm_icon_file_set(ic, "/usr/share/ventura/icon_right_arrow.png", NULL);
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

	//add fwd button
	fwd_bt = elm_button_add(win);
	elm_object_style_set(fwd_bt, "anchor");
	elm_button_icon_set(fwd_bt, ic);
	elm_layout_content_set(ly, "fwd", fwd_bt);
	evas_object_show(fwd_bt);
	evas_object_show(ic);
	evas_object_smart_callback_add(fwd_bt, "clicked", nav_fwd, menu);
	    
	//add an icon
	ic = elm_icon_add(win);
	elm_icon_file_set(ic, "/usr/share/ventura/icon_up_arrow.png", NULL);
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

	//add up button
	up_bt = elm_button_add(win);
	elm_object_style_set(up_bt, "anchor");
	elm_button_icon_set(up_bt, ic);
	elm_layout_content_set(ly, "up", up_bt);
	evas_object_show(up_bt);
	evas_object_show(ic);
	evas_object_smart_callback_add(up_bt, "clicked", nav_up, menu);

	//add an icon
	ic = elm_icon_add(win);
	elm_icon_file_set(ic, "/usr/share/ventura/icon_down_arrow.png", NULL);
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

	//add down button
	down_bt = elm_button_add(win);
	elm_object_style_set(down_bt, "anchor");
	elm_button_icon_set(down_bt, ic);
	elm_layout_content_set(ly, "down", down_bt);
	evas_object_show(down_bt);
	evas_object_show(ic);
	evas_object_smart_callback_add(down_bt, "clicked", nav_down, menu);

	//add an icon
	ic = elm_icon_add(win);
	elm_icon_file_set(ic, "/usr/share/ventura/icon_left_arrow.png", NULL);
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

	//add left button
	left_bt = elm_button_add(win);
	elm_object_style_set(left_bt, "anchor");
	elm_button_icon_set(left_bt, ic);
	elm_layout_content_set(ly, "left", left_bt);
	evas_object_show(left_bt);
	evas_object_show(ic);
	evas_object_smart_callback_add(left_bt, "clicked", nav_left, menu);

	//add an icon
	ic = elm_icon_add(win);
	elm_icon_file_set(ic, "/usr/share/ventura/icon_right_arrow.png", NULL);
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

	//add right button
	right_bt = elm_button_add(win);
	elm_object_style_set(right_bt, "anchor");
	elm_button_icon_set(right_bt, ic);
	elm_layout_content_set(ly, "right", right_bt);
	evas_object_show(right_bt);
	evas_object_show(ic);
	evas_object_smart_callback_add(right_bt, "clicked", nav_right, menu);

	//add an icon
	ic = elm_icon_add(win);
	elm_icon_file_set(ic, "/usr/share/ventura/icon_down_arrow.png", NULL);
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

	//add controls button
    controls_bt = elm_button_add(win);
	elm_object_style_set(controls_bt, "anchor");
	elm_button_icon_set(controls_bt, ic);
	elm_layout_content_set(ly, "controls", controls_bt);
	evas_object_hide(ic);
	evas_object_hide(controls_bt);
	evas_object_smart_callback_add(controls_bt, "clicked", show_controls, NULL);

	evas_object_focus_set(view, EINA_TRUE);
	    
    // make window full screen
    //evas_object_resize(win, 480, 600);
}
