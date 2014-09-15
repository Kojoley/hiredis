#include "portability.h"

#include <errno.h>
#include <winsock2.h>

/* MinGW lacks some of error codes in errno.h */
#define ESOCKTNOSUPPORT 44      /* Socket type not supported */
#define EPFNOSUPPORT    46      /* Protocol family not supported */
#define ESHUTDOWN       58      /* Cannot send after transport endpoint shutdown */
#define ETOOMANYREFS    59      /* Too many references: cannot splice */
#define EHOSTDOWN       64      /* Host is down */
#define EPROCLIM        67      /* SUNOS: Too many processes */
#define EUSERS          68      /* Too many users */
#define EDQUOT          69      /* Quota exceeded */
#define ESTALE          70      /* Stale file handle */
#define EREMOTE         71      /* Object is remote */
#define ENODATA         111     /* No data available */
#define EREMOTEIO       121     /* Remote I/O error */

int* _wsaerrno()
{
  static int __wsaerrno = 0;
  return &__wsaerrno;
}

int GetErrnoFromWinsockError(long lWsaError)
{
  switch (lWsaError) {
    case 0:                  return 0;
    case WSAEINTR:           return EINTR;
    case WSAEWOULDBLOCK:     return EWOULDBLOCK;
    case WSAEINPROGRESS:     return EINPROGRESS;
    case WSAEALREADY:        return EALREADY;
    case WSAENOTSOCK:        return ENOTSOCK;
    case WSAEDESTADDRREQ:    return EDESTADDRREQ;
    case WSAEMSGSIZE:        return EMSGSIZE;
    case WSAEPROTOTYPE:      return EPROTOTYPE;
    case WSAENOPROTOOPT:     return ENOPROTOOPT;
    case WSAEPROTONOSUPPORT: return EPROTONOSUPPORT;
    case WSAESOCKTNOSUPPORT: return ESOCKTNOSUPPORT;
    case WSAEOPNOTSUPP:      return EOPNOTSUPP;
    case WSAEPFNOSUPPORT:    return EPFNOSUPPORT;
    case WSAEAFNOSUPPORT:    return EAFNOSUPPORT;
    case WSAEADDRINUSE:      return EADDRINUSE;
    case WSAEADDRNOTAVAIL:   return EADDRNOTAVAIL;
    case WSAENETDOWN:        return ENETDOWN;
    case WSAENETUNREACH:     return ENETUNREACH;
    case WSAENETRESET:       return ENETRESET;
    case WSAECONNABORTED:    return ECONNABORTED;
    case WSAECONNRESET:      return ECONNRESET;
    case WSAENOBUFS:         return ENOBUFS;
    case WSAEISCONN:         return EISCONN;
    case WSAENOTCONN:        return ENOTCONN;
    case WSAESHUTDOWN:       return ESHUTDOWN;
    case WSAETOOMANYREFS:    return ETOOMANYREFS;
    case WSAETIMEDOUT:       return ETIMEDOUT;
    case WSAECONNREFUSED:    return ECONNREFUSED;
    case WSAELOOP:           return ELOOP;
    case WSAENAMETOOLONG:    return ENAMETOOLONG;
    case WSAEHOSTDOWN:       return EHOSTDOWN;
    case WSAEHOSTUNREACH:    return EHOSTUNREACH;
    case WSAENOTEMPTY:       return ENOTEMPTY;
    case WSAEPROCLIM:        return EPROCLIM;
    case WSAEUSERS:          return EUSERS;
    case WSAEDQUOT:          return EDQUOT;
    case WSAESTALE:          return ESTALE;
    case WSAEREMOTE:         return EREMOTE;
    case WSAEINVAL:          return EINVAL;
    case WSAEFAULT:          return EFAULT;
    case WSANO_DATA:         return ENODATA;

    default:                 return ESTALE;
  }
}

void SetErrnoFromWinsockError(long lWsaError)
{
  wsaerrno = lWsaError;
  errno = GetErrnoFromWinsockError(lWsaError);
}
