#!/bin/sh
bindir=$(pwd)
cd /home/hlias/documents/uni/7ο Εξάμηνο/Γραφικά και Εικονική Πραγματικότητα/Art_Gallery/art_gallery/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/hlias/documents/uni/7ο\ Εξάμηνο/Γραφικά\ και\ Εικονική\ Πραγματικότητα/Art_Gallery/build/art_gallery 
	else
		"/home/hlias/documents/uni/7ο\ Εξάμηνο/Γραφικά\ και\ Εικονική\ Πραγματικότητα/Art_Gallery/build/art_gallery"  
	fi
else
	"/home/hlias/documents/uni/7ο\ Εξάμηνο/Γραφικά\ και\ Εικονική\ Πραγματικότητα/Art_Gallery/build/art_gallery"  
fi
