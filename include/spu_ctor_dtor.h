#ifndef SPU_CTOR_DTOR_H
#define SPU_CTOR_DTOR_H

#include <stdio.h>

void spu_ctor(struct spu *proc, struct codes *all_codes, size_t n_codes);

void spu_dtor(struct spu *proc);

#endif
