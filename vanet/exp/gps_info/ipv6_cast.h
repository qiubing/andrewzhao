/*
 * vanet ipv6_cast header file
 */
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>

#define BUFLEN 1024
#define BUFLEN_SHORT 128

#define IPV6_FLOWINFO 11
#define VANET_SENDMSG_FLAG 0x20000

union hoplimit_control {
	struct cmsghdr cmsg;
	char control[CMSG_SPACE(sizeof(int))];
};

struct flowlabel_control {
	struct cmsghdr cmsg;
	unsigned char cmsg_data[4];
};

/*
 * GPS
 */
//#define GPSDEVICE "/dev/ttyS1"
//#define GPSDEVICE "./gps_info_whole"
#define GPSDEVICE "/dev/ttyUSB0"
#define GPS_REOPEN_TIME 3 // in second
#define VANET_DEFAULT_MSG "N vanet information without GPS device\n"
#define TIMELEN 11
#define LATITUDELEN 12
#define LONGITUDELEN 13

extern pthread_mutex_t parsed_gps_info_lock;
extern char parsed_gps_info[];

/*
 * NODE information for upper layer application
 */
#define NODE_NAME_LEN 32
#define NODE_KEEP_ALIVE 8
#define NI_QUERY_SERVER_PORT 28535
#define NI_QUERY_SERVER_IP "127.0.0.1"
// every pass-through, node's alive--;
// when alive meet to 0, just delete this node.
struct node_info {
	struct node_info *next;
	char name[NODE_NAME_LEN];
	struct in6_addr ip6addr;
	int alive;
	char lat[12];
	char lon[12];
	char speed[8];
	char direction[8];
};

struct node_info_stack {
	pthread_mutex_t lock;
	struct node_info *top;
	unsigned int count;
};

/********************************
  OBU <-------------> OBU <-> C&D

+-----+                     +-----+
|OBU1 |               	    |OBU0 |
|     |                	    |     |
|     |  unicast      	    |     |
|   *-----------> OBU_LISTEN_PORT |
|     |                	    |     |
|     | broadcast    	    |     |
|   *-----> OBU_LISTEN_BCAST_PORT |
|     |                     |     |           ipv4
|     |                     |   *------------------------+
|     |                     |     |            		 |
|     |          OBU_CONTROL_PORT |             	 |
|     |           ^         |     |             	 |
+-----+           |         +-----+             	 |
	     ipv4 |	                        	 |
             +----|------------------------+    	 |
	     |	  *			   |    	 |
	     |			       CTRL_DISP_PORT <--+
	     |				   |
	     | OBU0's C&D module           |
	     +-----------------------------+

********************************/
/**
 * OBU listen
 */
#define OBU_LISTEN_PORT 30000
#define OBU_LISTEN_BCAST_PORT 30001
#define OBU_LISTEN_BCAST_ADDR "ff05::39"
#define OBU_BCAST_NODE_NAME "ALL"
#define OBU_BW_TEST_PORT 30002
#define OBU_BW_TEST_RESP_PORT 30003
#define VANET_DEVICE_NAME "a5k0"
#define VANET_UC_HOPLIMIT_DEFAULT 4
#define VANET_BCAST_HOPLIMIT_DEFAULT 3

/*
 * Control & Display module
 */
// CTRL_DISP_PORT located in PC or any device else which used for controlling OBU and displaying info.
#define CTRL_DISP_PORT 40000
// Control & Display module's IPv4 address
#define CTRL_DISP_ADDRESS "192.168.10.200"
// When sendto C&D module messages failed, retry interval
#define CTRL_DISP_RETRY_INTERVAL 100

// OBU_CONTROL_PORT located in PowerPC used for receiving control instructions from C&D module, eg. PC
#define OBU_CONTROL_PORT 50000

// 4096 bits = 512 bytes
#define RTT_TEST_DATALEN 512

/*
 * functions prototypes
 */
void error(const char *msg);
void get_node_name(const char *buf, char *name);

int udp_connect(const char *addr, int port);
int udp_bind(const char *addr, int port);

int gps_open_port(const char *dev);
int set_port_speed(const int fd, const int speed);
int gps_write_port(const int fd, const char *cmd, const int len);
int gps_read_port(const int fd, char *buf, const int len);
int gps_set_freq(const int fd);
void *thr_parse_gpsinfo();

struct node_info *new_ni(char *, struct in6_addr *);
int get_ni_addr_thread_safe(struct node_info_stack *, char *, struct in6_addr *);
struct node_info *find_ni(struct node_info_stack *, char *);
void push_ni_thread_safe(struct node_info_stack *, struct node_info *);
void *thr_ni_query_server(void *arg);
int get_node_ip6addr(char nodename[], char *addr);

void tv_sub(struct timeval *in, struct timeval *out);
