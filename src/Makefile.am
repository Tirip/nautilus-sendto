INCLUDES =					\
	-I$(top_srcdir)				\
	-I$(top_builddir)			\
	-DLOCALEDIR="\"$(datadir)/locale\""	\
	$(NAUTILUS_SENDTO_CFLAGS)		\
	$(DISABLE_DEPRECATED)			\
	$(WARN_CFLAGS)

bin_PROGRAMS = nautilus-sendto

nautilus_sendto_SOURCES = nautilus-sendto.c
nautilus_sendto_LDADD = $(NAUTILUS_SENDTO_LIBS)
