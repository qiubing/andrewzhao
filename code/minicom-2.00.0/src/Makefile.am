
EXTRA_PROGRAMS = minicom.keyserv

bin_PROGRAMS = minicom runscript ascii-xfr @KEYSERV@

bin_SCRIPTS = xminicom

minicom_SOURCES = minicom.c vt100.c config.c help.c updown.c \
	util.c dial.c window.c wkeys.c ipc.c \
	windiv.c sysdep1.c sysdep2.c rwconf.c main.c \
	file.c getsdir.c wildmat.c common.c
	
noinst_HEADERS = charmap.h configsym.h defmap.h \
	getsdir.h intl.h keyboard.h minicom.h \
	port.h vt100.h window.h sysdep.h rcsid.h

runscript_SOURCES = script.c sysdep1.c common.c port.h minicom.h

ascii_xfr_SOURCES = ascii-xfr.c

minicom_keyserv_SOURCES = keyserv.c wkeys.c sysdep2.c

AUTOMAKE_OPTIONS = dist-tarZ no-dependencies 1.0

minicom_LDADD = @INTLLIBS@

runscript_LDADD = @INTLLIBS@

LDADD = $(top_builddir)/lib/libport.a
CPPFLAGS = @CPPFLAGS@  -I$(top_srcdir)/lib

EXTRA_DIST = functions.c xminicom

