#include "commands.h"
#include "assembler.h"
#include "file_func.h"
#include <stdlib.h>

int main()
{

    size_t data_size = 0;
    size_t str_count = 0;

    char *data = get_data_from_file("code_asm.txt", &data_size);
    char **lines = make_ptr_array(data, &str_count);
    struct codes *commands = assembler(lines, str_count);
    printf("command2 = %d\n", commands[4].op);
    write_file("byte_code.bin", commands, str_count);

    free(data);
    free(lines);
    free(commands);
    return 0;
}

