#ifndef __PORTABILITY
#define __PORTABILITY

#if defined(_WIN32)
#  include <stdint.h>
int GetErrnoFromWinsockError(long lWsaError);
void SetErrnoFromWinsockError(long lWsaError);
int* _wsaerrno();
#  define wsaerrno (*_wsaerrno())
#  define poll WSAPoll
#  define strerror_r(errnum, buf, buflen) strerror_s(buf, buflen, errnum)
#  define readsocket(fd,buf,bufsize) recv(fd,buf,bufsize,0)
#  define writesocket(fd,buf,bufsize) send(fd,buf,bufsize,0)
#else
#  define INVALID_SOCKET (-1)
#  define closesocket(fd) close(fd)
#  define readsocket(fd,buf,bufsize) read(fd,buf,bufsize)
#  define writesocket(fd,buf,bufsize) write(fd,buf,bufsize)
#endif

#endif
