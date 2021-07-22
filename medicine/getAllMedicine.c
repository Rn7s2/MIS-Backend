#include <stdio.h>
#include <stdlib.h>
#include <glib-object.h>
#include <json-glib/json-glib.h>

int main(int argc, char **argv)
{
    JsonBuilder *builder;
    JsonArray *array;
    GError *error;

    puts("Content-type: application/json\n");
    puts("[{\"id\":1,\"name\":\"测试药品\",\"code\":\"192345\",\"number\":17,\"unit\":\"支\",\"price\":13.5,\"expiration\":\"2021-07-25\",\"dosage\":\"每日一次，每次一片\"}]");
    return 0;
}
