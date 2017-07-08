
#ifdef DEBUG
#define DEBUG_MSG(fmt,...)				\
  printf(fmt,##__VA_ARGS__)

#else
#define DEBUG_MSG(fmt,...)
#endif
