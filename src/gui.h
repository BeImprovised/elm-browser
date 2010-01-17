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

#include <Elementary.h>
#include <EWebKit.h>

extern Evas_Object *view, *page, *frame, *win, *bt;
extern Evas *e;
extern EWebKit_Hit_Test_Contents contents;
extern int x, y, mouse_down, old_x, old_y, rotate, show_images, version, req;
extern char start_page[255], user_agent[255], ventura_dir[255], home_dir[255], req_page[255];

Evas_Object *url_notify, *menu, *menu_bt, *en, *set_page_win, *rotate_ck;
Evas_Object *show_image_ck, *user_agent_en, *start_page_en, *bookmark_win;
Evas_Object *add_bookmark_win, *name_en, *url_en, *full_screen_ck;

Elm_Genlist_Item_Class itc_gl;

void create_gui(Evas_Object *win);
void populate_bookmarks(Evas_Object *li);
