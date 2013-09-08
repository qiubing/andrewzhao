#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

#define IPV6_ALL_MC_FORWARD "/proc/sys/net/ipv6/conf/all/mc_forwarding"

int main()
{
        int fd;

        fd = open(IPV6_ALL_MC_FORWARD, O_WRONLY);
        if (fd < 0) {
		perror("open failed");
                return 1;
        } else {
                if (-1 == write(fd, "1", 1)) {
			perror("write failed");
                        (void)close(fd);
                        return 1;
                }
                (void)close(fd);
        }

        return 0;
}
