#include <sys/ioctl.h>
#include <net/if.h> 
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
//#include <sys/types.h>
#include <ifaddrs.h>

#define MAC_LEN 6       // MAC addr size 
#define IFNAME_LEN 6    // max char for a interface name
#define MAX_IF_NUM 10  // max interfaces can be found

int find_if_name(int if_max, char p_ifn_rt[][IFNAME_LEN]) {
  int num = 0, check;
  struct ifaddrs* ifh = NULL;
  struct ifaddrs* ifa = NULL;

  getifaddrs(&ifh);
  for(ifa=ifh;ifa!=NULL; ifa=ifa->ifa_next) {
    for(check=0; check<num; check++) {
      if( strncmp(ifa->ifa_name, p_ifn_rt[check], IFNAME_LEN)==0 ) break;
    }

    if( check<num ) ; // the ifa_flags already exist
    else {
      strncpy(p_ifn_rt[num], ifa->ifa_name, IFNAME_LEN);
      num++;
      if( num == if_max ) break; // too many interface
    }
  }

  freeifaddrs(ifh);
  return num;
}

int main(int argc, char* argv[]) {
  struct ifreq ifr;
  int sock_fd, num_if, i, j;
  char p_ifn_arr[MAX_IF_NUM][IFNAME_LEN];

  /* we need a socket fd to operate ioctl */
  /* family and type are not important here */
  sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
  if( sock_fd==-1 ) {
    printf("(ERR) Cannot setup socket: %s", strerror(errno));
    return errno;
  }

  strcpy(p_ifn_arr[0], "");
  num_if = find_if_name(10, p_ifn_arr);//MAX_IF_NUM, &p_ifn_arr);

  for( j=0; j<num_if; j++) {
    printf("try use if %s\n", p_ifn_arr[j]);
    strcpy(ifr.ifr_name, p_ifn_arr[j]);
    if( strcmp("lo", p_ifn_arr[j])==0 ) continue;

    if( 0==ioctl(sock_fd, SIOCGIFHWADDR, &ifr) ) {
      printf("ifr_ifindex of %s : %d\n", p_ifn_arr[j], ifr.ifr_ifindex);
      for (i = 0; i < MAC_LEN; ++i)
        printf(" %x", (unsigned char) ifr.ifr_addr.sa_data[i]);
      puts("\n");
    } else {
      printf("(ERR) Cannot get info of %s: %s\n", p_ifn_arr[j], strerror(errno));
    }
  }

  if( sock_fd ) close(sock_fd);
  return 0;
}

