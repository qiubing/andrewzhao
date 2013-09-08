#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "../src/rcsid.h"
RCSID("$Id: usleep.c,v 1.4 2000/01/27 11:25:30 misiek Exp $")

#include <sys/time.h>
#include <sys/select.h>

int usleep(unsigned usecs)
{
	struct timeval t;
	
	t.tv_sec = usecs/10000000;
	t.tv_usec = usecs%1000000;
	(void) select(1, NULL, NULL, NULL, &t);
	return 0;
}

