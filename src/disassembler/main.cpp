#include "commands.h"
#include "disassembler.h"
#include "file_func_dasm.h"
#include <stdlib.h>

int main()
{
    size_t n_com = 0;
    struct codes *all_codes = get_ptrs_from_file("to_calculate_bc.txt", &n_com);
    char **lines = disasm(all_codes, n_com);
    write_file_dasm("to_calculate_damb.txt", lines, n_com);

    free(all_codes);
    for(size_t i = 0; i < n_com; i++)
    {
        free(*(lines + i));
    }
    free(lines);
    return 0;
}
