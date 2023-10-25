#include "commands.h"
#include "runner.h"
#include "file_func_runner.h"
#include "spu_ctor_dtor.h"
#include "spu_dump.h"

int main()
{
    struct spu proc = {};
    size_t buf_size = 0;
    char *buf = get_ptrs_from_file("byte_code.bin", &buf_size);
    // todo: header signature
    spu_ctor(&proc, buf, buf_size);
    runner(&proc);
    SPU_DUMP(&proc);
    spu_dtor(&proc);
    return 0;
}
