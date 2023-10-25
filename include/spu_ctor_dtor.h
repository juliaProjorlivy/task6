#ifndef SPU_CTOR_DTOR_H
#define SPU_CTOR_DTOR_H

#include <stdio.h>

int spu_ctor(struct spu *proc, char *buf, size_t buf_size);

void spu_dtor(struct spu *proc);

#endif
