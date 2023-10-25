#include "commands.h"
#include "assembler.h"
#include "file_func.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{

    size_t data_size = 0;
    size_t str_count = 0;
    size_t i_buf = 0;

    char *data = get_data_from_file("code_asm.txt", &data_size);
    char **lines = make_ptr_array(data, &str_count);

    char *buf = (char *)calloc(sizeof(char), data_size);

    assembler(buf, &i_buf, lines, &str_count);
    write_file(buf, "byte_code.bin", str_count, i_buf);

    free(data);
    free(lines);
    free(buf);
    return 0;
}

