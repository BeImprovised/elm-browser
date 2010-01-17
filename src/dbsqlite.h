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
 
#include <sqlite3.h>
#include <Elementary.h>

void save_state(void);
void first_run(void);
void restore_state(void);
void get_bookmark_url(char *temp, char *ty);
void add_bookmark_db(char *temp, char *ty);

sqlite3 *ventura;

extern int rotate, show_images, version, full_screen;
extern char start_page[255], user_agent[255], ventura_dir[255], home_dir[255];

Elm_Genlist_Item_Class itc_gl;
