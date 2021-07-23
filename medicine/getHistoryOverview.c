/******************************************************************************
 * Filename: getHistoryOverview.c
 * Author:   雷瑞祺
 * License:  MIT License
 * Purpose:  handle get history overview http request
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <glib-object.h>
#include <json-glib/json-glib.h>

#define MAX_LEN 30001

int day_diff(int year_start, int month_start, int day_start,
             int year_end, int month_end, int day_end)
{
    int y2, m2, d2;
    int y1, m1, d1;

    m1 = (month_start + 9) % 12;
    y1 = year_start - m1 / 10;
    d1 = 365 * y1 + y1 / 4 - y1 / 100 + y1 / 400 + (m1 * 306 + 5) / 10 + (day_start - 1);

    m2 = (month_end + 9) % 12;
    y2 = year_end - m2 / 10;
    d2 = 365 * y2 + y2 / 4 - y2 / 100 + y2 / 400 + (m2 * 306 + 5) / 10 + (day_end - 1);

    return (d2 - d1);
}

int hash_date(const char* date)
{
    char tmp[5];
    int year, month, day;
    strncpy(tmp, date, 4);
    tmp[4] = '\0';
    year = atoi(tmp);
    strncpy(tmp, date + 5, 2);
    tmp[2] = '\0';
    month = atoi(tmp);
    strncpy(tmp, date + 8, 2);
    tmp[2] = '\0';
    day = atoi(tmp);
    return day_diff(2018, 1, 1, year, month, day) % MAX_LEN;
}

int table_in[MAX_LEN];
int table_out[MAX_LEN];
char table_date[MAX_LEN][11];

int post_select(void *data, int col_count, char** col_contents, char** col_names)
{
    char *date = col_contents[0];
    int number = atoi(col_contents[1]);
    int hsdate = hash_date(date);

    if(table_date[hsdate][0] == '\0') {
        strcpy(table_date[hsdate], date);
    }

    if(number > 0) {
        table_in[hsdate] += number;
    } else {
        table_out[hsdate] -= number;
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
    gint64 id;
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

    json_reader_read_member(reader, "id");
    id = json_reader_get_int_value(reader);
    json_reader_end_member(reader);

    json_reader_read_member(reader, "start");
    start_date = json_reader_get_string_value(reader);
    json_reader_end_member(reader);

    json_reader_read_member(reader, "end");
    end_date = json_reader_get_string_value(reader);
    json_reader_end_member(reader);

    sqlite3 *db;
    int rc;
    char *sql;
    char *err_msg = NULL;
    
    rc = sqlite3_open("/home/www/mis.db" , &db);
    if(rc) {
        fputs(sqlite3_errmsg(db), stderr);
        return 1;
    }

    sql = malloc(MAX_LEN * sizeof(char));
    sprintf(sql,
            "SELECT * FROM m_%ld WHERE date BETWEEN '%s' AND '%s'",
            id,
            start_date,
            end_date);
    rc = sqlite3_exec(db, sql, post_select, NULL, &err_msg);

    if(rc) {
        fputs(err_msg, stderr);
        sqlite3_free(err_msg);
        return 1;
    }

    free(sql);
    sqlite3_close(db);
    g_object_unref(reader);

    int hsstart_date = hash_date(start_date);
    int hsend_date = hash_date(end_date);

    int total_in = 0;
    int total_out = 0;
    int row_count = 0;
    
    for(int i = hsstart_date; i <= hsend_date; i++) {
        if(table_date[i][0] == '\0') {
            continue;
        }
        total_in += table_in[i];
        total_out += table_out[i];
        row_count++;
    }
    
    puts("Content-type: application/json\n");

    printf("{\"stockIn\":%d,\"stockOut\":%d,\"history\":[",
           total_in,
           total_out);

    for(int i = hsstart_date, cnt = 0; i <= hsend_date; i++) {
        if(table_date[i][0] == '\0') {
            continue;
        }
        printf("{\"date\":\"%s\",\"in\":%d,\"out\":%d}",
               table_date[i],
               table_in[i],
               table_out[i]);
        cnt++;
        if(cnt != row_count) {
            putchar(',');
        }
    }

    printf("]}");
    
    return 0;
}
