#include <sys/types.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>

int funcWhatever(struct sockaddr* ptr, unsigned short fm, int size) {
  char host[NI_MAXHOST] = "";
  int s = getnameinfo(ptr, size, host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
  if( s!=0 )
    printf("getnameinfo() failed: %s\n", gai_strerror(s));
  else
    printf("address: <%s>\n", host);

  return 0;
}

int main () {

  struct ifaddrs* ifa;
  struct ifaddrs* ifh;
  int i;
  unsigned short family;

  getifaddrs(&ifh);
  // maintaining head pointer so we can free list later
  for(ifa=ifh; ifa!=NULL; ifa=ifa->ifa_next) {
    printf("ifa %p\t", ifa);
    printf("ifa_next %p\t", ifa->ifa_next);
    printf("ifa_name %s\t", ifa->ifa_name);
    printf("ifa_flags %d\n", ifa->ifa_flags);
    printf("ifa_addr: ");
    if( ifa->ifa_addr==NULL ) continue;
/* Display interface name and family (including symbolic
   form of the latter for the common families) */
    family = ifa->ifa_addr->sa_family;
    printf("sa_family (%d):", family);
    switch( family ) {
    case AF_PACKET:
      printf("AF_PACKET, ");
      break;
    case AF_INET:
      printf("AF_INET, ");
      funcWhatever(ifa->ifa_addr, family, sizeof(struct sockaddr_in));
      break;
    case AF_INET6:
      printf("AF_INET6, ");
      funcWhatever(ifa->ifa_addr, family, sizeof(struct sockaddr_in6));
      break;
    default:
      printf("????, ");
    }
    printf("go next\n");
  }

  freeifaddrs(ifh);
  return 0;
}

