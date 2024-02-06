#include<limits.h>
#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<ctype.h>
#include<glib.h>
#include<assert.h>

#define STR_MAX 100
#define ARR_TYPE uint8_t 

void g_ht_print(gpointer key, gpointer value, gpointer user_data) {
    printf("Node: %s\n", (char*)key); 
    GPtrArray* arr = (GPtrArray*) value;
    printf("Destinations: ");
    for(int i=0;i<(arr->len-1);i++) {
        printf("%s, ", (char*) g_ptr_array_index(arr, i));
    }
    printf("%s\n", (char*) g_ptr_array_index(arr, arr->len-1));
}

int count_path(GHashTable* ht, GHashTable* seen, GHashTable* seen2, char* location) {
    if (strcmp("end", location) == 0) {
        //printf("Reached!\n");
        return 1;
    }
    int total = 0;
    if (islower(location[0])) {
        // Seen once
        if (g_hash_table_contains(seen, location)) {
            // Start can only be seen once
            if (strcmp("start", location) == 0) {
                return 0;
            }
            // Seen twice only one!
            if (g_hash_table_size(seen2) > 0) {
                return 0;
            }
            g_hash_table_add(seen2, location);
        }
        g_hash_table_add(seen, location);
    }
    GPtrArray* arr = g_hash_table_lookup(ht, location);
    //printf("Loc: %s\n", location);
    assert(arr != NULL);
    for(int i=0; i<arr->len; i++) {
        total += count_path(ht, seen, seen2, (char*)g_ptr_array_index(arr, i));
    }
    if (islower(location[0])) {
        // Removed from second seen
        if(!g_hash_table_remove(seen2, location)) {
            // Removed from first seen if no in second seen
            assert(g_hash_table_remove(seen, location));
        }
    }
    return total;
}

int main(int argc, char** argv) {
    GHashTable* ht = g_hash_table_new(g_str_hash, g_str_equal);
    char str[STR_MAX] = "\0";
    scanf("%100s", str);
    while (!feof(stdin)) {
        printf("%s\n", str);
        // Search for the divisor '-'
        for(int i=0; str[i]!='\0'; i++) {
            if (str[i] == '-') {
                str[i] = '\0';
                char* sstr = g_strdup(str);
                char* dstr = g_strdup(&str[i+1]);

                // start -> dest
                GPtrArray* arr = g_hash_table_lookup(ht, str);
                if (arr == NULL) {
                    arr = g_ptr_array_new();
                    g_ptr_array_add(arr, dstr);
                    g_hash_table_insert(ht, sstr, arr);
                } else {
                    g_ptr_array_insert(arr, -1, dstr);
                }

                // dest -> start
                arr = g_hash_table_lookup(ht, dstr);
                if (arr == NULL) {
                    arr = g_ptr_array_new();
                    g_ptr_array_add(arr, sstr);
                    g_hash_table_insert(ht, dstr, arr);
                } else {
                    g_ptr_array_insert(arr, -1, sstr);
                }
            }
        }
        scanf("%100s", str);
    }

    printf("\n\n");
    g_hash_table_foreach(ht, g_ht_print, NULL);

    printf("\n\n");
    GHashTable* seen_set = g_hash_table_new(g_str_hash, g_str_equal);
    GHashTable* seen_set_2 = g_hash_table_new(g_str_hash, g_str_equal);
    

    int total = count_path(ht, seen_set, seen_set_2, "start");
    printf("Total\n");
    printf("%d\n", total);

    return 0;
}
