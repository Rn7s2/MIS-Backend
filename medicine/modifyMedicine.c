/******************************************************************************
 * Filename: modifyMedicine.c
 * Author:   雷瑞祺
 * License:  MIT License
 * Purpose:  handle modify medicine http request
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <glib-object.h>
#include <json-glib/json-glib.h>

#define MAX_LEN 200001

int main(int argc, char **argv)
{
    FILE *ferr;
    
    char *tmp = NULL;
    char *post = NULL;

    JsonParser *parser;
    GError *error;
    JsonReader *reader;
 
    struct {
        gint64 id, number;
        gdouble price;
        const gchar *name, *code, *unit, *expiration, *dosage;
    } medicine;
    
    sqlite3 *db;
    char *sql = NULL;
    char *err_msg = NULL;
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
    g_object_unref(parser);

    json_reader_read_member(reader, "form");

    json_reader_read_member(reader, "id");
    medicine.id = json_reader_get_int_value(reader);
    json_reader_end_member(reader);
    json_reader_read_member(reader, "number");
    medicine.number = json_reader_get_int_value(reader);
    json_reader_end_member(reader);
    json_reader_read_member(reader, "price");
    medicine.price = json_reader_get_double_value(reader);
    json_reader_end_member(reader);
    json_reader_read_member(reader, "name");
    medicine.name = json_reader_get_string_value(reader);
    json_reader_end_member(reader);
    json_reader_read_member(reader, "code");
    medicine.code = json_reader_get_string_value(reader);
    json_reader_end_member(reader);
    json_reader_read_member(reader, "unit");
    medicine.unit = json_reader_get_string_value(reader);
    json_reader_end_member(reader);
    json_reader_read_member(reader, "expiration");
    medicine.expiration = json_reader_get_string_value(reader);
    json_reader_end_member(reader);
    json_reader_read_member(reader, "dosage");
    medicine.dosage = json_reader_get_string_value(reader);
    json_reader_end_member(reader);

    json_reader_end_member(reader);
    
    // open database
    rc = sqlite3_open("/home/www/mis.db", &db);

    if(rc) {
        fprintf(ferr, "Failed to open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // prepare sql statement
    sql = malloc(MAX_LEN * sizeof(char));
    sprintf(sql,
            "UPDATE medicine SET name='%s',"
            "code='%s',number=%ld,unit='%s',price=%lf,"
            "expiration='%s',dosage='%s' WHERE id=%ld",
            medicine.name,
            medicine.code,
            medicine.number,
            medicine.unit,
            medicine.price,
            medicine.expiration,
            medicine.dosage,
            medicine.id);

    g_object_unref(reader);
    
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
