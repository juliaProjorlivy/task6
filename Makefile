CXX = g++

CXX_FLAGS = -I include -I lib/stack/include -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations 	\
			-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion \
			-Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 	\
			-Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self 		\
			-Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 					\
			-Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default 			\
			-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix 		\
			-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new 			\
			-fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 \
			-Wstack-usage=8192 -pie -fPIE -Werror=vla #-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
SRCS = src/verror.cpp
SRCS_ASM = src/assembler/assembler.cpp src/assembler/file_func.cpp src/assembler/main.cpp
SRCS_VM = src/runner/runner.cpp src/runner/command_functions.cpp src/runner/spu_ctor_dtor.cpp src/runner/spu_dump.cpp src/runner/main.cpp src/runner/file_func_runner.cpp
SRCS_DASM = src/disassembler/disassembler.cpp src/disassembler/file_func_dasm.cpp src/disassembler/main.cpp

OBJ = build/verror.o
OBJ_ASM = $(patsubst %.cpp, build/assembler/%.o, $(subst src/assembler/, , $(SRCS_ASM))) 
OBJ_VM = $(patsubst %.cpp, build/runner/%.o, $(subst src/runner/, , $(SRCS_VM))) lib/stack/lib/libStack.a
OBJ_DASM = $(patsubst %.cpp, build/disassembler/%.o, $(subst src/disassembler/, , $(SRCS_DASM))) 

EXECUTABLE_ASM = assembler
EXECUTABLE_VM = runner
EXECUTABLE_DASM = disassembler

VALGRIND = valgrind --leak-check=full --leak-resolution=med ./$(EXECUTABLE_DASM)

assembler: $(OBJ_ASM) $(OBJ)
	@echo "CXX $(EXECUTABLE_ASM)"
	@$(CXX) $(CXX_FLAGS) -lasan $(OBJ_ASM) $(OBJ) -o $(EXECUTABLE_ASM)

runner: $(OBJ_VM) $(OBJ) 
	@echo "CXX $(EXECUTABLE_VM)"
	@$(CXX) $(CXX_FLAGS) -lasan $(OBJ_VM) $(OBJ) -o $(EXECUTABLE_VM)

disassembler: $(OBJ_DASM) $(OBJ)
	@echo "CXX $(EXECUTABLE_DASM)"
	@$(CXX) $(CXX_FLAGS) -lasan $(OBJ_DASM) $(OBJ) -o $(EXECUTABLE_DASM)

build/%.o: src/verror.cpp
	mkdir -p ./build
	@$(CXX) $(CXX_FLAGS) -c -o $@ $<

build/assembler/%.o: src/assembler/%.cpp
	mkdir -p ./build/assembler
	@$(CXX) $(CXX_FLAGS) -c -o $@ $<

build/runner/%.o: src/runner/%.cpp 
	mkdir -p ./build/runner
	@$(CXX) $(CXX_FLAGS) -c -o $@ $<

build/disassembler/%.o: src/disassembler/%.cpp
	mkdir -p ./build/disassembler
	@$(CXX) $(CXX_FLAGS) -c -o $@ $<

lib/stack/lib/libStack.a:
	make lib -C lib/stack/

.PHONY: clean mem

clean:
	@rm -f build/*.o -f build/assembler/*.o -f build/runner/*.o -f build/disassembler/*.o -f assembler -f runner -f disassembler
clean_asm:
	@rm -f build/assembler/*.o -f assembler
clean_vm:
	@rm -f build/runner/*.o -f runner
clean_dasm:
	@rm -f build/disassembler/*.o -f disassembler
clean_stk:
	@rm -f lib/stack/lib/libStack.a

mem_dasm:
	valgrind --leak-check=full --leak-resolution=med ./$(EXECUTABLE_DASM)

mem_asm:
	valgrind --leak-check=full --leak-resolution=med ./$(EXECUTABLE_ASM)

mem_vm:
	valgrind --leak-check=full --leak-resolution=med ./$(EXECUTABLE_VM)
