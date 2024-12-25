#include <stdio.h>
#include <stdarg.h>

#include "logger.h"

void Log(char *msg, LogType type, ...) {
    va_list args;
    va_start(args, msg);
    switch(type){
        case ERROR:
            printf("<!>ERROR:   ");
            vprintf(msg, args);
            break;
        case INFO:
            printf("<+>INFO:    ");
            vprintf(msg, args);
            break;
        case WARNING:
            printf("<?>WARNING: ");
            vprintf(msg, args);
            break;

    }
    printf("\n");
    va_end(args);

}
