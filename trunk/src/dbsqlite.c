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
 
#include "dbsqlite.h"

/*
 * We use 2 tables for now : state and bookmarks.
 * The state table stores the values of rotate, show_images, start_page, user_agent.
 * The bookmarks table stores bookmark page name and bookmark url.
 */

void save_state(void)
{
	int db_ret;
	char *err, *sql_st=NULL, *sql;

	//clear up the state table
	sql = "DELETE FROM state";
	db_ret = sqlite3_exec(ventura, sql, NULL, NULL, &err);
	if (db_ret != SQLITE_OK) {
	  if (err != NULL) {
		  fprintf(stderr, "1 SQL error: %s\n", err);
		  sqlite3_free(err);
	  }
	}

	//save rotate, show_images, start_page, user_agent
	sql_st = sqlite3_mprintf("INSERT INTO state(version, rotate, show_images, full_screen, start_page, user_agent) VALUES(%d, %d, %d, %d, '%q', '%q')",
							 version, rotate, show_images, full_screen, start_page, user_agent);
	db_ret = sqlite3_exec(ventura, sql_st, NULL, NULL, &err);
	if (db_ret != SQLITE_OK) {
	  if (err != NULL) {
		  fprintf(stderr, "2 SQL error: %s\n", err);
		  sqlite3_free(err);
	  }
	}
	sqlite3_free(sql_st);
}

void first_run(void)
{
	char *sql, *err;
	int db_ret;

	//create tables
	sql ="CREATE TABLE state(key integer primary key, version integer, rotate integer, show_images integer, full_screen integer, start_page text, user_agent text)";
	db_ret = sqlite3_exec(ventura, sql, NULL, NULL, &err);
	if (db_ret != SQLITE_OK) {
		if (err != NULL) {
		  fprintf(stderr, "3 SQL error: %s\n", err);
		  sqlite3_free(err);
		}
	}	
	sql = "CREATE TABLE bookmarks(key INTEGER PRIMARY KEY, name text, url text)";
	db_ret = sqlite3_exec(ventura, sql, NULL, NULL, &err);
	if (db_ret != SQLITE_OK) {
		if (err != NULL) {
		  fprintf(stderr, "4 SQL error: %s\n", err);
		  sqlite3_free(err);
		}
	}
	// add default values
	sql ="INSERT INTO state(version, rotate, show_images, full_screen, start_page, user_agent) VALUES(1, 0, 1, 0, 'http://www.google.com/m', '')";
	db_ret = sqlite3_exec(ventura, sql, NULL, NULL, &err);
	if (db_ret != SQLITE_OK) {
		if (err != NULL) {
		  fprintf(stderr, "5 SQL error: %s\n", err);
		  sqlite3_free(err);
		}
	}
	sql ="INSERT INTO bookmarks(name, url) VALUES('Google', 'http://www.google.com/m')";
	db_ret = sqlite3_exec(ventura, sql, NULL, NULL, &err);
	if (db_ret != SQLITE_OK) {
		if (err != NULL) {
		  fprintf(stderr, "5 SQL error: %s\n", err);
		  sqlite3_free(err);
		}
	}
	sql ="INSERT INTO bookmarks(name, url) VALUES('Yahoo', 'http://m.yahoo.com')";
	db_ret = sqlite3_exec(ventura, sql, NULL, NULL, &err);
	if (db_ret != SQLITE_OK) {
		if (err != NULL) {
		  fprintf(stderr, "5 SQL error: %s\n", err);
		  sqlite3_free(err);
		}
	}
	    
	//set up environment values
	rotate = 0;
	show_images = 1;
    full_screen = 0;
	strcpy(start_page, "http://www.google.com/m");
	strcpy(user_agent, "");
	version = 1;
}

void modify_state(void)
{
	char *sql, *err;
	int db_ret;

	//alter table
	sql ="ALTER TABLE state ADD COLUMN full_screen integer";
	db_ret = sqlite3_exec(ventura, sql, NULL, NULL, &err);
	if (db_ret != SQLITE_OK) {
		if (err != NULL) {
		  fprintf(stderr, "3 SQL error: %s\n", err);
		  sqlite3_free(err);
		}
	}		
}

void restore_state(void)
{
	int db_ret, ret;
	char *sql, db[255];
	const char  *tail;
	sqlite3_stmt *stmt;
	
	//set ventura directory
	sprintf(ventura_dir, "/usr/share/ventura");
	
	//get the users home directory
	const char *name = "HOME";
	sprintf(home_dir, "%s", getenv(name));
	sprintf(db, "%s/.ventura/ventura.db", home_dir);
	
	//connect to the db
	db_ret = sqlite3_open(db, &ventura);
	
	if (db_ret == SQLITE_ERROR || db_ret == SQLITE_CANTOPEN) {
		printf("6 SQL error: %s\n", sqlite3_errmsg(ventura));
		printf("Creating new db file\n");
		//exit(1);
		ret = system("mkdir ~/.ventura");
		sqlite3_close(ventura);
		db_ret = sqlite3_open(db, &ventura);
	}
	
	//get the db version
	sql = "SELECT version FROM state";
	db_ret = sqlite3_prepare(ventura, sql, strlen(sql), &stmt, &tail);
	if(db_ret != SQLITE_OK) {
		if (strcmp(sqlite3_errmsg(ventura), "no such table: state")==0) first_run();
		printf("7 SQL error: %d %s\n", db_ret, sqlite3_errmsg(ventura));
	}
	else {
		db_ret = sqlite3_step(stmt);
		if (db_ret == SQLITE_ROW) {
			version = sqlite3_column_int(stmt, 0);
		}
	}
	sqlite3_finalize(stmt);
	printf("db ver %d\n", version);
	if (version < 1) modify_state();
	
	//load the state data from state table
	sql = "SELECT rotate, show_images, full_screen, start_page, user_agent FROM state";
	db_ret = sqlite3_prepare(ventura, sql, strlen(sql), &stmt, &tail);
	if(db_ret != SQLITE_OK) {
		printf("8 SQL error: %d %s\n", db_ret, sqlite3_errmsg(ventura));
	}
	else {
		//set up environment variables
		while((db_ret = sqlite3_step(stmt)) == SQLITE_ROW) {
			rotate = sqlite3_column_int(stmt, 0);
			show_images = sqlite3_column_int(stmt, 1);
			full_screen = sqlite3_column_int(stmt, 2);
			sprintf(start_page, "%s", sqlite3_column_text(stmt, 3));
			sprintf(user_agent, "%s", sqlite3_column_text(stmt, 4));
		}
	}
	sqlite3_finalize(stmt);
}

void populate_bookmarks(Evas_Object *li)
{
	int db_ret;
	char *sql, *book_mark, ty[255];
	const char  *tail;
	sqlite3_stmt *stmt;

	elm_genlist_clear(li);
	sql = "SELECT name, url FROM bookmarks";
	db_ret = sqlite3_prepare(ventura, sql, strlen(sql), &stmt, &tail);
	if(db_ret != SQLITE_OK) {
		printf("9 SQL error: %s\n", sqlite3_errmsg(ventura));
	}
	while((db_ret = sqlite3_step(stmt)) == SQLITE_ROW) {
		sprintf(ty, "%s", sqlite3_column_text(stmt, 0));
		book_mark = strdup(ty);
		elm_genlist_item_append(li, &itc_gl, (char *)book_mark, NULL, 
		                        ELM_GENLIST_ITEM_NONE, NULL, NULL);
	}
	sqlite3_finalize(stmt);
}

void get_bookmark_url(char *temp, char *ty)
{
	int db_ret;
	char *sql;
	const char  *tail;
	sqlite3_stmt *stmt;

	sql = sqlite3_mprintf("SELECT url FROM bookmarks WHERE name='%q'", temp);
	db_ret = sqlite3_prepare(ventura, sql, strlen(sql), &stmt, &tail);
	if(db_ret != SQLITE_OK) {
		printf("10 SQL error: %s\n", sqlite3_errmsg(ventura));
	}
	while((db_ret = sqlite3_step(stmt)) == SQLITE_ROW) {
		sprintf(ty, "%s", sqlite3_column_text(stmt, 0));
	}
	sqlite3_finalize(stmt);
	sqlite3_free(sql);
}

void add_bookmark_db(char *temp, char *ty)
{
	int db_ret;
	char *sql, *err;

	sql = sqlite3_mprintf("INSERT INTO bookmarks(name, url) VALUES('%q', '%q')", temp, ty);
	printf("%s\n", sql);
	db_ret = sqlite3_exec(ventura, sql, NULL, NULL, &err);
	if (db_ret != SQLITE_OK) {
		if (err != NULL) {
		  fprintf(stderr, "11 SQL error: %s\n", err);
		  sqlite3_free(err);
		}
	}
	sqlite3_free(sql);
}
