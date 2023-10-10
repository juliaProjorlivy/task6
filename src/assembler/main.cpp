#include "assembler.h"
#include "file_func.h"
#include <stdlib.h>

int main()
{

    size_t data_size = 0;
    size_t str_count = 0;

    char *data = get_data_from_file("to_calculate.txt", &data_size);
    char **lines = make_ptr_array(data, &str_count);
    struct codes *commands = assembler(lines, str_count);
    write_file("to_calculate_bc.txt", commands, str_count);

    free(data);
    // for(size_t i = 0; i < str_count; i++)
    // {
    //     free(lines[i]);
    // }
    free(lines);
    free(commands);
    return 0;
}

