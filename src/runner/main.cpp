#include "commands.h"
#include "runner.h"
#include "file_func_runner.h"
#include "spu_ctor_dtor.h"

int main()
{
    struct spu proc = {};
    size_t n_codes = 0;
    struct codes *all_codes = get_ptrs_from_file("to_calculate_bc.txt", &n_codes);
    spu_ctor(&proc, all_codes, n_codes);
    runner(&proc);
    spu_dtor(&proc);
    return 0;
}
