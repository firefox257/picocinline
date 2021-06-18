/* picoc main program - this varies depending on your operating system and
 * how you're using picoc */
/* platform-dependent code for running programs is in this file */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef DEBUGGER
static int gEnableDebugger = true;
#else
static int gEnableDebugger = false;
#endif



#include "cstdlib/ctype.c"
#include "cstdlib/errno.c"
#include "cstdlib/math.c"
#include "cstdlib/stdbool.c"
#include "cstdlib/stdio.c"
#include "cstdlib/stdlib.c"
#include "cstdlib/string.c"
#include "cstdlib/time.c"
//#include "cstdlib/unistd.c"

#include "platform/platform_unix.c"
#include "platform/library_unix.c"

#include "clibrary.c"
#include "debug.c"
#include "expression.c"
#include "heap.c"
#include "include.c"

#include "lex.c"
#include "parse.c"
#include "platform.c"
#include "table.c"
#include "type.c"
#include "variable.c"

/* include only picoc.h here - should be able to use it with only the
    external interfaces, no internals from interpreter.h */
#include "picoc.h"


#include "LICENSE.h"

/* Override via STACKSIZE environment variable */
#define PICOC_STACK_SIZE (128000*4)

int main(int argc, char **argv)
{
    int ParamCount = 1;
    int DontRunMain = false;
    int StackSize = getenv("STACKSIZE") ? atoi(getenv("STACKSIZE")) : PICOC_STACK_SIZE;
    Picoc pc;

    if (argc < 2 || strcmp(argv[ParamCount], "-h") == 0) {
        printf(PICOC_VERSION "  \n"
               "Format:\n\n"
               "> picoc <file1.c>... [- <arg1>...]    : run a program, calls main() as the entry point\n"
               "> picoc -s <file1.c>... [- <arg1>...] : run a script, runs the program without calling main()\n"
               "> picoc -i                            : interactive mode, Ctrl+d to exit\n"
               "> picoc -c                            : copyright info\n"
               "> picoc -h                            : this help message\n");
        return 0;
    }

    if (strcmp(argv[ParamCount], "-c") == 0) {
        printf("%s\n", (char*)&__LICENSE);
        return 0;
    }

    PicocInitialize(&pc, StackSize);

    if (strcmp(argv[ParamCount], "-s") == 0) {
        DontRunMain = true;
        PicocIncludeAllSystemHeaders(&pc);
        ParamCount++;
    }

    if (argc > ParamCount && strcmp(argv[ParamCount], "-i") == 0) {
        PicocIncludeAllSystemHeaders(&pc);
        PicocParseInteractive(&pc);
    } else {
        if (PicocPlatformSetExitPoint(&pc)) {
            PicocCleanup(&pc);
            return pc.PicocExitValue;
        }

        for (; ParamCount < argc && strcmp(argv[ParamCount], "-") != 0; ParamCount++)
            PicocPlatformScanFile(&pc, argv[ParamCount]);

        if (!DontRunMain)
            PicocCallMain(&pc, argc - ParamCount, &argv[ParamCount]);
    }

    PicocCleanup(&pc);
    return pc.PicocExitValue;
}


