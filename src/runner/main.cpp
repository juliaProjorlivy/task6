#include "commands.h"
#include "runner.h"
#include "file_func_runner.h"
#include "spu_ctor_dtor.h"
#include "spu_dump.h"
#include "verror.h"

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf(RED "no needed file" END_OF_RED);
        return 1;
    }
    struct spu proc = {};
    size_t buf_size = 0;
    char *buf = get_ptrs_from_file(argv[1], &buf_size);
    if(buf == NULL)
    {
        VERROR_MEM;
        return 1;
    }
    // todo: header signature
    if(spu_ctor(&proc, buf, buf_size))
    {
        return 1;
    }
    if(runner(&proc))
    {
        return 1;
    }
    spu_dtor(&proc);
    return 0;
}
