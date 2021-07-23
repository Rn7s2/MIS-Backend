/******************************************************************************
 * Filename: getHistoryByID.c
 * Author:   雷瑞祺
 * License:  MIT License
 * Purpose:  handle get history by ID http request
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <glib-object.h>
#include <json-glib/json-glib.h>

#define MAX_LEN 20001

int main()
{
    char *post;

    JsonParser *parser;
    GError *error = NULL;
    JsonReader *reader;

    gint64 id;
    gchar *start_date, *end_date;

    sqlite3 *db;
    int rc;
    char *sql;

    post = malloc(MAX_LEN * sizeof(char));
    fgets(post, MAX_LEN, stdin);

    parser = json_parser_new();

    json_parser_load_from_data(parser, post, -1, &error);

    free(post);

    if(error) {
        fputs(error->message, stderr);
        g_error_free(error);
        return 1;
    }

    reader = json_reader_new(json_parser_get_root(parser));
    g_object_unref(parser);

    json_reader_read_member(reader, "id");
    id = json_reader_get_int_value(reader);
    json_reader_end_member(reader);

    json_reader_read_member(reader, "start");
    start_date = json_reader_get_string_value(reader);
    json_reader_end_member(reader);

    json_reader_read_member(reader, "end");
    start_date = json_reader_get_string_value(reader);
    json_reader_end_member(reader);

    rc = sqlite3_open("/home/www/mis.db" , &db);

    if(rc) {
        puts(sqlite3_errmsg(db));
        return 1;
    }

    sql = malloc(MAX_LEN * sizeof(char));


    // h这里得合并一下
    sprintf(sql,
            "SELECT");

    free(sql);
    
    g_object_unref(reader);
    
    return 0;
}
