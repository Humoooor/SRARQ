#ifndef ERROR_H
#define ERROR_H

void Log();
void WarnLog();
void ErrLog();
void EndLog();

#define myLog(...) Log();printf(__VA_ARGS__);EndLog()
#define myWarnLog(...) WarnLog();printf(__VA_ARGS__);EndLog()
#define myErrLog(...) ErrLog();printf(__VA_ARGS__);EndLog()

#endif
