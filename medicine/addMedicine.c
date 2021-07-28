/******************************************************************************
 * Filename: addMedicine.c
 * Author:   雷瑞祺
 * License:  MIT License
 * Purpose:  handle add medicine http request
 ******************************************************************************/

#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <glib-object.h>
#include <json-glib/json-glib.h>

#define MAX_LEN 20001

int get_max_id(void* data, int col_count, char** col_contents, char** col_names)
{
    *(int*)data = atoi(col_contents[0]);
    
    return 0;
}

int main()
{
    char *post = NULL;

    post = malloc(MAX_LEN * sizeof(char));
    fgets(post, MAX_LEN, stdin);

    JsonParser *parser;
    GError *error = NULL;
    JsonReader *reader;

    parser = json_parser_new();
    json_parser_load_from_data(parser, post, -1, &error);

    free(post);

    reader = json_reader_new(json_parser_get_root(parser));

    g_object_unref(parser);

    struct {
        int number;
        double price;
        const char *name;
        const char *code;
        const char *unit;
        const char *expiration;
        const char *dosage;
    } medicine;

    json_reader_read_member(reader, "name");
    medicine.name = json_reader_get_string_value(reader);
    json_reader_end_member(reader);

    json_reader_read_member(reader, "code");
    medicine.code = json_reader_get_string_value(reader);
    json_reader_end_member(reader);

    json_reader_read_member(reader, "number");
    medicine.number = (int)json_reader_get_int_value(reader);
    json_reader_end_member(reader);

    json_reader_read_member(reader, "unit");
    medicine.unit = json_reader_get_string_value(reader);
    json_reader_end_member(reader);

    json_reader_read_member(reader, "price");
    medicine.price = json_reader_get_double_value(reader);
    json_reader_end_member(reader);

    json_reader_read_member(reader, "expiration");
    medicine.expiration = json_reader_get_string_value(reader);
    json_reader_end_member(reader);

    json_reader_read_member(reader, "dosage");
    medicine.dosage = json_reader_get_string_value(reader);
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
        free(sql);
        return 1;
    }

    sprintf(sql,
            "INSERT INTO medicine (name, code, number,"
            " unit, price, expiration, dosage) "
            "VALUES ('%s', '%s', %d, '%s', %lf, '%s', '%s');"
            "CREATE TABLE m_%d(date TEXT, number INT)",
            medicine.name,
            medicine.code,
            medicine.number,
            medicine.unit,
            medicine.price,
            medicine.expiration,
            medicine.dosage,
            max_id + 1
        );

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if(rc) {
        fputs(err_msg, stderr);
        sqlite3_free(err_msg);
        free(sql);
        return 1;
    }

    if(medicine.number > 0) {
        sprintf(sql,
                "INSERT INTO m_%d VALUES (date('now'), %d)",
                max_id + 1,
                medicine.number);
        
        rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
        if(rc) {
            fputs(err_msg, stderr);
            sqlite3_free(err_msg);
            free(sql);
            return 1;
        }
    }
    
    g_object_unref(reader);

    free(sql);
    sqlite3_close(db);

    puts("Content-type: text/html\n");
    
    return 0;
}
