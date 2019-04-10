cmake_minimum_required(VERSION 3.12)
project(FastCap0_1 C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(FastCap0_1
        src/blkDirect.c
        src/busgen.c
        src/calcp.c
        src/capsolve.c
        src/cubegen.c
        src/direct.c
        src/disrect.c
        src/distri.c
        src/electric.c
        src/electric_mod.c
        src/electric_old.c
        src/epsilon.c
        src/fastcap.c
        src/input.c
        src/mulDisplay.c
        src/mulDo.c
        src/mulGlobal.c
        src/mulGlobal.h
        src/mulLocal.c
        src/mulMats.c
        src/mulMats_mod.c
        src/mulMats_old.c
        src/mulMulti.c
        src/mulSetup.c
        src/mulStruct.h
        src/patran.c
        src/patran.h
        src/pipedgen.c
        src/pltcapgen.c
        src/psMatDisplay.c
        src/quickif.c
        src/quickif.h
        src/resusage.h
        src/savemat_mod.c
        src/teragen.c
        src/uglyalloc.c
        src/zbuf2fastcap.c
        src/zbufGlobal.h
        src/zbufInOut.c
        src/zbufProj.c
        src/zbufSort.c
        src/zbufStruct.h)
