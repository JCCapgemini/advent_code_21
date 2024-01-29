#include<stdio.h>
#include <glib.h>

int main(int argc, char** argv) {
    GList* list = NULL;
    list = g_list_append(list, "Hello world!1");
    list = g_list_append(list, "Hello world!2");
    list = g_list_append(list, "Hello world!3");
    while (g_list_length(list) > 0) {
        GList* current = g_list_first(list);
        list = g_list_remove(list, current->data);
        printf("The first item is '%s'\n", (char*)current->data);
    }
    return 0;
}
