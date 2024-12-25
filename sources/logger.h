#ifndef LOGGER2_H
#define LOGGER2_H

typedef enum {
    ERROR,
    INFO,
    WARNING,
    DEBUG_
} LogType;

void Log(char *, LogType, ...);



#endif
