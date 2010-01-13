/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * callbacks.c
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

void back_button_clicked(void *data, Evas_Object *obj, void *event_info)
{
	if (ewk_webview_object_navigation_can_go_back(view)) ewk_webview_object_navigation_back(view);
}

void fwd_button_clicked(void *data, Evas_Object *obj, void *event_info)
{
	if (ewk_webview_object_navigation_can_go_forward(view)) ewk_webview_object_navigation_forward(view);
}