#include<limits.h>
#include<stdio.h>
#include<stdlib.h>
#include<glib.h>

#define STR_MAX 100

gpointer my_ptr_array_copy (gconstpointer src, gpointer dest)
{
    dest = src;
    return dest;
}

void print_array(GPtrArray const * const arr) {
    // Process actual input
    printf("Print array:\n");
    for (int i=0; i<arr->len ; i++) {
        printf("%s\n", (char*)g_ptr_array_index(arr, i));
    }
}

int my_string_sort_function (gconstpointer a, gconstpointer b)
{
    char *str_a = *(char **)a;
    char *str_b = *(char **)b;
    //printf("A: %s B: %s\n", str_a, str_b);
    return strcmp (str_a, str_b);
}

int bits_to_int(char const * const bit_str) {
    const int len = strlen(bit_str)-1;
    int res = 0;
    for(int i=0; i<=len; i++) {
        res |= (bit_str[i] == '1')<<(len-i);
    }
    printf("Bti Len: %d In: %s Out: %d\n", len, bit_str, res);
    return res;
}

void least_most_bits(int* least, int* most, int* pwidth, GPtrArray const * const arr){
    int counts[STR_MAX] = {0};
    int ncounts[STR_MAX] = {0};
    int count = 0;
    int width = 0;
    for (int i=0; i<arr->len ; i++) {
        char* str = (char*)g_ptr_array_index(arr, i);
        count++;
        for(int i=0; str[i] != '\0'; i++){
            switch(str[i]){
                case '0': ncounts[i]++; break;
                case '1': counts[i]++; break;
                default: exit(1);
            };
            width = width * (width>=i) | i * (width<i);
        }
        //printf("Val %d: %s\n", i, str);
    }
    *pwidth = width;

    // Calculate most common and least common bits
    *most = 0;
    *least = 0;
    // Width is inclusive due to being the max index allowed
    for(int i=0; i <= width ; i++){
        printf("Counts %d: %d %d\n", i, counts[i], ncounts[i]);
        // We swap it around because I store it MSB to LSB
        if (counts[i] >= ncounts[i]) {
            *most |= 1<<(width-i);
        } else {
            *least |= 1<<(width-i);
        }
    }
    char resbuf[100] = "\0";
    char nresbuf[100] = "\0";
    itoa(*most, resbuf, 2);
    itoa(*least, nresbuf, 2);
    printf("Result %s Neg %s\n", resbuf, nresbuf);
}

int most_common_value(GPtrArray const * const arr) {
    GPtrArray* most_array = g_ptr_array_copy(arr, my_ptr_array_copy, NULL);
    int most, least, width;
    least_most_bits(&most, &least, &width, most_array);
    int cur_idx = width;
    // Get the values by most common digits
    printf("Size: %d\n", most_array->len);
    while (cur_idx >= 0 && most_array->len > 1) {
        const int mask = (1 << cur_idx);
        const int res_choice = (most & mask) != 0;
        printf("Idx %d Mask %d Choice %d\n", cur_idx, mask, res_choice);
        for (int val=0; val<most_array->len; val++) {
            printf("Val %d\n", val);
            const char * const act_val = g_ptr_array_index(most_array, val);
            char delete = FALSE;
            char cur_char_val = act_val[width - cur_idx];
            if (res_choice) {
                if (cur_char_val == '1') {
                    delete = TRUE;
                }
            } else if (cur_char_val == '0'){
                delete = TRUE;
            }
            if (delete == TRUE) {
                if (most_array->len <= 1) {
                    break;
                }
                printf("Deleted %d %s\n", most_array->len, (char*)g_ptr_array_index(most_array, val));
                g_ptr_array_remove_index(most_array, val);
                val--;
            }
        }
        cur_idx--;
        least_most_bits(&most, &least, &width, most_array);
        print_array(most_array);
    }
    printf("most size %d\n", most_array->len);
    printf("most data: %s\n", (char*)g_ptr_array_index(most_array, 0));
    int res = bits_to_int((char*)g_ptr_array_index(most_array, 0));
    g_ptr_array_free(most_array, TRUE);

    return res;
}

int least_common_value(GPtrArray const * const arr) {
    print_array(arr);
    GPtrArray* most_array = g_ptr_array_copy(arr, my_ptr_array_copy, NULL);
    int most, least, width;
    least_most_bits(&most, &least, &width, most_array);
    int cur_idx = width;
    // Get the values by most common digits
    printf("Size: %d\n", most_array->len);
    while (cur_idx >= 0 && most_array->len > 1) {
        const int mask = (1 << cur_idx);
        const int res_choice = (least & mask) != 0;
        printf("Idx %d Mask %d Choice %d\n", cur_idx, mask, res_choice);
        for (int val=0; val<most_array->len; val++) {
            printf("Val %d\n", val);
            const char * const act_val = g_ptr_array_index(most_array, val);
            char delete = FALSE;
            char cur_char_val = act_val[width - cur_idx];
            if (res_choice) {
                if (cur_char_val == '1') {
                    delete = TRUE;
                }
            } else if (cur_char_val == '0'){
                delete = TRUE;
            }
            if (delete == TRUE) {
                if (most_array->len <= 1) {
                    break;
                }
                printf("Deleted RemSize %d ElementDel %s\n", most_array->len-1, (char*)g_ptr_array_index(most_array, val));
                g_ptr_array_remove_index(most_array, val);
                val--;
            }
        }
        cur_idx--;
        least_most_bits(&most, &least, &width, most_array);
        print_array(most_array);
    }
    printf("least size %d\n", most_array->len);
    printf("least data: %s\n", (char*)g_ptr_array_index(most_array, 0));
    int res = bits_to_int((char*)g_ptr_array_index(most_array, 0));
    g_ptr_array_free(most_array, TRUE);

    return res;
}

int main(int argc, char** argv) {
    GPtrArray* input = g_ptr_array_new();
    char str[STR_MAX] = "\0";
    int width = 0;

    // Just store data
    scanf("%100s", str);
    while (!feof(stdin)) {
        width = strlen(str);
        char* t_arr = malloc(sizeof(char) * width + 1);
        strcpy(t_arr, str);
        g_ptr_array_insert(input, -1, t_arr);
        scanf("%100s", str);
    }

    // Sort to remove duplicates
    g_ptr_array_sort(input, my_string_sort_function);

    // Process actual input
    for (int i=1; i<input->len ; i++) {
        // If current and before are equals, remove current and remove 1 from position
        if (strcmp((char*)g_ptr_array_index(input, i-1), (char*)g_ptr_array_index(input, i)) == 0) {
            g_ptr_array_remove_index(input, i);
            i--;
            continue;
        }
    }

    printf("Most!!!\n");
    int most = most_common_value(input);
    printf("Returned most: %d\n", most);

    printf("Least!!!\n");
    int least = least_common_value(input);
    printf("Returned least: %d\n", least);

    // Just release all memory
    for (int i=0; i<input->len ; i++) {
        free(g_ptr_array_index(input, i));
    }
    g_ptr_array_free(input, TRUE);
    printf("%d\r", most * least);
    return 0;
}
