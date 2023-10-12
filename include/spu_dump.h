#ifndef SPU_DUMP_H
#define SPU_DUMP_H

#define SPU_DUMP(proc) spu_dump((proc), __FILE__, __LINE__, __func__, (#proc))

void spu_dump(struct spu *proc, const char *file, int line, const char *func, const char *arg);

#endif
