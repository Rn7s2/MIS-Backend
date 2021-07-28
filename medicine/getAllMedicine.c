/******************************************************************************
 * Filename: getAllMedicine.c
 * Author:   雷瑞祺
 * License:  MIT License
 * Purpose:  handle get all medicine http request
 ******************************************************************************/

#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <glib-object.h>
#include <json-glib/json-glib.h>

int post_exec(void *data, int col_count, char** col_contents, char** col_names)
{
    JsonBuilder* builder = (JsonBuilder*)data;

    json_builder_begin_object(builder);
    while(col_count--) {
        json_builder_set_member_name(builder, *col_names);
        if((!strcmp(*col_names, "id")) || (!strcmp(*col_names, "number"))) {
            json_builder_add_int_value(builder, atoll(*col_contents));
        } else if(!strcmp(*col_names, "price")) {
            json_builder_add_double_value(builder, atof(*col_contents));
        } else {
            json_builder_add_string_value(builder, *col_contents);
        }
        col_names++, col_contents++;
    }    
    json_builder_end_object(builder);
    
    return 0;
}

int main(int argc, char **argv)
{
    sqlite3 *db;
    char *err_msg = NULL;
    int ret_code = 0;

    JsonBuilder *builder;
    JsonGenerator *generator;
    JsonNode *root_node;
    gchar* str;

    puts("Content-type: application/json\n");

    ret_code = sqlite3_open(DATABASE, &db);
    if(ret_code) {
        fprintf(stderr, "Failed to open SQLite Database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    builder = json_builder_new();
    json_builder_begin_array(builder);
    
    ret_code = sqlite3_exec(db, "SELECT * FROM medicine", post_exec, builder, &err_msg);

    if(ret_code) {
        fprintf(stderr, "Failed to eval SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }
    sqlite3_close(db);

    json_builder_end_array(builder);

    generator = json_generator_new();
    root_node = json_builder_get_root(builder);
    json_generator_set_root(generator, root_node);
    str = json_generator_to_data(generator, 0);

    puts(str);
    g_free(str);    
    
    json_node_free(root_node);
    g_object_unref(generator);
    g_object_unref(builder);
    return 0;
}
