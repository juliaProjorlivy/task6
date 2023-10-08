#ifndef SPU_CTOR_DTOR_H
#define SPU_CTOR_DTOR_H

void spu_ctor(struct spu *proc, size_t code_size);

void spu_dtor(struct spu *proc);

#endif
