#
# Copyright (c) 1993-1994 The Regents of the University of California.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. All advertising materials mentioning features or use of this software
#    must display the following acknowledgement:
#	This product includes software developed by the University of
#	California, Berkeley and the Network Research Group at
#	Lawrence Berkeley Laboratory.
# 4. Neither the name of the University nor of the Laboratory may be used
#    to endorse or promote products derived from this software without
#    specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# @(#) $Header: /cvsroot/otcl-tclcl/tclcl/Makefile.in,v 1.50 2007/03/10 23:29:36 tom_henderson Exp $ (LBL)
#

BLANK	= # make a blank space.  DO NOT add anything to this line

# The following will be redefined under Windows (see WIN32 lable below)
CC	= gcc
CPP	= g++
LINK	= $(CPP)
MKDEP	= ./conf/mkdep
TCLSH	= /home/yeaseen/ns-allinone-2.35/bin/tclsh8.5
TCL2C	= ./tcl2c++
AR	= ar rc $(BLANK)

RANLIB	= ranlib
INSTALL	= /usr/bin/install -c
LN	= ln
TEST	= test
RM	= rm -f
PERL	= perl


CCOPT	=  -Wall -Wno-write-strings -Wno-parentheses
STATIC	= 
LDFLAGS	= $(STATIC)
LDOUT	= -o $(BLANK)

DEFINE	= -DNO_TK  -DNDEBUG -DUSE_SHM -DHAVE_UNISTD_H=1 -DHAVE_ZLIB_H -DHAVE_LIBOTCL1_14 -DHAVE_OTCL_H -DHAVE_LIBTK8_5 -DHAVE_TK_H -DHAVE_LIBTCL8_5 -DHAVE_TCLINT_H -DHAVE_TCL_H  -DHAVE_CONFIG_H

INCLUDES = \
	-I. -I/usr/include -I/home/yeaseen/ns-allinone-2.35/otcl-1.14 -I/home/yeaseen/ns-allinone-2.35/include -I/home/yeaseen/ns-allinone-2.35/include -I/home/yeaseen/ns-allinone-2.35/include

LIB	= \
	-L/home/yeaseen/ns-allinone-2.35/otcl-1.14 -lotcl -L/home/yeaseen/ns-allinone-2.35/lib -ltk8.5 -L/home/yeaseen/ns-allinone-2.35/lib -ltcl8.5

CFLAGS	= $(CCOPT) $(DEFINE)

# Explicitly define compilation rules since SunOS 4's make doesn't like gcc.
# Also, gcc does not remove the .o before forking 'as', which can be a
# problem if you don't own the file but can write to the directory.
.SUFFIXES: .cc	# $(.SUFFIXES)

.cc.o:
	@rm -f $@
	$(CPP) -c $(CFLAGS) $(INCLUDES) -o $@ $*.cc

.c.o:
	@rm -f $@
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $*.c


GEN_DIR	= gen/
LIB_TCLCL = libtclcl.a
LIBRARY_TK = ../lib/tk8.5
LIBRARY_TCL = ../lib/tcl8.5


TCL_76_LIBRARY_FILES = \
	$(LIBRARY_TCL)/init.tcl
TCL_BASE_LIBRARY_FILES= \
	$(LIBRARY_TCL)/init.tcl \
	$(LIBRARY_TCL)/history.tcl \
	$(LIBRARY_TCL)/word.tcl
TCL_80_LIBRARY_FILES = \
	$(TCL_BASE_LIBRARY_FILES) \
	$(LIBRARY_TCL)/http2.0/http.tcl
TCL_81_LIBRARY_FILES = $(TCL_80_LIBRARY_FILES)
TCL_82_LIBRARY_FILES = \
	$(TCL_BASE_LIBRARY_FILES) \
	$(LIBRARY_TCL)/http2.1/http.tcl
TCL_83_LIBRARY_FILES = \
	$(TCL_BASE_LIBRARY_FILES) \
	$(LIBRARY_TCL)/http2.[34]/http.tcl
TCL_835_LIBRARY_FILES = \
	$(TCL_BASE_LIBRARY_FILES) \
	$(LIBRARY_TCL)/http2.4/http.tcl
TCL_84_LIBRARY_FILES = \
        $(TCL_BASE_LIBRARY_FILES) \
	$(LIBRARY_TCL)/http2.4/http.tcl

TCL_LIBRARY_FILES = \
	$(TCL_BASE_LIBRARY_FILES) ../lib/tcl8.5/http1.0/http.tcl

#TKDOSNAMES = \
#	$(LIBRARY_TK)/optionMenu.tcl \
#	$(LIBRARY_TK)/scrollbar.tcl
TKDOSNAMES = $(LIBRARY_TK)/optMenu.tcl $(LIBRARY_TK)/scrlbar.tcl

OBJ_COMPAT_C = 

# WIN32: uncomment the following line to be use with vc++ nmake
# !include <conf/makefile.win>

TK_LIBRARY_FILES = \
		$(TCL_LIBRARY_FILES) \
		$(LIBRARY_TK)/tk.tcl \
		$(LIBRARY_TK)/clrpick.tcl \
		$(LIBRARY_TK)/comdlg.tcl \
		$(LIBRARY_TK)/button.tcl \
		$(LIBRARY_TK)/dialog.tcl \
		$(LIBRARY_TK)/entry.tcl \
		$(LIBRARY_TK)/focus.tcl \
		$(LIBRARY_TK)/listbox.tcl \
		$(LIBRARY_TK)/menu.tcl \
		$(LIBRARY_TK)/palette.tcl \
		$(LIBRARY_TK)/scale.tcl \
		$(LIBRARY_TK)/tearoff.tcl \
		$(LIBRARY_TK)/text.tcl \
		$(LIBRARY_TK)/tkfbox.tcl \
		$(LIBRARY_TK)/msgbox.tcl \
		$(TKDOSNAMES)

CONSOLE_FILES = $(LIBRARY_TK)/console.tcl

OBJ_CC = Tcl.o Tcl2.o idlecallback.o iohandler.o timer.o rate-variable.o \
	tracedvar.o \
	embedded-tcl.o embedded-tk.o embedded-tclobj.o $(OBJ_COMPAT_CC)

SRC =	$(OBJ_CC:.o=.cc) $(OBJ_COMPAT_C:.o=.c)

all:	$(TCL2C) $(LIB_TCLCL)

$(TCL2C): tcl2c++.o
	$(RM) $@
	$(LINK) $(STATIC) $(LDFLAGS) $(LDOUT)$@ tcl2c++.o

# We used to link to libTcl.a for backwards compatibilty, but no more.
$(LIB_TCLCL): $(OBJ_CC) $(OBJ_COMPAT_C)
	$(RM) $@
	$(AR)$@ $(OBJ_CC) $(OBJ_COMPAT_C)
	$(RANLIB) $@

#
# pass-1 embedded tcl files
#
embedded-tcl.cc: $(TCL_LIBRARY_FILES) $(TCL2C)
	$(RM) $@
	$(TCL2C) et_tcl $(TCL_LIBRARY_FILES) > $@

embedded-tk.cc: $(TK_LIBRARY_FILES) $(TCL2C)
	$(RM) $@
	$(TCL2C) et_tk $(TK_LIBRARY_FILES) > $@

embedded-tclobj.cc: tcl-object.tcl tcl-import.tcl tcl-http.tcl $(TCL2C)
	$(RM) $@
	$(TCL2C) et_tclobject tcl-object.tcl tcl-import.tcl tcl-http.tcl > $@

#
# pass-1 console tcl files
#
embedded-console.cc: $(CONSOLE_FILES) $(TCL2C)
	$(RM) embedded-console.o
	$(TCL2C) et_console $(CONSOLE_FILES) > embedded-console.cc

install: force
	$(INSTALL) -m 644 *.h /usr/local/include
	$(INSTALL) tcl2c++ /usr/local/bin
	$(INSTALL) $(LIB_TCLCL) /usr/local/lib
	$(RANLIB)  /usr/local/lib/$(LIB_TCLCL)

# libTcl is no longer installed as of 13-Nov-99
#	-test -f /usr/local/lib/libTcl.a && rm -f /usr/local/lib/libTcl.a || true
#	ln -s /usr/local/lib/libtclcl.a /usr/local/lib/libTcl.a

clean:
	$(RM) $(LIB_TCLCL) $(TCL2C) *.o config.log config.cache \
	embedded-tk.cc embedded-tclobj.cc embedded-tcl.cc

realclean:
	$(MAKE) clean
	rm -f Makefile config.log config.cache config.status

distclean: realclean

tags:	force
	ctags -tdwC $(SRC) *.h $(TKSRC)/*.c $(TKSRC)/*.h \
		$(TCLSRC)/*.c $(TCLSRC)/*.h

force:

depend:
	$(MKDEP) $(INCLUDES) $(DEFINE) $(SRC)

tar:	force
	tar=tclcl-`cat VERSION`-linux-gnu.tar.gz ; \
	rm -f $$tar ; \
	tar cfh - README CHANGES.html  $(ALL) | \
		gzip -c > $$tar

srctar: force
	@cwd=`pwd` ; dir=`basename $$cwd` ; \
	    name=tclcl-`cat VERSION | tr A-Z a-z` ; \
	    tar=tclcl-src-`cat VERSION`.tar.gz ; \
	    list="" ; \
	    for i in `cat FILES` ; do list="$$list $$name/$$i" ; done; \
	    echo \
	    "(rm -f $$tar; cd .. ; ln -s $$dir $$name)" ; \
	     (rm -f $$tar; cd .. ; ln -s $$dir $$name) ; \
	    echo \
	    "(cd .. ; tar cfhz $$tar [lots of files])" ; \
	     (cd .. ; tar cfhz - $$list) > $$tar ; \
	    echo \
	    "rm ../$$name; chmod 444 $$tar" ;  \
	     rm ../$$name; chmod 444 $$tar

# Create makefile.vc for Win32 development by replacing:
# "# !include ..." 	-> 	"!include ..."
makefile.vc:	Makefile.in
	$(PERL) -pe 's/^# (\!include)/\!include/o' < Makefile.in > makefile.vc
