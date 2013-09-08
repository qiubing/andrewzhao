/*
 * main.c	main loop of emulator.
 *
 *		This file is part of the minicom communications package,
 *		Copyright 1991-1995 Miquel van Smoorenburg.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * fmg 1/11/94 color mods
 * jl  22.06.97 log it when DCD drops
 * jl  02.06.98 added call duration to the "Gone offline" log line
 * jl  14.06.99 moved lockfile creation to before serial port opening
 *
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "rcsid.h"
RCSID("$Id: main.c,v 1.7 2000/10/27 14:19:38 walker Exp $")

#include "port.h"
#include "minicom.h"
#include "intl.h"
#if defined(__linux__) || (defined(BSD) && (BSD >= 199306))
#include <errno.h>
#endif

#ifdef SVR4_LOCKS
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mkdev.h>
#endif /* SVR4_LOCKS */

static jmp_buf albuf;

/* Compile SCCS ID into executable. */
char *Version = VERSION;

void curs_status();

/*
 * Return the last part of a filename.
 */
char *mbasename(s)
char *s;
{
  char *p;
  
  if((p = strrchr(s, '/')) == (char *)NULL)
  	p = s;
  else
  	p++;
  return(p);
}

/*
 * Leave.
 */
void leave(s)
char *s;
{
  if (stdwin) wclose(stdwin, 1);
  if (portfd > 0) {
	m_restorestate(portfd);
	close(portfd);
  }
  set_privs();
  if (lockfile[0]) unlink(lockfile);
  if (P_CALLIN[0]) (void) fastsystem(P_CALLIN, CNULL, CNULL, CNULL);
  if (real_uid) chown(dial_tty, (uid_t)portuid, (gid_t)portgid);
  fprintf(stderr, "%s", s);
  exit(1);
}

/*
 * Return text describing command-key.
 */
char *esc_key()
{
  static char buf[16];

  if (!alt_override && P_ESCAPE[0] == '^' && P_ESCAPE[1] != '[') {
	sprintf(buf, "CTRL-%c ", P_ESCAPE[1]);
	return(buf);
  }
#if defined(_COHERENT) || defined(i386) || defined(__i386__)
  sprintf(buf, "ALT-");
#else
  sprintf(buf, "Meta-");
#endif
  return(buf);
}

/*ARGSUSED*/
static void get_alrm(dummy)
int dummy;
{
  (void)dummy;
  longjmp(albuf, 1);
}

/*
 * Open the terminal.
 */
int open_term(doinit)
int doinit;
{
  struct stat stt;
  char buf[128];
  int fd, n = 0;
  int pid;
  int mask;
#if defined(__linux__) || (defined(BSD) && (BSD >= 199306))
  int s_errno;
#endif

  /* Upgrade our status. */
  set_privs();

  /* First see if the lock file directory is present. */
  if (P_LOCK[0] && stat(P_LOCK, &stt) == 0) {

#ifdef SVR4_LOCKS
    stat(dial_tty, &stt);
    sprintf(lockfile, "%s/LK.%03d.%03d.%03d", P_LOCK, major(stt.st_dev), major(stt.st_rdev), minor(stt.st_rdev));

#else /* SVR4_LOCKS */
    snprintf(lockfile, sizeof(lockfile), "%s/LCK..%s", P_LOCK, mbasename(dial_tty));

#endif /* SVR4_LOCKS */

  }
  else
    lockfile[0] = 0;

  if (doinit > 0 && lockfile[0] && (fd = open(lockfile, O_RDONLY)) >= 0) {
	n = read(fd, buf, 127);
	close(fd);
	if (n > 0) {
		pid = -1;
		if (n == 4)
			/* Kermit-style lockfile. */
			pid = *(int *)buf;
		else {
			/* Ascii lockfile. */
			buf[n] = 0;
			sscanf(buf, "%d", &pid);
		}
		if (pid > 0 && kill((pid_t)pid, 0) < 0 &&
			errno == ESRCH) {
		    fprintf(stderr, _("Lockfile is stale. Overriding it..\n"));
		    sleep(1);
		    unlink(lockfile);
		} else
		    n = 0;
	}
	if (n == 0) {
  		if (stdwin != NIL_WIN) wclose(stdwin, 1);
  		fprintf(stderr, _("Device %s is locked.\n"), dial_tty);
		drop_privs();
		return(-1);
	}
  }

  if (doinit > 0 && lockfile[0]) {
  	/* Create lockfile compatible with UUCP-1.2 */
	mask = umask(022);
#ifdef _COH3
	if ((fd = creat(lockfile, 0666)) < 0) {
#else
  	if ((fd = open(lockfile, O_WRONLY | O_CREAT | O_EXCL, 0666)) < 0) {
#endif
  		if (stdwin != NIL_WIN) wclose(stdwin, 1);
  		fprintf(stderr, _("Cannot create lockfile. Sorry.\n"));
		drop_privs();
		return(-1);
  	}
	(void) umask(mask);
	(void) chown(lockfile, (uid_t)real_uid, (gid_t)real_gid);
  	snprintf(buf, sizeof(buf), "%10ld minicom %.20s\n", (long)getpid(), username);
  	write(fd, buf, strlen(buf));
  	close(fd);
  }

  /* Run a special program to disable callin if needed. */
  if (doinit > 0 && P_CALLOUT[0]) {
	if(fastsystem(P_CALLOUT, CNULL, CNULL, CNULL) < 0) {
  		if (stdwin != NIL_WIN) wclose(stdwin, 1);
  		fprintf(stderr, _("Could not setup for dial out.\n"));
  		if (lockfile[0]) unlink(lockfile);
		drop_privs();
		return(-1);
  	}
  }

  /* Now open the tty device. */
  if (setjmp(albuf) == 0) {
	portfd = -1;
	signal(SIGALRM, get_alrm);
	alarm(4);
#if defined(O_NDELAY) && defined(F_SETFL)
	portfd = open(dial_tty, O_RDWR|O_NDELAY);
	if (portfd >= 0){
		/* Cancel the O_NDELAY flag. */
		n = fcntl(portfd, F_GETFL, 0);
		(void) fcntl(portfd, F_SETFL, n & ~O_NDELAY);
	}
#else
	portfd = open(dial_tty, O_RDWR);
#endif
	if (portfd >= 0) {
		if (doinit > 0) m_savestate(portfd);
		port_init();
	}
  }
#if defined(__linux__) || (defined(BSD) && (BSD >= 199306))
  s_errno = errno;
#endif
  alarm(0);
  signal(SIGALRM, SIG_IGN);
  if (portfd < 0) {
	if (doinit > 0) {
  		if (stdwin != NIL_WIN) wclose(stdwin, 1);
#if defined(__linux__) || (defined(BSD) && (BSD >= 199306))
		fprintf(stderr, _("minicom: cannot open %s: %s\n"),
			dial_tty, strerror(s_errno));
#else
  		fprintf(stderr, _("minicom: cannot open %s. Sorry.\n"), dial_tty);
#endif
		if (lockfile[0]) unlink(lockfile);
		drop_privs();
		return(-1);
	}
	if (lockfile[0]) unlink(lockfile);
	werror(_("Cannot open %s!"), dial_tty);
	drop_privs();
	return(-1);
  }

  /* Remember owner of port */
  stat(dial_tty, &stt);
  portuid = stt.st_uid;
  portgid = stt.st_gid;

  /* Give it to us! */
  if (real_uid != 0) chown(dial_tty, (uid_t)real_uid, (gid_t)real_gid);

  /* Set CLOCAL mode */
  m_nohang(portfd);

  /* Set Hangup on Close if program crashes. (Hehe) */
  m_hupcl(portfd, 1);
  if (doinit > 0) m_flush(portfd);
  drop_privs();
  return(0);
}


/* Function to write output. */
static void do_output(s, len)
char *s;
int len;
{
  char buf[256];
  int f;

  if (len == 0) len = strlen(s);

  if (P_PARITY[0] == 'M') {
	for(f = 0; f < len && f < 256; f++)
		buf[f] = *s++ | 0x80;
	write(portfd, buf, f);
  } else
	write(portfd, s, len);
}

/* Function to handle keypad mode switches. */
static void kb_handler(a, b)
int a,b;
{
  cursormode = b;
  keypadmode = a;
  if (st) curs_status();
}

/*
 * Initialize screen and status line.
 */
void init_emul(type, do_init)
int type;
int do_init;
{
  int x = -1, y = -1;
  char attr = 0;
  int maxy;
  int ypos;
  extern int use_status;

  if (st != NIL_WIN) {
  	wclose(st, 1);
  	tempst = 0;
  	st = NIL_WIN;
  }

  if (us != NIL_WIN) {
	x = us->curx; y = us->cury;
	attr = us->attr;
  	wclose(us, 0);
  }	

  /* See if we have space for a fixed status line */
  maxy = LINES - 1;
  if ((use_status || LINES > 24)
		&& P_STATLINE[0] == 'e') {
	if (use_status) {
		ypos = LINES;
		maxy = LINES - 1;
	} else {
		ypos = LINES - 1;
		maxy = LINES - 2;
	}
	st = wopen(0, ypos, COLS - 1, ypos, BNONE,
		st_attr, sfcolor, sbcolor, 1, 0, 1);
	wredraw(st, 1);
  }

  /* MARK updated 02/17/95 - Customizable size for history buffer */
  num_hist_lines = atoi(P_HISTSIZE);
  if (num_hist_lines < 0) num_hist_lines = 0;
  if (num_hist_lines > 5000) num_hist_lines = 5000;

  /* Open a new main window, and define the configured history buffer size. */
  us = wopen(0, 0, COLS - 1, maxy,
             BNONE, XA_NORMAL, tfcolor, tbcolor, 1, num_hist_lines, 0);

  if (x >= 0) {
  	wlocate(us, x, y);
  	wsetattr(us, attr);
  }

  us->autocr = 0;
  us->wrap = wrapln;

  terminal = type;
  lines = LINES - (st != NIL_WIN);
  cols = COLS;
  
  /* Install and reset the terminal emulator. */
  if (do_init) {
  	vt_install(do_output, kb_handler, us);
	vt_init(type, tfcolor, tbcolor, us->wrap, 0);
  } else
	vt_pinit(us, -1, -1);

  if (st != NIL_WIN) show_status();
}

/*
 * Locate the cursor at the correct position in
 * the user screen.
 */
static void ret_csr()
{
  wlocate(us, us->curx, us->cury);
  wflush();
}

/*
 * Show baudrate, parity etc.
 */
void mode_status()
{
  if (st) { /* if swich off status line - NOT print !!! (vlk@st.simbirsk.su) */
    wlocate(st, 20, 0);
    if (P_SHOWSPD[0] == 'l')
      wprintf(st, "%6ld %s%s%s", linespd, P_BITS, P_PARITY, P_STOPB);
    else
      wprintf(st, "%6.6s %s%s%s", P_BAUDRATE, P_BITS, P_PARITY, P_STOPB);
    ret_csr();
  }
}

/*
 * Show offline or online time.
 * If real dcd is not supported, Online and Offline will be
 * shown in capitals.
 */
void time_status()
{
  if (!st)
    return;
  wlocate(st, 63, 0);
  if (online < 0)
  	wprintf(st, " %12.12s ", P_HASDCD[0] == 'Y' ? _("Offline") : _("OFFLINE"));
  else
  	wprintf(st," %s %02ld:%02ld", P_HASDCD[0] == 'Y' ? _("Online") : _("ONLINE"),
  		online / 3600, (online / 60) % 60);
  		
  ret_csr();
}

/*
 * Show in which mode the cursor keys are (normal or application)
 */
void curs_status()
{
  wlocate(st, 33, 0);
  wprintf(st, cursormode == NORMAL ? "NOR" : "APP");
  ret_csr();
}

time_t old_online = -1;

/*
 * Update the online time.
 */
static void updtime()
{

  if (old_online == online) return;
  if ((P_LOGCONN[0] == 'Y') && (old_online >= 0) && (online < 0)) {
    do_log(_("Gone offline (%ld:%02ld:%02ld)"),
      old_online / 3600, (old_online / 60) % 60, old_online % 60);
  }
  old_online = online;
  if (st != NIL_WIN) {
  	time_status();
  	ret_csr();
  }
  wflush();
}


/* Update the timer display. This can also be called from updown.c */
void timer_update()
{
  static time_t t1, start;
  int dcd_support = P_HASDCD[0] == 'Y';

  /* See if we're online. */
  if ((!dcd_support && bogus_dcd) || (dcd_support && m_getdcd(portfd))) {
	/* We are online at the moment. */
  	if (online < 0) {
		/* This was a transition from off to online */
  		time(&start);
  		t1 = start;
  		online = 0;
  		updtime();
#if _DCDFLOW
		/* DCD has gotten high, we can turn on hw flow control */
		if (P_HASRTS[0] == 'Y')
			m_sethwf(portfd, 1);
#endif
  	}
  } else {
	/* We are offline at the moment. */
#if _DCDFLOW
	if (online >= 0) {
		/* DCD has dropped, turn off hw flow control. */
		m_sethwf(portfd, 0);
	}
#endif
	if (online >= 0 && old_online >= 0) {
	  /* First update the timer for call duration.. */
	  time(&t1);
	  online = t1 - start;
	  updtime();
	}
	/* ..and THEN notify that we are now offline */
  	online = -1;
  	updtime();
  }

  /* Update online time */
  if (online >= 0) {
    time(&t1);
    online = t1 - start;
    if (online > (old_online + 59))
      updtime();
  }
}

/*
 * Show the status line 
 */
void show_status()
{
  st->direct = 0;
  wlocate(st, 0, 0);
  wprintf(st,
	  _(" %7.7sZ for help |           |     | Minicom %-6.6s |       | "),
	  esc_key(), VERSION);
  mode_status();
  time_status();
  curs_status();
  wlocate(st, 56, 0);
  switch(terminal) {
  	case VT100:
  		wputs(st, "VT102");
  		break;
  	case ANSI:
  		wputs(st, "ANSI");
  		break;
  }
  wredraw(st, 1);
  ret_csr();
}

/*
 * Show the name of the script running now.
 */
void scriptname(s)
char *s;
{
  if (st == NIL_WIN) return;
  wlocate(st, 39, 0);
  if (*s == 0)
  	wprintf(st, "Minicom %-6.6s", VERSION);
  else
      	wprintf(st, "script %-7.7s", s);
  ret_csr();
}

/*
 * Show status line temporarily
 */
static void showtemp()
{
  if (st != NIL_WIN) return;

  st = wopen(0, LINES - 1, COLS - 1, LINES - 1,
                BNONE, st_attr, sfcolor, sbcolor, 1, 0, 1);
  show_status();
  tempst = 1;
}

/*
 * The main terminal loop:
 *	- If there are characters recieved send them
 *	  to the screen via the appropriate translate function.
 */
int do_terminal()
{
  char buf[128];
  char *ptr;
  int c;
  int x;
  int blen;
  int zauto = 0;
  char *zsig = "**\030B00";
  int zpos = 0;
#if _HAVE_MACROS
  char *s;
#endif
  dirflush = 0;

dirty_goto:
  /* Show off or online time */
  updtime();

  /* If the status line was shown temporarily, delete it again. */
  if (tempst) {
  	tempst = 0;
  	wclose(st, 1);
  	st = NIL_WIN;
  }


  /* Auto Zmodem? */
  if (P_PAUTO[0] >= 'A' && P_PAUTO[0] <= 'Z') zauto = P_PAUTO[0];
  /* Set the terminal modes */
  (void) setcbreak(2); /* Raw, no echo */

  keyboard(KSTART, 0);

  /* Main loop */
  while(1) {
	/* See if window size changed */
	if (size_changed) {
		size_changed = 0;
#if 1
		/* I got the resize code going again! Yeah! */
		wclose(us, 0);
		us = NIL_WIN;
		if (st) wclose(st, 0);
		st = NIL_WIN;
		wclose(stdwin, 0);
                if (win_init(tfcolor, tbcolor, XA_NORMAL) < 0)
			leave(_("Could not re-initialize window system."));
		init_emul(terminal, 0);
#else
		werror(_("Resize not supported, screen may be messed up!"));
#endif
	}
	/* Update the timer. */
	timer_update();

	/* Check for I/O or timer. */
	x = check_io(portfd, 0, 1000, buf, &blen);

	/*  Send data from the modem to the screen. */
  	if ((x & 1) == 1) {
  		ptr = buf;
  		while(blen-- > 0) {
			/* Auto zmodem detect */
			if (zauto) {
				if (zsig[zpos] == *ptr) zpos++;
				  else zpos = 0;
			}
			if (P_PARITY[0] == 'M' || P_PARITY[0] == 'S')
				*ptr &= 0x7f;
  			vt_out(*ptr++);
			if (zauto && zsig[zpos] == 0) {
				dirflush = 1;
				keyboard(KSTOP, 0);
				updown('D', zauto - 'A');
				dirflush = 0;
				zpos = 0;
				blen = 0;
				goto dirty_goto;
			}
		}
  		wflush();
  	}
	
	/* Read from the keyboard and send to modem. */
	if ((x & 2) == 2) {
		/* See which key was pressed. */
		c = keyboard(KGETKEY, 0);
		if (c < 0) c += 256; /* XXX - shouldn't happen */

		/* Was this a command key? */
#ifndef RH_FIX
		if ((escape == 128 && c > 224 && c < 252) ||
#else   /* filipg 8/22/97: needed to recognize Fkeys in RedHat 4.1 */
		if ((escape == 128 && c > 128 && c < 256) ||
#endif /* RH_FIX */
		    (escape != 27 && c == escape) || 
		    (c > K_META)) {

			/* Stop keyserv process if we have it. */
  			keyboard(KSTOP, 0);

  			/* Restore keyboard modes */
  			(void) setcbreak(1); /* Cbreak, no echo */

  			/* Show status line temporarily */
  			showtemp();
  			if (c == escape) /* CTRL A */
				c = keyboard(KGETKEY, 0);
			if (c > K_META) c -= K_META;
  			if (c > 128) c -= 128;
  			if (c > ' ') {
				dirflush = 1;
				m_flush(0);
				return(c);
			}
			/* CTRLA - CTRLA means send one CTRLA */
#if 0
			write(portfd, &c, 1);
#else
			vt_send(c);
#endif
			goto dirty_goto;
  		}

		/* No, just a key to be sent. */
#if _HAVE_MACROS
		if (c >= K_F1 && c <= K_F10 && P_MACENAB[0] == 'Y') {
		    s = "";
		    switch(c) {
			case K_F1: s = P_MAC1; break;
			case K_F2: s = P_MAC2; break;
			case K_F3: s = P_MAC3; break;
			case K_F4: s = P_MAC4; break;
			case K_F5: s = P_MAC5; break;
			case K_F6: s = P_MAC6; break;
			case K_F7: s = P_MAC7; break;
			case K_F8: s = P_MAC8; break;
			case K_F9: s = P_MAC9; break;
			case K_F10: s = P_MAC10; break;
		    }
		    if (*s)
			mputs(s, 1);
		    else
			vt_send(c);
		} else
#endif
		vt_send(c);
	}
  }
}
