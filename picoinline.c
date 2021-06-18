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

int main()
{
    int ParamCount = 1;
    int DontRunMain = false;
    int StackSize = PICOC_STACK_SIZE;
    Picoc pc;
    PicocInitialize(&pc, StackSize);

   

 
        if (PicocPlatformSetExitPoint(&pc)) {
            PicocCleanup(&pc);
            return pc.PicocExitValue;
        }

        //fo//r (; ParamCount < argc && strcmp(argv[ParamCount], "-") != 0; ParamCount++)
            //PicocPlatformScanFile(&pc, argv[ParamCount]);
		
			PicoStringCode(&pc,"try1.c", "#include <stdio.h>\r\n int main(){ int i = 123 * 3 %99;  printf(\"hi there %i\\r\\n\", i); return 0;} ");

        //if (!DontRunMain)
            PicocCallMain(&pc,0, "");
 

    PicocCleanup(&pc);
    return pc.PicocExitValue;
}
