
EXTRA_DIST = ABOUT-NLS AUTHORS COPYING ChangeLog FILE_ID.DIZ \
	     INSTALL NEWS README aux minicom.spec

SUBDIRS = doc extras man intl po lib src

AUTOMAKE_OPTIONS = dist-tarZ no-dependencies 1.0

## make rpms
rpm:	Makefile
	$(MAKE) dist
	rpm -ta --clean $(PACKAGE)-$(VERSION).tar.gz
