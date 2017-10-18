AC_INIT(BWM801,0.1.0,astrobit2@yahoo.com,bwm801,https://github.com/astronaos/BWM801)

AM_INIT_AUTOMAKE

AC_PROG_RANLIB

AC_LANG(C++)

AC_PROG_CXX

AX_CXX_COMPILE_STDCXX(11,noext,mandatory) #use c++11 standard, required in a variety of places

AX_PTHREAD #needed for anything using EngineMk1

m4_ifdef([AM_PROG_AR], [AM_PROG_AR]) #not sure why the ifdef is required; this is supposed to be a standard command in automake, but my implementation (v1.15) seems to not find it.

AC_CONFIG_FILES([Makefile])

# if the user specified an installation path other that /usr/local
AM_CONDITIONAL([NON_DEFAULT_PREFIX],[test "${prefix}" != "NONE"])
#if there is a privatemodules folder in the user's home directory
AC_CHECK_FILE([${HOME}/privatemodules],[AM_CONDITIONAL([PRIVMODINST],[test "1" == "1"])],[AM_CONDITIONAL([PRIVMODINST],[test "1" == "2"])])
#if there is a modulefiles folder in share - good indicator that environment modules are installed
AC_CHECK_FILE([/usr/share/Modules/modulefiles],[AM_CONDITIONAL([ENVMODINST],[test "1" == "1"])],[AM_CONDITIONAL([ENVMODINST],[test "1" == "2"])])
#if LMOD_CMD is defined  - good indicator that lmod (https://www.tacc.utexas.edu/research-development/tacc-projects/lmod) is installed
AM_CONDITIONAL([LMODINST],[test "${LMOD_CMD}" != ""])
#if modulecmd exists - indicator that environment modules are installed
AC_CHECK_PROG([ENVMOD],modulecmd,[1],[2])
# create a conditional for environment modules
AM_CONDITIONAL([ENVMODPINST],[test "${ENVMOD}" != "2"])

AC_PROG_MKDIR_P

AC_OUTPUT


