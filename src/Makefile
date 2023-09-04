MAKEFLAGS += --no-builtin-rules
MAKEFLAGS += --no-builtin-variables
.SUFFIXES:

# MSVC windows may be not supported. If you are on windows, you may have to have mingw installed in
# order to compile this.  
CC=gcc -Wall -Wextra -std=c11

INCLUDES=-isystem ../include

LIBS_SRC=
LIBS=-lglfw3

# ==== OS - dependent

# win64_mingw-w64, win32_mingw-w64, linux_amd64, linux_i386, macos
LIBRARIES_VERSION=

RMRF=rm -rf
MKDIR=mkdir -p
CP=cp -rf
RENAME_FOLDER=mv
DYLIB_EXT=.so
EXEC_EXT=
WARNS=

RMRF_EXE=
CP_EXE=
MKDIR_EXE=

# Determining target OS 
ifeq ($(OS),Windows_NT)
	RMRF=./../sane_windows/sane_rm.exe
	CP=./../sane_windows/sane_cp.exe -f
	MKDIR=./../sane_windows/sane_mkdir.exe

	RMRF_EXE=../sane_windows/sane_rm.exe
	CP_EXE=../sane_windows/sane_cp.exe
	MKDIR_EXE=../sane_windows/sane_mkdir.exe

	DYLIB_EXT=.dll
	EXEC_EXT=.exe

	RENAME_FOLDER=mv
#	Removed -mwindows flag from CC
	CC+=-D WIN32
	LIBS+=-lgdi32 -lwinmm

	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
		LIBRARIES_VERSION=win64_mingw-w64
		CC+=-D AMD64
	else
		ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
			LIBRARIES_VERSION=win64_mingw-w64
			CC+=-D AMD64
		endif
		ifeq ($(PROCESSOR_ARCHITECTURE),x86)
			LIBRARIES_VERSION=win32_mingw-w64
			RMRF=./../sane_windows/sane_rm32.exe
			CP=./../sane_windows/sane_cp32.exe
			MKDIR=./../sane_windows/sane_mkdir32.exe

			RMRF_EXE=../sane_windows/sane_rm32.exe
			CP_EXE=../sane_windows/sane_cp32.exe
			MKDIR_EXE=../sane_windows/sane_mkdir32.exe

			CC+=-D IA32
		endif
	endif
	ALL_EXE=${RMRF_EXE} ${CP_EXE} ${MKDIR_EXE}
else
	LIBS+=-lm
	UNAME_S := $(shell uname -s)
	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_S),Linux)
		CC += -D LINUX
		LIBS_T=-lcheck -lsubunit -lrt -lm -lpthread

		ifeq ($(UNAME_P),x86_64)
			LIBRARIES_VERSION=linux_amd64
		else ifneq ($(filter %86,$(UNAME_P)),)
			LIBRARIES_VERSION=linux_i386
		else ifneq ($(filter arm%,$(UNAME_P)),)
			WARNS+=WARNING: We do not have raylib build for Linux ARM architecture. Going from there with AMD64 build, idk.
			LIBRARIES_VERSION=linux_amd64
		else
			WARNS+=WARNING: Failed to determiтe processor architecture. Will assume linux_amd64.
			LIBRARIES_VERSION=linux_amd64
		endif
	endif
	ifeq ($(UNAME_S),Darwin)
		CC += -D OSX
		LIBS_T=-lcheck -lm
		LIBS+=-lobjc -framework Cocoa -framework IOKit
		DYLIB_EXT=.dylib
		LIBRARIES_VERSION=macos
	endif

	ifeq ($(UNAME_P),x86_64)
		CC+=-D AMD64
	endif
	ifneq ($(filter %86,$(UNAME_P)),)
		CC+=-D IA32
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		CC+=-D ARM
	endif
endif

LIBRARIES_DIR=../libraries/${LIBRARIES_VERSION}
INCLUDES+= -isystem ${LIBRARIES_DIR}/include
LIBS_SRC+=-L${LIBRARIES_DIR}/lib

BUILD_DIR=build
TARGET_FILE=${BUILD_DIR}/3dviewer${EXEC_EXT}
TEST_BIN=tests/s21_test${EXEC_EXT}
GCOV_BIN=gcov_bin${EXEC_EXT}

# install, uninstall, clean, dvi, dist, test, gcov_report
all: run

run: ${TARGET_FILE}
	./${TARGET_FILE}

build: ${TARGET_FILE}

install: build
	${MKDIR} ../3DViewer
	${CP} build/* ../3DViewer

uninstall: 
	${RMRF} ../3DViewer

rebuild:
	${RMRF} build
	make build

test: ${TEST_BIN}
	./${TEST_BIN}

dist: clean
	cd .. && tar -czvf s21_3DViwer.tar.gz sane_windows include libraries src
dvi:
	open dvi.md

gcov_report: ${GCOV_BIN}
	./${GCOV_BIN}
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info

# Build
SOURCES=$(wildcard */*.h) $(wildcard */*.c) $(wildcard */*/*.h) $(wildcard */*/*.c) $(wildcard */*/*/*.h) $(wildcard */*/*/*.c)
C_SOURCES=$(filter %.c,$(SOURCES))
H_SOURCES=$(filter %.h,$(SOURCES))
OBJ_FILES=$(C_SOURCES:.c=.reg.o)
GCOV_OBJ_FILES=$(C_SOURCES:.c=.gcov.o)
REQUIRED_GCOV_OBJS=$(filter s21_matrix/%,$(GCOV_OBJ_FILES)) $(filter tests/%,$(GCOV_OBJ_FILES)) obj_parser/obj_parser.gcov.o

# Modules
UTIL_OBJS=$(filter util/%,$(OBJ_FILES))
UI_OBJS=$(filter ui/%,$(OBJ_FILES))
OBJ_PARSER_OBJS=$(filter obj_parser/%,$(OBJ_FILES))
S21_MATRIX_OBJS=$(filter s21_matrix/%,$(OBJ_FILES))
TESTS_OBJS=$(filter tests/%,$(OBJ_FILES))

OTHER_SOURCES=$(wildcard *.h) $(wildcard *.c)
OTHER_C_SOURCES=$(filter %.c,$(OTHER_SOURCES))
OTHER_OBJS=$(OTHER_C_SOURCES:.c=.reg.o)


${TEST_BIN}: ${TESTS_OBJS} util.a s21_matrix.a obj_parser.a
	${CC} -g ${TESTS_OBJS} util.a s21_matrix.a obj_parser.a util.a s21_matrix.a obj_parser.a $(LIBS_T) -o ${TEST_BIN}

util.a: ${UTIL_OBJS}
	ar -rc util.a ${UTIL_OBJS}
	ranlib util.a
	
ui.a: ${UI_OBJS}
	ar -rc ui.a ${UI_OBJS}
	ranlib ui.a

s21_matrix.a: ${S21_MATRIX_OBJS}
	ar -rc s21_matrix.a ${S21_MATRIX_OBJS}
	ranlib s21_matrix.a  

obj_parser.a: ${OBJ_PARSER_OBJS}
	ar -rc obj_parser.a ${OBJ_PARSER_OBJS}
	ranlib obj_parser.a



# Target file 
${TARGET_FILE}: ${OTHER_OBJS} ui.a util.a s21_matrix.a obj_parser.a | ${BUILD_DIR}/assets ${LIBRARIES_DIR}/lib.cache ${MKDIR_EXE} ${CP_EXE}
	@echo
	@echo ===== BUILDING EXECUTABLE =====
	${MKDIR} ${BUILD_DIR}
	${CP} ${LIBRARIES_DIR}/lib/*${DYLIB_EXT}* ${BUILD_DIR}/
	${CC} $^ ui.a util.a s21_matrix.a obj_parser.a ${LIBS_SRC} ${LIBS} -o ${TARGET_FILE}
	@echo ===== BUILDING DONE =====
	@echo

${GCOV_BIN}: ${REQUIRED_GCOV_OBJS} util.a
	${CC} -lgcov --coverage ${REQUIRED_GCOV_OBJS} util.a ${LIBS_SRC} ${LIBS_T} -o $@

${BUILD_DIR}/assets: assets/*
	${CP} assets ${BUILD_DIR}/

# This thing just builds any .o file
%.reg.o: %.c | ${H_SOURCES} ${LIBRARIES_DIR}/lib.cache
	${CC} -c -fPIC $< ${INCLUDES} -o $@

# This thing just builds any .o file
%.gcov.o: %.c | ${H_SOURCES} ${LIBRARIES_DIR}/lib.cache
	${CC} -g -fprofile-arcs -ftest-coverage -c -fPIC $< ${INCLUDES} -o $@

# This target unpacks libraries
lib: ${LIBRARIES_DIR}/lib.cache

../libraries/%/lib.cache: ../libraries/compressed.tar.gz
	@echo
	@echo ===== UNPACKING LIBRARIES =====
	@echo Nevermind ${WARNS}
	tar -C ../libraries -xf $< $*
	echo 1 > $@
	@echo ===== UNPACKING DONE =====
	@echo
	
# This target unpacks utilities for windows
../sane_windows/%.exe: ../sane_windows/sane_utils.tar.gz
	tar -C ../sane_windows -xf $< $*.exe

clean_lite: | ${RMRF_EXE}
	${RMRF} *.o
	${RMRF} */*.o
	${RMRF} */*/*.o
	${RMRF} *.gcov*
	${RMRF} */*.gcov*
	${RMRF} */*/*.gcov*
	${RMRF} *.gcno*
	${RMRF} */*.gcno*
	${RMRF} */*/*.gcno*
	${RMRF} *.a
	${RMRF} */*.a
	${RMRF} */*/*.a
	${RMRF} ${TARGET_FILE}
	${RMRF} obj_parser_bin

clean: clean_lite | ${RMRF_EXE}
	${RMRF}	lib.cache
	${RMRF}	${BUILD_DIR}
	${RMRF}	../libraries/linux_amd64
	${RMRF}	../libraries/linux_i386
	${RMRF}	../libraries/macos
	${RMRF}	../libraries/win32_mingw-w64
	${RMRF}	../libraries/win64_mingw-w64
	${RMRF}	.clang-format
	${RMRF}	report
	${RMRF} ${GCOV_BIN}
	${RMRF}	test.info
	${RMRF} smartcalc-verdaqui-dist.tar.gz
	${RMRF} ${TEST_BIN}

gitignore:
	mv ../.gitignore ../.gitignore-original
	mv ../.localignore ../.gitignore

ungitignore:
	mv ../.gitignore ../.localignore
	mv ../.gitignore-original ../.gitignore

# Formatting code
.clang-format: ../materials/linters/.clang-format | ${CP_EXE}
	${CP} $< $@

format: .clang-format
	clang-format -i ${C_SOURCES} ${H_SOURCES}
