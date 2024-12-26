#include <stdio.h>
#include <stdarg.h>

#include "logger.h"

void Log(char *msg, LogType type, ...) {
    va_list args;
    va_start(args, msg);
    switch(type){
        case _ERROR_:
            printf("<!>ERROR: ");
            vprintf(msg, args);
            printf("\n");
            break;
        case _INFO_:
            printf("<+>INFO: ");
            vprintf(msg, args);
            printf("\n");
            break;
        case _WARNING_:
            printf("<?>WARNING: ");
            vprintf(msg, args);
            printf("\n");
            break;
        case _DEBUG_:
            #ifdef DEBUG
                printf("<*>DEBUG: ");
                vprintf(msg, args);
                printf("\n");
            #endif
            break;

    }
    va_end(args);

}
