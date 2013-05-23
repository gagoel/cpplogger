dnl
dnl Autoconf requirements
dnl AC_INIT(package, version, bug-reoprt-address)
dnl information on the package
dnl checks for programs
dnl checks for libraries
dnl checks for header files
dnl checks for types
dnl checks for structures
dnl checks for compiler characteristics
dnl checks for library functions
dnl checks for system services
dnl AC_CONFIG_FILES([file...])
dnl AC_OUTPUT

dnl
dnl Autoconf configuration for CPP Logger
dnl


dnl Ensure that the CPPLogger version is included
dnlCPPLOGGER_VERSION='1.0'

dnl AC_PREFIX_DEFAULT(/usr/local/cpplogger)

dnlAC_MSG_NOTICE([CPPLOGGER])

dnl AC_ARG_WITH(filehandler, [  --with-filehandler      enable file logger handler.])
dnlif test "$with_filehandler" == "yes"
dnl then
dnl    AC_MSG_NOTICE([Compiling with file logger handler.])
dnl else
dnl    AC_MSG_NOTICE([Compiling without file logger handler.])
dnlfi



AC_PREREQ(2.50)
AC_INIT([CPPLOGGER], [1.0], [gauravgoel9nov@gmail.com])

dnl Substituting the required variables before any checking.
dnl TODO - not working setting builddir, need to check
dnl builddir = $(srcdir)/build
dnl AC_SUBST([builddir])
CXXFLAGS="$CXXFLAGS -O2 -Wall"
AC_SUBST(CXXFLAGS)

dnl Getting configuration input
AC_SUBST([cpplogger_handlers])
AC_SUBST([app_default_config_file])
AC_SUBST([cpplogger_log_file])

AC_ARG_ENABLE([file-handler], [AS_HELP_STRING(
[--enable-file-handler], [enable file handler])], \
[AS_VAR_APPEND([cpplogger_handlers],[ " file " ])], [])

AC_ARG_ENABLE([debug], [AS_HELP_STRING(
[--enable-debug], [add debugging symbols])], \
[AS_VAR_APPEND([CXXFLAGS],[" -g"]);\
AS_VAR_APPEND([CXXFLAGS],[" -DCPPLOGGER_DEBUG"])], [])

AC_ARG_ENABLE([cpplogger-vlogging], [AS_HELP_STRING(
[--enable-cpplogger-vlogging], \
[enable cpplogger verbose logging. logs info messages also.])], \
[AS_VAR_APPEND([CXXFLAGS],[" -DCPPLOGGER_VLOGGING"])], [])

AC_ARG_ENABLE([cpplogger-vvlogging], [AS_HELP_STRING(
[--enable-cpplogger-vvlogging], \
[enable cpplogger very verbose logging. logs info and debug messages also.])],\
[AS_VAR_APPEND([CXXFLAGS],[" -DCPPLOGGER_VVLOGGING"])], [])

AC_ARG_WITH([config-file], [AS_HELP_STRING(
[--with-app-default-config-file], \
[default configuration file for applications])], \
[AS_VAR_SET([app_default_config_file], [$with_config_file])], \
[AS_VAR_SET([app_default_config_file], [no])])

AC_ARG_WITH([log-file], [AS_HELP_STRING(
[--with-log-file], [cpplogger log file])], \
[AS_VAR_SET([cpplogger_log_file], [$with_log_file])], \
[AS_VAR_SET([cpplogger_log_file], [no])])

dnl Checking for required programs.
AC_PROG_RANLIB
AC_PROG_CXX
AC_LANG(C++)

dnl Checking for required libraries.
AC_CHECK_LIB(m, cos)

dnl Checking for required header files.
AC_CHECK_HEADERS(iostream)

dnl Checking for types.

dnl Checking for strucutres.


dnl Setting configuration files.
AC_CONFIG_FILES([
    Makefile
])

AC_OUTPUT
