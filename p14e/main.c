#include<limits.h>
#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<ctype.h>
#include<glib.h>
#include<assert.h>

#define STR_MAX 100

void g_ht_print(gpointer key, gpointer value, gpointer user_data) {
    printf("Patt: %s Res: %s\n", (char*)key, (char*) value); 
}

// Input and ht_transformations must contain valid data.
// tmp_buf has to be initialized, used to reuse the allocated memory between cycles.
void iterate(GString* input, GString* tmp_buf, GHashTable* ht_transformations) {
    // Initialize memories
    char buf[3] = {'\0'};
    g_string_erase(tmp_buf, 0, -1);
    const int org_len = strlen(input->str);
    // Setup initial status
    buf[1] = input->str[0];
    g_string_append(tmp_buf, &buf[1]);
    for(int i=1; i<org_len; i++) {
        buf[0] = buf[1];
        buf[1] = input->str[i];
        char* append = g_hash_table_lookup(ht_transformations, buf);
        if (append != NULL) {
            // We know it is only 1 char
            g_string_append(tmp_buf, append);
        }
        g_string_append(tmp_buf, &buf[1]);
    }
    g_string_overwrite(input, 0, tmp_buf->str);
}

int compare_uint64_t( const void* a , const void* b )
{
    const uint64_t ai = *( const uint64_t* )a;
    const uint64_t bi = *( const uint64_t* )b;

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

#define ARR_TYPE uint64_t
#define ARR_TYPE_CHR char 
GArray* countHits(char* str) {
    GArray* chars = g_array_new(FALSE, FALSE, sizeof(ARR_TYPE_CHR));
    GArray* counts = g_array_new(FALSE, FALSE, sizeof(ARR_TYPE));
    str--;
    while(*(++str) != '\0') {
        int char_idx = -1; 
        while(++char_idx < chars->len) if(g_array_index(chars, ARR_TYPE_CHR, char_idx) == *str) break;
        if (char_idx == chars->len) {
            ARR_TYPE iv = 0;
            g_array_append_val(chars, *str);
            g_array_append_val(counts, iv);
        }
        g_array_index(counts, ARR_TYPE, char_idx)++;

    }
    printf("Chars:\n");
    for(int i=0; i<counts->len; i++) {
        printf("%c ", g_array_index(chars, ARR_TYPE_CHR, i));
    }
    printf("\n");
    g_array_free(chars, TRUE);
    return counts;
}

uint64_t procHits(GArray* arr) {
    g_array_sort(arr, compare_uint64_t);
    return g_array_index(arr, ARR_TYPE, arr->len-1) - g_array_index(arr, ARR_TYPE, 0);
}

int main(int argc, char** argv) {
    // String -> String
    GHashTable* ht = g_hash_table_new(g_str_hash, g_str_equal);
    char str[STR_MAX] = "\0";
    // First line, actual input
    fgets(str, STR_MAX, stdin);
    // Remove last char \n
    str[strlen(str)-1] = '\0';
    GString* initial_state = g_string_new(str);
    // Empty line, not use
    fgets(str, STR_MAX, stdin);
    // Just a newline, so either \r\n or \n
    assert(strlen(str) <= 2);
    // Read the transformations
    fgets(str, STR_MAX, stdin);
    while (!feof(stdin)) {
        printf("IT: %s", str);
        if(strlen(str) == 0) break;
        // Search for the divisor '-'
        for(int i=0; str[i]!='\0'; i++) {
            if (str[i] == '-') {
                str[i-1] = '\0';
                char* sstr = g_strdup(str);
                str[i+4] = '\0';
                char* dstr = g_strdup(&str[i+3]);

                // Must not exist
                assert(g_hash_table_lookup(ht, sstr) == NULL);
                g_hash_table_insert(ht, sstr, dstr);
            }
        }
        fgets(str, STR_MAX, stdin);
    }

    printf("\n\n");
    g_hash_table_foreach(ht, g_ht_print, NULL);

    GArray* counts = NULL;
    GString* buf = g_string_new(initial_state->str);
    printf("IS: %s\n", str);
    for(int i=0; i<20; i++) {
        iterate(initial_state, buf, ht);
        printf("I: %d L: %lld\n", i, strlen(initial_state->str));
        counts = countHits(initial_state->str);
        printf("Frequencies:\n");
        for(int i=0; i<counts->len; i++) {
            printf("%lld ", g_array_index(counts, ARR_TYPE, i));
        }
        printf("\n");
        g_array_free(counts, TRUE);
    }
    g_string_free(buf, TRUE);

    counts = countHits(initial_state->str);
    printf("Frequencies:\n");
    for(int i=0; i<counts->len; i++) {
        printf("%lld ", g_array_index(counts, ARR_TYPE, i));
    }
    printf("\n");

    int total = procHits(counts);
    printf("Total\n");
    printf("%d\n", total);

    return 0;
}
