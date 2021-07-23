/******************************************************************************
 * Filename: increaseMedicine.c
 * Author:   雷瑞祺
 * License:  MIT License
 * Purpose:  handle increase medicine http request
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <glib-object.h>
#include <json-glib/json-glib.h>

#define MAX_LEN 2001

int main(int argc, char **argv)
{
    FILE *ferr;
    
    char* tmp = NULL;
    char* post = NULL;

    JsonParser *parser;
    GError *error;
    JsonReader *reader;

    gint64 id, number;
    
    sqlite3 *db;
    char* sql = NULL;
    char* err_msg = NULL;
    int rc;

    ferr = fopen("/home/www/err.txt", "w");
    
    // now read post data from stdin
    post = malloc(MAX_LEN * sizeof(char));
    fgets(post, MAX_LEN, stdin);
    
    // parse the data
    parser = json_parser_new();
    error = NULL;
    json_parser_load_from_data(parser, post, -1, &error);

    free(post);
    
    if(error) {
        fprintf(ferr, "Parse failed: %s\n", error->message);
        g_error_free(error);
        g_object_unref(parser);
        return 1;
    }

    // use reader to get elements
    reader = json_reader_new(json_parser_get_root(parser));

    json_reader_read_member(reader, "id");
    id = json_reader_get_int_value(reader);
    json_reader_end_member(reader);
    json_reader_read_member(reader, "number");
    number = json_reader_get_int_value(reader);
    json_reader_end_member(reader);

    g_object_unref(reader);
    g_object_unref(parser);
    
    // open database
    rc = sqlite3_open("/home/www/mis.db", &db);

    if(rc) {
        fprintf(ferr, "Failed to open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // prepare sql statement
    tmp = malloc(MAX_LEN * sizeof(char));
    sql = malloc(MAX_LEN * sizeof(char));
    sql[0] = '\0';
    strcat(sql, "UPDATE medicine SET number=");
    sprintf(tmp, "%ld", number);
    strcat(sql, tmp);
    strcat(sql, "+number WHERE id=");
    sprintf(tmp, "%ld", id);
    strcat(sql, tmp);
    strcat(sql, ";INSERT INTO m_");
    strcat(sql, tmp);
    strcat(sql, " VALUES (date('now'), ");
    sprintf(tmp, "%ld", number);
    strcat(sql, tmp);
    strcat(sql, ")");
    
    free(tmp);    
    
    // run query
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

    if(rc) {
        fprintf(ferr, "Failed to eval SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }

    free(sql);
    sqlite3_close(db);

    // send response
    puts("Content-type: text/html\n");

    fclose(ferr);
    return 0;
}
