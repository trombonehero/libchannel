#!/bin/sh

if test -z "${DIR}" ; then DIR=$( make -V .OBJDIR ); fi
if test -z "${DIR}" ; then DIR=$( dirname $0 ); fi

BIN=$DIR/test-libchannel
LD_LIBRARY_PATH=$DIR/../libchannel; export LD_LIBRARY_PATH
BINARY=$BIN; export BINARY

TRACE_FILE=`mktemp -t libchannel.test`
ktrace -di -f $TRACE_FILE $BIN $* || echo "# ktrace in $TRACE_FILE"

