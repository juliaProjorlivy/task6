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

    struct stat file_buf;
    stat("code_asm.txt", &file_buf);
    ssize_t file_size = file_buf.st_size;
    char *buf = (char *)calloc(sizeof(char), file_size);

    char *data = get_data_from_file("code_asm.txt", &data_size);
    char **lines = make_ptr_array(data, &str_count);

    struct codes *all_codes = assembler(buf, &i_buf, lines, &str_count);
    printf("op = %d \n", (int)(all_codes[0].op));
    write_file(buf, "byte_code.bin", str_count, i_buf);

    free(data);
    free(lines);
    free(all_codes);
    free(buf);
    return 0;
}

