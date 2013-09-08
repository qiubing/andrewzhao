/*
 * file.c	Functions to handle file selector.
 *
 *	This file is part of the minicom communications package,
 *	Copyright 1991-1995 Miquel van Smoorenburg.
 *
 *	This file created from code mostly cadged from "dial.c"
 *	by Miquel van Smoorenburg.  Written by James S. Seymour.
 *	Copyright (c) 1998 by James S. Seymour (jseymour@jimsun.LinxNet.com)
 *      Some mods for i18n  
 *      Copyright (c) 1998 by Arnaldo Carvalho de Melo <acme@conectiva.com.br>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "rcsid.h"
RCSID("$Id: file.c,v 1.7 2001/07/07 18:57:23 walker Exp $")

#include "port.h"
#include "minicom.h"
#include "intl.h"
#include "getsdir.h"

#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif

_PROTO(extern int wildmat, (char *s, char *p));

/* We want the ANSI offsetof macro to do some dirty stuff. */
#ifndef offsetof
#  define offsetof(type, member) ((int) &((type *)0)->member)
#endif

/* Values for the "flags". */
#define FL_ECHO		0x01	/* Local echo on/off. */
#define FL_DEL		0x02	/* Backspace or DEL */
#define FL_WRAP		0x04	/* Use autowrap. */
#define FL_ANSI		0x08	/* Type of term emulation */
#define FL_TAG		0x80	/* This entry is tagged. */
#define FL_SAVE		0x0f	/* Which portions of flags to save. */

#define FILE_MWTR 1	/* main window top row */
#define SUBM_OKAY 5	/* last entry in sub-menu */

static int nrents = 1;

_PROTO(static void file_tell, (char *s));
/*
 * replaced by macro (on test basis)
_PROTO(static GETSDIR_ENTRY *getno, (int no, GETSDIR_ENTRY *dirdat));
 */
_PROTO(static void dhili, (int k));
_PROTO(static void prdir, (WIN *dirw, int top, int cur, \
    GETSDIR_ENTRY *dirdat, int longest));
_PROTO(static void prone, (WIN *dirw, int top, int cur, \
    GETSDIR_ENTRY *dirdat, int longest, int inverse));
_PROTO(static void *set_work_dir, (void *existing, size_t min_len));
_PROTO(static int new_filedir, (GETSDIR_ENTRY *o_dirdat, int flushit));
_PROTO(static void goto_filedir, (char *new_dir, int absolut));
_PROTO(static int tag_untag, (char *pat, int tag));
_PROTO(static char *concat_list, (GETSDIR_ENTRY *d));
_PROTO(static int init_filedir, (void));

static WIN *dsub;
#define WHAT_NR_OPTIONS 6
static char *what =  N_(" [Goto]  [Prev]  [Show]  [Tag]  [Untag]  [Okay] ");
/*                       123456789012345678901234567890123456789012345678
                                  1         2         3         4        */
static int dprev;

/* Little menu. */
static char *d_yesno[] = { N_("   Yes  "), N_("   No   "), CNULL };


/*
 * Tell a little message
 */
static void
file_tell(s)
char *s;
{
  WIN *w;

  w = mc_tell("%s", s);
  sleep(1);
  wclose(w, 1);
}

/*
 * Highlight a choice in the horizontal menu.
 */
static void
dhili(k)
int k;
{
    int what_len = strlen (_(what)),
        what_option_len = what_len / WHAT_NR_OPTIONS,
        initial_y = (76 - (what_len >= 76 ? 74 : what_len)) / 2;
       
    if(k == dprev)
	return;

    if(dprev >= 0) {
	wlocate(dsub, initial_y + what_option_len*dprev, 0);
	if(!useattr) {
	    wputs(dsub, " ");
	} else {
	    wsetattr(dsub, stdattr);
	    wprintf(dsub, "%*.*s", what_option_len - 1, what_option_len - 1,
                          _(what) + what_option_len*dprev);
	}
    }
    dprev = k;
    wlocate(dsub, initial_y + what_option_len*k, 0);
    if(!useattr) {
	wputs(dsub, ">");
    } else {
	wsetattr(dsub, XA_REVERSE | stdattr);
	wprintf(dsub, "%*.*s", what_option_len - 1, what_option_len - 1,
                              _(what) + what_option_len*k);
	wsetattr(dsub, stdattr);
    }
}

/*
 */
#define getno(n,d) ((n) >= nrents? (GETSDIR_ENTRY *) NULL : &((d)[n]))

/*
 * Get entry "no" in list.
static GETSDIR_ENTRY *
getno(no, dirdat)
int no;
GETSDIR_ENTRY *dirdat;
{
    if(no >= nrents)
	return((GETSDIR_ENTRY *) NULL);

    return(&dirdat[no]);
}
 */

/*
 * Print the directory. Only draw from "cur" to bottom.
 */
static void
prdir(dirw, top, cur, dirdat, longest)
WIN *dirw;
int top, cur;
GETSDIR_ENTRY *dirdat;
int longest;
{
    int f, start;
    GETSDIR_ENTRY *d;
    char f_str[BUFSIZ];
    char t_str[BUFSIZ];

    start = cur - top;
    dirflush = 0;
    sprintf(f_str, " %%-%ds", longest + 2);
    wlocate(dirw, 0, start + FILE_MWTR);
    for(f = start; f < dirw->ys - (1 + FILE_MWTR); f++) {
	if((d = getno(f + top, dirdat)) == (GETSDIR_ENTRY *) NULL)
	    break;
	if(d->cflags & FL_TAG)
	    wsetattr(dirw, XA_REVERSE | stdattr);
	if(S_ISDIR(d->mode)) {
	    snprintf(t_str, sizeof(t_str), "[%s]", d->fname);
	    wprintf(dirw, f_str, t_str);
	}
	else
	    wprintf(dirw, f_str, d->fname);
	wsetattr(dirw, XA_NORMAL | stdattr);
	wputc(dirw, '\n');
    }
    dirflush = 1;
    wflush();
}

/*
 * Print one directory entry.
 */
static void
prone(dirw, top, cur, dirdat, longest, inverse)
WIN *dirw;
int top, cur;
GETSDIR_ENTRY *dirdat;
int longest;
int inverse;
{
    char f_str[BUFSIZ];
    char t_str[BUFSIZ];

    dirflush = 0;
    sprintf(f_str, " %%-%ds", longest + 2);
    /*
    if(dirdat->cflags & FL_TAG)
	wsetattr(dirw, XA_REVERSE | stdattr);
     */
    if(inverse)
	wsetattr(dirw, XA_REVERSE | stdattr);
    if(S_ISDIR(dirdat->mode)) {
	snprintf(t_str, sizeof(t_str),  "[%s]", dirdat->fname);
	wprintf(dirw, f_str, t_str);
    }
    else
	wprintf(dirw, f_str, dirdat->fname);
    wsetattr(dirw, XA_NORMAL | stdattr);
    dirflush = 1;
    wflush();
}

static WIN *main_w;
static GETSDIR_ENTRY *dirdat, *d;
static int cur = 0;
static int ocur = 0;
static int subm = SUBM_OKAY;
static int quit = 0;
static int top = 0;
static int c = 0;
static int pgud = 0;
static int first = 1;
static char *s;
static int longest;
static char file_title[BUFSIZ];
static char cwd_str[BUFSIZ];
static char *prev_dir = NULL;
static char *work_dir = NULL;
static char *d_work_dir = NULL;
static char *u_work_dir = NULL;
static int min_len = 1;
static char wc_str[128] = "";
static char wc_mem[128] = "";
static int tag_cnt = 0;
static int how_many = 0;
static int down_loading = 0;
static char *ret_buf = NULL;


/* Init up/down work directories to make sure we start from
 * the configuration defaults on the next up/download. jl 6/2000
 */
void init_dir(char dir)
{
  char *p;
  
  switch (dir) {
  case 'u':
    p=u_work_dir;
    u_work_dir = (char *) NULL;
    break;
  case 'd':
    p=d_work_dir;
    d_work_dir = (char *) NULL;
    break;
  }
  free ((void *) p);
  return;
}

static void *
set_work_dir(existing, min_len)
void *existing;
size_t min_len;
{
    void *vp;

    vp = existing == NULL ? (void *)malloc(min_len) : (void *)realloc(existing, min_len);

    if(down_loading)
	d_work_dir = vp;
    else
	u_work_dir = vp;

    return(vp);
}


/*
 * Initialize new file directory.
 *
 * Sets the current working directory.  Non-0 return = no change.
 */
static int
new_filedir(o_dirdat, flushit)
GETSDIR_ENTRY *o_dirdat;
int flushit;
{
    static int dp_len = 0;
    static char cwd_str_fmt[BUFSIZ] = "";
    int new_dp_len,
        fmt_len;
    char disp_dir[80];
    int what_len = strlen (_(what)),
        initial_y = (76 - (what_len >= 76 ? 74 : what_len)) / 2;

    cur      =  0;
    ocur     =  0;
    subm     =  SUBM_OKAY;
    quit     =  0;
    top      =  0;
    c        =  0;
    pgud     =  0;
    first    =  1;
    min_len  =  1;
    dprev    = -1;
    tag_cnt  =  0;

    /* got to do some error-checking here!!!  Maybe use mcd(), too! */
    if(prev_dir != NULL)
	free(prev_dir);
    prev_dir = getcwd(NULL, BUFSIZ);

    /*
     * get last directory
     */
    work_dir = down_loading? d_work_dir : u_work_dir;

    /*
     * init working directory to default?
     */
    if(work_dir == NULL) {
	char *s = down_loading? (P_DOWNDIR) : (P_UPDIR);
	min_len = 1;

	if(*s != '/')
	    min_len += strlen(homedir) + 1;
	min_len += strlen(s);
	if(min_len < BUFSIZ)
	    min_len = BUFSIZ;

	work_dir = set_work_dir(NULL, min_len);

	if(*s == '/')
	  strncpy(work_dir, s, min_len);
	else
	  snprintf(work_dir, min_len, "%s/%s", homedir, s);
    }
    /* lop-off trailing "/" for consistency */
    if(strlen(work_dir) > 1 && work_dir[strlen(work_dir) - 1] == '/')
	work_dir[strlen(work_dir) - 1] = (char) 0;

    chdir(work_dir);

    /* All right, draw the file directory! */

    if(flushit) {
	dirflush = 0;
	winclr(main_w);
	wredraw(main_w, 1);
    }

    wcursor(main_w, CNORMAL);

    /* select <a file|one or more files|a directory> for <upload|download> */
    snprintf(file_title, sizeof(file_title), _("Select %s for %s"),
      how_many? (how_many < 0? _("one or more files") : _("a file"))
	    : _("a directory"),
      down_loading? _("download") : _("upload"));
    wtitle(main_w, TMID, file_title);
    if((new_dp_len = strlen(work_dir)) > dp_len) {
      dp_len = new_dp_len;
      snprintf(cwd_str_fmt, sizeof(cwd_str_fmt),
	       _("Directory: %%-%ds"), dp_len);
    }
    if (new_dp_len + (fmt_len = strlen(cwd_str_fmt)) > 75) {
      snprintf(disp_dir, sizeof(disp_dir),
	       "...%s", work_dir + new_dp_len - 73 + fmt_len);
      snprintf(cwd_str, sizeof(cwd_str), cwd_str_fmt, disp_dir);
    }
    else
      snprintf(cwd_str, sizeof(cwd_str), cwd_str_fmt, work_dir);
    wlocate(main_w, 0, 0);
    wputs(main_w, cwd_str);

    wlocate(dsub, initial_y, 0);
    wputs(dsub, _(what));

    wsetregion(main_w, 1, main_w->ys - FILE_MWTR);
    main_w->doscroll = 0;

    /* old dir to discard? */
    if(o_dirdat != (GETSDIR_ENTRY *) NULL)
	free(o_dirdat);

    /* get sorted directory */
    if((nrents = getsdir(".", wc_str,
      GETSDIR_PARNT|GETSDIR_NSORT|GETSDIR_DIRSF, 0, &dirdat, &longest)) < 0) {
	/* we really want to announce the error here!!! */
	wclose(main_w, 1);
	wclose(dsub, 1);
	free(dirdat);
	return(-1);
    }

    prdir(main_w, top, top, dirdat, longest);  
    wlocate(main_w, initial_y, main_w->ys - FILE_MWTR);
    wputs(main_w, _("( Escape to exit, Space to tag )"));
    dhili(subm);
    /* this really needs to go in dhili !!!*/
    wlocate(main_w, 0, cur + FILE_MWTR - top);
    if(flushit) {
	dirflush = 1;
	wredraw(dsub, 1);
    }

    return(0);
}


/*
 * Goto a new directory
 */
static void
goto_filedir(new_dir, absolut)
char *new_dir;
int absolut;
{
    if(strcmp(new_dir, "..") == 0) {
	if(strcmp(work_dir, "/")) {
	    char *sp = strrchr(work_dir, '/');
	    *sp = (char) 0;
	    if(strlen(work_dir) == 0)
		strcpy(work_dir, "/");
	}
	else {
	    file_tell(_("Can't back up!"));
	    return;
	}
    }
    else if(!absolut) {
	int new_len = strlen(work_dir) + 1;	/* + '/' */
	if((new_len += strlen(new_dir) + 1) > min_len) {
	    min_len = new_len;
	    work_dir = set_work_dir(work_dir, min_len);
	}
	if(strcmp(work_dir, "/") != 0)
	    strcat(work_dir, "/");
	strcat(work_dir, new_dir);
    }
    else {
	int new_len = 1;
	if(*new_dir != '/')
	    new_len += strlen(homedir) + 1;
	new_len += strlen(new_dir);
	if(min_len < new_len)
	    min_len = new_len;

	work_dir = set_work_dir(work_dir, min_len);

	if(*new_dir == '/')
	    strncpy(work_dir, new_dir, min_len);
	else
	    snprintf(work_dir, min_len, "%s/%s", homedir, new_dir);
    }
    new_filedir(dirdat, 1);
}


/*
 * Initialize the file directory.
 */
static int
init_filedir()
{
    int x1, x2;
    int retstat = -1;

    dirflush = 0;
    x1 = (COLS / 2) - 37;
    x2 = (COLS / 2) + 37;
    dsub = wopen(x1 - 1, LINES - 3, x2 + 1, LINES - 3, BNONE, 
	stdattr, mfcolor, mbcolor, 0, 0, 1);
    main_w = wopen(x1, 2, x2, LINES - 6, BSINGLE, stdattr, mfcolor,
	mbcolor, 0, 0, 1);

    if(ret_buf != NULL ||
      (retstat = ((ret_buf = (char *)malloc(BUFSIZ)) == NULL)? -1 : 0) == 0) {
	retstat = new_filedir((GETSDIR_ENTRY *) NULL, 0);
	dirflush = 1;
	wredraw(dsub, 1);
    }
    return(retstat);
}


static int
tag_untag(pat, tag)
char *pat;
int tag;
{
    GETSDIR_ENTRY *my_d = dirdat;
    int indxr, cntr;

    for(indxr = nrents, cntr = 0; indxr; --indxr, ++my_d)
	if(S_ISREG(my_d->mode) && wildmat(my_d->fname, pat)) {
	    if(tag) {
		my_d->cflags |= FL_TAG;
		++cntr;
	    }
	    else if(my_d->cflags & FL_TAG) {
		my_d->cflags &= ~FL_TAG;
		++cntr;
	    }
	}

    return(cntr);
}


/*
 * concatenate tagged files into a buffer
 */
static char *
concat_list(d)
GETSDIR_ENTRY *d;
{
    GETSDIR_ENTRY *my_d;
    int indxr, len;

    my_d = d;
    for(indxr = nrents, len = 0; indxr; --indxr, ++my_d)
	if(my_d->cflags & FL_TAG)
	    len += strlen(my_d->fname) + 1;

    if(len) {
	if(len > BUFSIZ) {
	  if((ret_buf = (char *)realloc(ret_buf, len)) == NULL) {
	      file_tell(_("Too many files tagged - buffer would overflow"));
	      return(NULL);
	  }
	}
	    
	*ret_buf = (char) 0;
	my_d = d;
	for(indxr = nrents; indxr; --indxr, ++my_d)
	    if(my_d->cflags & FL_TAG) {
		/* this could be *much* more efficient */
		strcat(ret_buf, my_d->fname);
		strcat(ret_buf, " ");
	    }

	ret_buf[strlen(ret_buf) - 1] = (char) 0;
	return(ret_buf);
    }

    return(NULL);
}


/*
 * Draw the file directory.
 *
 *      howmany - How many files can be selected
 *		      0 = none (for directory selection only, as in "rz")
 *		      1 = one  (for single-file up-/down-loads, as in "rx")
 *		     -1 = any number (for multiple files, as in "sz")
 *
 *    downloading - Is this for download selection?
 *		      0 = no
 *		      1 = yes - when single file selected, see if it exists
 */
char *
filedir(howmany, downloading)
int howmany;
int downloading;
{
    time_t click_time = (time_t) 0;

    how_many = howmany;
    down_loading = downloading;
    init_filedir();

again:
    wlocate(main_w, 0, cur + FILE_MWTR - top);
    if(first) {
	wredraw(main_w, 1);
	first = 0;
    }
    while(!quit) {
	d = getno(cur, dirdat);
	/*
	if(S_ISDIR(d->mode))
	    prone(main_w, top, top, d, longest, 0);	
	*/
	switch(c = wxgetch()) {
	    case K_UP:
	    case 'k':
		/*
		if(S_ISDIR(d->mode))
		    prone(main_w, top, top, d, longest, 1);	
		*/
		cur -= (cur > 0);
		break;
	    case K_DN:
	    case 'j':
		/*
		if(S_ISDIR(d->mode))
		    prone(main_w, top, top, d, longest, 1);	
		*/
		cur += (cur < nrents - 1);
		break;
	    case K_LT:
	    case 'h':
		subm--;
		if(subm < 0)
		    subm = SUBM_OKAY;
		break;
	    case K_RT:
	    case 'l':
		subm = (subm + 1) % 6;
		break;
	    case K_PGUP:
	    case '\002': /* Control-B */
		pgud = 1;
		quit = 1;
		break;	
	    case K_PGDN:
	    case '\006': /* Control-F */
		pgud = 2;
		quit = 1;
		break;	
	    case 'f':    /* Find. */
		subm = 1;
		quit = 1;
		break;
	    case ' ':    /* Tag if not directory */
		if(S_ISDIR(d->mode)) {
		    time_t this_time = time((time_t *) NULL);
		    if(this_time - click_time < 2) {
			GETSDIR_ENTRY *d2 = getno(cur, dirdat);
			goto_filedir(d2->fname, 0);
			click_time = (time_t) 0;
		    }
		    else
			click_time = this_time;
		}
		else {
		    if(how_many) {
			if((d->cflags ^= FL_TAG) & FL_TAG) {
			    if(tag_cnt && how_many == 1) {
				d->cflags &= ~FL_TAG;
				file_tell(_("Can select only one!"));
				break;
			    }
			    ++tag_cnt;
			}
			else
			    --tag_cnt;
			wlocate(main_w, 0, cur + FILE_MWTR - top);
			prone(main_w, top, top, d, longest, d->cflags & FL_TAG);	
			wputc(main_w, '\n');
			cur += (cur < nrents - 1);
		    }
		}
		break;

	    case 'G':
	    case 'g':
		subm = 0;
		goto real_programmers_use_gotos;
		break;
	    case 'P':
	    case 'p':
		subm = 1;
		goto real_programmers_use_gotos;
		break;
	    case 'S':
	    case 's':
		subm = 2;
		goto real_programmers_use_gotos;
		break;
	    case 'T':
	    case 't':
		subm = 3;
		goto real_programmers_use_gotos;
		break;
	    case 'U':
	    case 'u':
		subm = 4;
		goto real_programmers_use_gotos;
		break;
	    case 'O':
	    case 'o':
		subm = SUBM_OKAY;
real_programmers_use_gotos:
		c = '\n';
		quit = 1;
		break;

	    case '\033':
	    case '\r':
	    case '\n':
		quit = 1;
		break;

	    default:
		break;
	}

	if(c != ' ')
	    click_time = (time_t) 0;

	if(cur < top) {
	    top--;
	    prdir(main_w, top, top, dirdat, longest);	
	}
	if(cur - top > main_w->ys - (2 + FILE_MWTR)) {
	    top++;
	    prdir(main_w, top, top, dirdat, longest);
	}
	/*
	if(cur != ocur)
	    wlocate(main_w, 0, cur + FILE_MWTR - top);
	 */

	ocur = cur;
	dhili(subm);
	/* this really needs to go in dhili !!!*/
	wlocate(main_w, 0, cur + FILE_MWTR - top);
    }

    quit = 0;
    /* ESC means quit */
    if(c == '\033') {
	wclose(main_w, 1);
	wclose(dsub, 1);
	free(dirdat);
	return(NULL);
    }
    /* Page up or down ? */
    if(pgud == 1) { /* Page up */
	ocur = top;
	top -= main_w->ys - (1 + FILE_MWTR);
	if(top < 0)
	    top = 0;
	cur = top;
	pgud = 0;
	if(ocur != top)
	    prdir(main_w, top, cur, dirdat, longest);
	ocur = cur;
	goto again;
    }
    if(pgud == 2) { /* Page down */
	ocur = top;
	if(top < nrents - main_w->ys + (1 + FILE_MWTR)) {
	    top += main_w->ys - (1 + FILE_MWTR);
	    if(top > nrents - main_w->ys + (1 + FILE_MWTR)) {
		top = nrents - main_w->ys + (1 + FILE_MWTR);
	    }
	    cur = top;
	} else
	    cur = nrents - 1;
	pgud = 0;
	if(ocur != top)
	    prdir(main_w, top, cur, dirdat, longest);
	ocur = cur;
	goto again;
    }

    if(c =='\r' || c == '\n') {
	switch(subm) {
	    case 0:
		/* Goto directory */
		{
		    char buf[128];
		    char *s;
		    strncpy(buf, down_loading? P_DOWNDIR : P_UPDIR,
			sizeof(buf) -1);
		    s = input(_("Goto directory:"), buf);
/*		    if(s == CNULL || *s == (char) 0) */
		    if(s == CNULL)
			break;
		    goto_filedir(buf, 1);
		}
		break;
	    case 1:
		/* Previous directory */
		goto_filedir(prev_dir, 1);
		break;
	    case 2:
		/* File (wildcard) spec */
		{
		    char *s = input(_("Filename pattern:"), wc_mem);
		    if(s == CNULL || *s == (char) 0)
			break;
		    strcpy(wc_str, wc_mem);
		    new_filedir(dirdat, 1);
		    wc_str[0] = (char) 0;
		}
		break;
	    case 3:
		/* Tag */
		if(how_many == 1)
		    file_tell(_("Can select only one!"));
		else if(how_many == -1) {
		    char tag_buf[128];
		    char *s;
		    strncpy(tag_buf, wc_mem, 128);
		    
		    s = input(_("Tag pattern:"), tag_buf);
		    if(s != CNULL && *s != (char) 0) {
			int newly_tagged;
			if((newly_tagged = tag_untag(tag_buf, 1)) == 0) {
			    file_tell(_("No file(s) tagged"));
			    goto tag_end;
			}
			tag_cnt += newly_tagged;
			prdir(main_w, top, top, dirdat, longest);  
		    }
		}
tag_end:
		break;
	    case 4:
		/* Untag */
		{
		    char tag_buf[128];
		    char *s;
		    int untagged;
		    strncpy(tag_buf, wc_mem, 128);
		    
		    s = input(_("Untag pattern:"), tag_buf);
		    if(s == CNULL || *s == (char) 0)
			goto untag_end;
		    if((untagged = tag_untag(tag_buf, 0)) == 0) {
			file_tell(_("No file(s) untagged"));
			goto untag_end;
		    }
		    tag_cnt -= untagged;
		    prdir(main_w, top, top, dirdat, longest);  
		}
untag_end:
		break;
	    case 5:
		{
		  /* Done */
		  char *ret_ptr = NULL;	/* failsafe: assume failure */

		  if(how_many != 0 && !tag_cnt) {

		    while(1) {
		      s = input(_("No file selected - enter filename:"),
				ret_buf);
		      if(s != CNULL && *s != (char) 0) {
			int f_exist = access(ret_buf, F_OK);
			if(down_loading) {
			  if(f_exist != -1) {
			    /* ask 'em if they're *sure* */
			    char buf[BUFSIZ];

			    snprintf(buf, sizeof(buf), 
				    _("File: \"%s\" exists!. Over-write?"),
				    ret_buf);
			    if(ask(buf, d_yesno) == 0) {
			      ret_ptr = ret_buf;
			      break;
			    }
			  }
			  else {
			    ret_ptr = ret_buf;
			    break;
			  }
			}
			else {
			  if(f_exist == -1)
			    file_tell(_("no such file!"));
			  else {
			    ret_ptr = ret_buf;
			    break;
			  }
			}
		      }
		      else {
				/* maybe want to ask: "abort?", here */
			goto again;
		      }
		    }
		  }
		  else {
		    /* put 'em in a buffer for return */
		    if(how_many == 0) {
		      /* current working directory */
		      ret_ptr = work_dir;
		    }
		    else {
		      ret_ptr = concat_list(dirdat);
		    }
		  }

		  wclose(main_w, 1);
		  wclose(dsub, 1);
		  free(dirdat);
		  return(ret_ptr);
		}
		break;
	    default:
		/* should "beep", I guess (? shouldn't get here) */
		file_tell("BEEP!");
		break;
	} /* switch */
    }

    goto again;
}
