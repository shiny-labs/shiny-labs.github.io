#!/bin/sh
PROGRAMS="easy serious nothere"
for PROGRAM in $PROGRAMS; do
case "$@" in
$PROGRAM)
	if [ -f $@ ]; then
		rm $@
	fi
	make $@
	if [ ! -f ./troll ]; then
		make troll
	fi
	if [ ! -f ./frob ]; then
		make frob
	fi
	START=$(objdump --file-offsets -d $@ | grep '<dummy>' | cut -d':' -f2 | sed 's/)//;s/ //')
	END=$(objdump --file-offsets -d $@ | grep '<__libc_csu_init>' | cut -d':' -f2 | sed 's/)//;s/ //')
	if [ "x$START" != "x" ] ; then
		./frob $@ $START $END
	fi
	./troll $@
	exit
esac
done
echo "Choose one of the binaries to play with: easy, serious or nothere"
