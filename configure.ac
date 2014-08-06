AC_INIT([nDPI], [1.4.0])
AC_CONFIG_SRCDIR([src/include/ndpi_protocols.h])
AM_PROG_AR

AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_LIBTOOL

AC_PROG_CC
AC_PROG_INSTALL

LT_INIT

OWNER=tonychung00
SVN_RELEASE=`svn info . | grep "^Revision"|cut -d " " -f 2`
if test -z ${SVN_RELEASE}; then
	GIT_COMMIT=`git rev-parse --short HEAD`
	wget https://github.com/${OWNER}/ndpi/archive/${GIT_COMMIT}/ndpi-${GIT_COMMIT}.tar.gz
	SVN_RELEASE=${GIT_COMMIT:0:7}
fi
AC_DEFINE_UNQUOTED(NDPI_SVN_RELEASE, "r${SVN_RELEASE}", [SVN Release])
SVN_DATE=`svn info . | grep "^Last Changed Date"|cut -d " " -f 4-`
if test -z ${SVN_DATE} ; then
        GIT_COMMIT_DATE=`git show -s --format=%ci`
        SVN_DATE=${GIT_COMMIT_DATE}
fi

AC_DEFINE_UNQUOTED(NDPI_SVN_DATE, "${SVN_DATE}", [Last SVN change])

case "$host:$CC" in
    *darwin* )
        RES=`gcc -Wall -fno-color-diagnostics src/lib/ndpi_main.c 2>&1 | grep "unrecognized command line option "|wc -l`
	if test $RES -eq 0; then
	        PLATFORM_DEPENDENT="-fno-color-diagnostics"
	fi
	;;
esac

AC_CHECK_HEADERS([netinet/in.h stdint.h stdlib.h string.h unistd.h])

AC_CONFIG_FILES([Makefile
		src/lib/Makefile
		src/include/Makefile
		libndpi.pc
		ndpi.spec
		])

AC_CONFIG_HEADERS(config.h)
AC_SUBST(PLATFORM_DEPENDENT)
AC_SUBST(SVN_RELEASE)
AC_SUBST(SVN_DATE)
AC_SUBST(GIT_COMMIT)

AC_OUTPUT
