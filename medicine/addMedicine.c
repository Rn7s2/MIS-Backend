/******************************************************************************
 * Filename: addMedicine.c
 * Author:   雷瑞祺
 * License:  MIT License
 * Purpose:  handle add medicine http request
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

    printf("The posted new medicine information:\n"
           "name: %s\n"
           "code: %s\n"
           "number: %d\n"
           "unit: %s\n"
           "price: %lf\n"
           "expiration: %s\n"
           "dosage: %s\n",
           medicine.name,
           medicine.code,
           medicine.number,
           medicine.unit,
           medicine.price,
           medicine.expiration,
           medicine.dosage);

    g_object_unref(reader);
    
    return 0;
}
