#include "commands.h"
#include "runner.h"
#include "file_func_runner.h"
#include "spu_ctor_dtor.h"
#include "spu_dump.h"

int main()
{
    struct spu proc = {};
    size_t n_codes = 0;
    struct codes *all_codes = get_ptrs_from_file("to_calculate_bc.txt", &n_codes);
    spu_ctor(&proc, all_codes, n_codes);
    SPU_DUMP(&proc);
    runner(&proc);
    spu_dtor(&proc);
    return 0;
}
