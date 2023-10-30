#include "commands.h"
#include "assembler.h"
#include "file_func.h"
#include "verror.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf(RED "no needed file" END_OF_RED);
        return 1;
    }
    size_t data_size = 0;
    size_t str_count = 0;
    size_t i_buf = 0;

    size_t n_labels = 50;
    size_t n_filled_labels = 0;

    struct label *labels = fill_labels(&n_labels, &n_filled_labels);
    if(labels == NULL)
    {
        VERROR_MEM;
        return 1;
    }

    char *data = get_data_from_file(argv[1], &data_size);
    if(data == NULL)
    {
        VERROR_MEM;
        return 1;
    }
    char **lines = make_ptr_array(data, &str_count);
    if(lines == NULL)
    {
        VERROR_MEM;
        return 1;
    }

    char *buf = (char *)calloc(sizeof(char), data_size);
    if(buf == NULL)
    {
        VERROR_MEM;
        return 1;
    }

    if(assembler(buf, &i_buf, lines, str_count, labels, &n_labels, &n_filled_labels))
    {
        return 1;
    }
    if(assembler(buf, &i_buf, lines, str_count, labels, &n_labels, &n_filled_labels))
    {
        return 1;
    }
    buf = (char *)realloc(buf, i_buf * sizeof(char));
    if(buf == NULL)
    {
        VERROR_MEM;
        return 1;
    }
    if(write_file(buf, argv[2], str_count, i_buf))
    {
        return 1;
    }

    printf("n lab = %zu\n", n_labels);

    free_labels(labels, n_labels);
    free(data);
    free(lines);
    free(buf);
    return 0;
}

