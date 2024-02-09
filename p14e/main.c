#include<limits.h>
#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<ctype.h>
#include<glib.h>
#include<assert.h>

#define STR_MAX 100
#define ARR_TYPE_CHR char 

void g_ht_print_str_str(gpointer key, gpointer value, gpointer user_data) {
    printf("Patt: %s Res: %s\n", (char*)key, (char*) value); 
}

void g_ht_print_str___int128(gpointer key, gpointer value, gpointer user_data) {
    printf("Patt: %s PattLen: %lld Res: %lld \n", (char*)key, strlen((char*)key), *(__int128*) value); 
}

void g_ht_reset_str___int128(gpointer key, gpointer value, gpointer user_data) {
    *(__int128*)value = 0;
}

void g_ht_copy_str___int128(gpointer org_key, gpointer org_value, gpointer ht_dst) {
    g_hash_table_insert(ht_dst, (char*)org_key, (__int128*)org_value);
}

void g_ht_copy_add_str___int128(gpointer org_key, gpointer org_value, gpointer ht_dst) {
    __int128* ov = g_hash_table_lookup(ht_dst, org_key);
    if(ov == NULL) {
        ov = malloc(sizeof(__int128));
        g_hash_table_insert(ht_dst, org_key, ov);
    }
    *ov = *(__int128*) org_value;
}

void g_ht_copy_char_str___int128(gpointer org_key, gpointer org_value, gpointer ht_dst) {
    if(strlen((char*)org_key) == 1) {
        __int128* ov = g_hash_table_lookup(ht_dst, org_key);
        *ov = *(__int128*)org_value;
    }
}

void hash_table_inc(GHashTable* dest, char* key, __int128 inc) {
    __int128* oc = g_hash_table_lookup(dest, key);
    if (oc == NULL) {
        // Add the new pair
        char* nk = g_strdup(key);
        // To buffer ht
        oc = malloc(sizeof(__int128));
        *oc = 0;
        g_hash_table_insert(dest, nk, oc);
    }
    *oc += inc;
}

// Input and ga_transformations must contain valid data.
// tmp_buf has to be initialized, used to reuse the allocated memory between cycles.
void iterate(GHashTable* input, GHashTable* tmp_buf, GHashTable* ht_transformations) {
    printf("Iterate start\n");
    // Initialize memories
    char buf[3] = {'\0'};
    g_hash_table_foreach(tmp_buf, g_ht_reset_str___int128, NULL);
    // Setup initial status
    g_hash_table_foreach(input, g_ht_copy_char_str___int128, tmp_buf);

    // Do all transformations
    char* key;
    char* value;
    GHashTableIter iter;
    g_hash_table_iter_init(&iter, ht_transformations);
    while(g_hash_table_iter_next(&iter, (void*)&key, (void*)&value)) {
        __int128* count = (__int128*) g_hash_table_lookup(input, key);
        if (count != NULL) {
            printf("K: %s V: %lld \n", key, *count);
            // Add all the new letters
            buf[0] = value[0];
            buf[1] = '\0';
            hash_table_inc(tmp_buf, buf, *count);

            // If first half is contained, add all pairs
            buf[0] = key[0];
            buf[1] = value[0];
            buf[2] = '\0';
            hash_table_inc(tmp_buf, buf, *count);

            // If second half is contained, add all pairs
            buf[0] = value[0];
            buf[1] = key[1];
            buf[2] = '\0';
            hash_table_inc(tmp_buf, buf, *count);

            // We know we will not have any more of the current pair due to insertion
            *count = 0;
        }
    }

    // Finalize
    g_hash_table_foreach(tmp_buf, g_ht_copy_add_str___int128, input);
    printf("Iterate end\n");
}

int compare___int128( const void* a , const void* b )
{
    const __int128 ai = *( const __int128* )a;
    const __int128 bi = *( const __int128* )b;

    if( ai < bi )
    {
        return -1;
    }
    else if( ai > bi )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Create a table with the amount of pairs con tained
GHashTable* fromStringWindows(char* str) {
    GHashTable* ht = g_hash_table_new(g_str_hash, g_str_equal);
    char buf[3] = {'\0'};
    char buf_c[2] = {'\0'};
    // Setup initial status
    buf[1] = str[0];
    buf_c[0] = str[0];
    __int128* v = malloc(sizeof(__int128));
    *v = 1;
    g_hash_table_insert(ht, g_strdup(buf_c), v);
    // Loop
    const int org_len = strlen(str);
    for(int i=1; i<org_len; i++) {
        // Set char values
        buf_c[0] = str[i];
        v = g_hash_table_lookup(ht, buf_c);
        if (v==NULL) {
            v = malloc(sizeof(__int128));
            *v = 0;
            g_hash_table_insert(ht, g_strdup(buf_c), v);
        } 
        (*v)++;

        // Set window values
        buf[0] = buf[1];
        buf[1] = str[i];
        char* key = g_strdup(buf);
        __int128* vw = g_hash_table_lookup(ht, key);
        if (vw==NULL) {
            vw = malloc(sizeof(__int128));
            *vw = 0;
            g_hash_table_insert(ht, key, vw);
        } 
        (*vw)++;

        // Print results after iter
        printf("Key: %s Val: %lld Chr: %s ChrCount: %lld \n", key, *v, buf_c, *vw);
    }
    return ht;
}


// char* -> __int128
int calculate_total(GHashTable* ht) {
    __int128 max = 0;
    __int128 min = -1;
    char* key;
    __int128* value;
    GHashTableIter iter;
    g_hash_table_iter_init(&iter, ht);
    while(g_hash_table_iter_next(&iter, (void*)&key, (void*)&value)) {
        if(strlen(key) == 1) {
            if (*value < min) {
                min = *value;
            }
            if (*value > max) {
                max = *value;
            }
        }
    }
    return max - min;
}

int main(int argc, char** argv) {
    // String -> String
    GHashTable* ht_transformations = g_hash_table_new(g_str_hash, g_str_equal);
    //
    // First line, actual input
    char istr[STR_MAX] = "\0";
    fgets(istr, STR_MAX, stdin);
    istr[strlen(istr)-1] = '\0';
    GHashTable* ht_windows = fromStringWindows(istr);
    GHashTable* ht_windows_buf = fromStringWindows(istr);
    printf("Input:\n");
    g_hash_table_foreach(ht_windows, g_ht_print_str___int128, NULL);
    printf("Input end\n");

    // Empty line, not use
    char str[STR_MAX] = "\0";
    fgets(str, STR_MAX, stdin);
    // Just a newline, so either \r\n or \n
    assert(strlen(str) <= 2);

    // Read the transformations
    fgets(str, STR_MAX, stdin);
    while (!feof(stdin)) {
        if(strlen(str) == 0) break;
        // Search for the divisor '-'
        for(int i=0; str[i]!='\0'; i++) {
            if (str[i] == '-') {
                str[i-1] = '\0';
                char* sstr = g_strdup(str);
                str[i+4] = '\0';
                char* dstr = g_strdup(&str[i+3]);

                // We know is unique
                printf("IT Src: %s Dst: %s\n", sstr, dstr);
                assert(TRUE == g_hash_table_insert(ht_transformations, sstr, dstr));
            }
        }
        fgets(str, STR_MAX, stdin);
    }

    printf("Transformations\n");
    g_hash_table_foreach(ht_transformations, g_ht_print_str_str, NULL);
    printf("Transformations end\n");

    printf("IS: %s\n", str);
    for(int i=0; i<10; i++) {
        iterate(ht_windows, ht_windows_buf, ht_transformations);
        printf("I: %d\n", i);

        printf("Work:\n");
        g_hash_table_foreach(ht_windows, g_ht_print_str___int128, NULL);
        printf("Work end\n");
    }

    int total = calculate_total(ht_windows);
    printf("Total\n");
    printf("%d\n", total);

    return 0;
}
