#include "commands.h"
#include "disassembler.h"
#include "file_func_dasm.h"
#include <stdlib.h>

int main()
{
    size_t n_com = 0;
    size_t buf_size = 0;
    char *buf = get_ptrs_from_file("recrusion_byte_code.bin", &buf_size, &n_com);
    char **lines = disasm(buf, buf_size, n_com);
    write_file_dasm("recrusion_dasm.txt", lines, n_com);

    free(buf);
    for(size_t i = 0; i < n_com; i++)
    {
        free(*(lines + i));
    }
    free(lines);
    return 0;
}
