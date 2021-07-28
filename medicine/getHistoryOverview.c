/******************************************************************************
 * Filename: getHistoryOverview.c
 * Author:   雷瑞祺
 * License:  MIT License
 * Purpose:  handle get history overview http request
 ******************************************************************************/

#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <glib-object.h>
#include <json-glib/json-glib.h>

#define MAX_LEN 30001

int get_max_id(void* data, int col_count,
               char** col_contents, char** col_names)
{
    *(int*)data = atoi(col_contents[0]);
    return 0;
}

int get_medicine_data(void* data, int col_count,
                      char** col_contents, char** col_names)
{
    struct {
        char *name;
        char *code;
        double price;
    } *medicine = data;

    strcpy(medicine->name, col_contents[0]);
    strcpy(medicine->code, col_contents[1]);
    medicine->price = atof(col_contents[2]);
    
    return 0;
}

int get_history_data(void *data, int col_count,
                     char** col_contents, char** col_names)
{
    struct {
        int total_in;
        int total_out;
    } *history = data;

    int number = atoi(col_contents[1]);

    if(number > 0) {
        history->total_in += number;
    } else {
        history->total_out -= number;
    }
    
    return 0;
}

int main()
{   
    char *post = malloc(MAX_LEN * sizeof(char));  

    fgets(post, MAX_LEN, stdin);

    JsonParser *parser;
    GError *error = NULL;
    JsonReader *reader;
    const gchar *start_date, *end_date;
    
    parser = json_parser_new();
    json_parser_load_from_data(parser, post, -1, &error);
    if(error) {
        fputs(error->message, stderr);
        g_error_free(error);
        return 1;
    }

    free(post);

    reader = json_reader_new(json_parser_get_root(parser));
    g_object_unref(parser);

    json_reader_read_member(reader, "start");
    start_date = json_reader_get_string_value(reader);
    json_reader_end_member(reader);

    json_reader_read_member(reader, "end");
    end_date = json_reader_get_string_value(reader);
    json_reader_end_member(reader);

    sqlite3 *db;
    int rc, max_id;
    char *sql;
    char *err_msg = NULL;
    
    rc = sqlite3_open(DATABASE, &db);
    if(rc) {
        fputs(sqlite3_errmsg(db), stderr);
        return 1;
    }

    sql = malloc(MAX_LEN * sizeof(char));

    sprintf(sql, "SELECT MAX(id) AS MAX_ID FROM medicine");
    rc = sqlite3_exec(db, sql, get_max_id, &max_id, &err_msg);
    if(rc) {
        fputs(err_msg, stderr);
        sqlite3_free(err_msg);
        return 1;
    }

    puts("Content-type: application/json\n");
    putchar('[');

    int is_started = 0;
    for(int id = 1; id <= max_id; id++) {        
        struct {
            char *name;
            char *code;
            double price;
        } medicine;
        medicine.name = malloc(MAX_LEN * sizeof(char));
        medicine.code = malloc(MAX_LEN * sizeof(char));
        
        sprintf(sql, "SELECT name,code,price FROM medicine WHERE id=%d", id);
        rc = sqlite3_exec(db, sql, get_medicine_data, &medicine, &err_msg);
        if(rc) {
            fputs(err_msg, stderr);
            sqlite3_free(err_msg);
            return 1;
        }
        
        struct {
            int total_in;
            int total_out;
        } history;
        history.total_in = 0;
        history.total_out = 0;
        
        sprintf(sql,
                "SELECT * FROM m_%d WHERE date BETWEEN '%s' AND '%s'",
                id,
                start_date,
                end_date);
        rc = sqlite3_exec(db, sql, get_history_data, &history, &err_msg);
        if(rc) {
            fputs(err_msg, stderr);
            sqlite3_free(err_msg);
            return 1;
        }

        if(history.total_in || history.total_out) {
            if(is_started) {
                putchar(',');
            }
            if(!is_started) {
                is_started = 1;
            }
            printf("{\"id\":%d,\"name\":\"%s\",\"code\":\"%s\","
                   "\"price\":%lf,\"in\":%d,\"out\":%d}",
                   id,
                   medicine.name,
                   medicine.code,
                   medicine.price,
                   history.total_in,
                   history.total_out);
        }
        
        free(medicine.name);
        free(medicine.code);
    }

    putchar(']');

    free(sql);
    sqlite3_close(db);
    g_object_unref(reader);
    
    return 0;
}
