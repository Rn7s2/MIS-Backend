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

#define MAX_LEN 20001

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
        char *name;
        char *code;
        char *unit;
        char *expiration;
        char *dosage;
    } medicine;

    medicine.xxx = malloc(MAX_LEN * sizeof(char)); // 分配内存

    json_reader_read_member(reader, "name");
    // xxx =xxx 赋值
    json_reader_end_member(reader);

    json_reader_read_member(reader, "code");
    
    json_reader_end_member(reader);

    json_reader_read_member(reader, "number");
    
    json_reader_end_member(reader);

    json_reader_read_member(reader, "unit");
    
    json_reader_end_member(reader);

    json_reader_read_member(reader, "price");
    
    json_reader_end_member(reader);

    json_reader_read_member(reader, "expiration");
    
    json_reader_end_member(reader);

    json_reader_read_member(reader, "dosage");
    
    json_reader_end_member(reader);
    
    return 0;
}
