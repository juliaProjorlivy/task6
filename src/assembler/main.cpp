#include "commands.h"
#include "assembler.h"
#include "file_func.h"
#include "verror.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{

    size_t data_size = 0;
    size_t str_count = 0;
    size_t i_buf = 0;

    size_t n_labels = 10;
    size_t n_filled_labels = 0;

    struct label *labels = fill_labels(&n_labels, &n_filled_labels);
    if(labels == NULL)
    {
        VERROR_MEM;
        return 1;
    }

    char *data = get_data_from_file("code_asm.txt", &data_size);
    char **lines = make_ptr_array(data, &str_count);

    char *buf = (char *)calloc(sizeof(char), data_size);

    assembler(buf, &i_buf, lines, str_count, labels, &n_labels, &n_filled_labels);
    assembler(buf, &i_buf, lines, str_count, labels, &n_labels, &n_filled_labels);
    buf = (char *)realloc(buf, i_buf * sizeof(char));
    write_file(buf, "byte_code.bin", str_count, i_buf);

    // printf("last op = %d\n", (*(codes *)(buf + i_buf - sizeof(codes))).op);

    free_labels(labels, n_labels);
    free(data);
    free(lines);
    free(buf);
    return 0;
}

