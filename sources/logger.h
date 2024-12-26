#ifndef LOGGER2_H
#define LOGGER2_H

typedef enum {
    _ERROR_,
    _INFO_,
    _WARNING_,
    _DEBUG_
} LogType;

void Log(char *, LogType, ...);



#endif
