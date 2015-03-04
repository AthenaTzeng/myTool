Git ToolBox
===========
## This is the README for myTool ##
## v1. add 4 files, 2011.10.17. by athena@TECO ##

This is also a test Repo.


* anaglyph.c	no input
		output an_DSCF3957.bmp
		read ./bL_DSCF3957.bmp & ./bR_DSCF3957.bmp
		output is a R/B 3D bmp file

* bmp2raw.c	input argv[1] as the input bmp file
		no output
		unzip input and show raw data to frambuffer
		Note that the picture is upside down

* hello.sh	no input, no output
		a shell script that teach me the grammar of shell script

* mpo2jpg.c	input argv[1] as a MPO file
		output ./outFile_0.jpg & ./outFile_1.jpg, 2 JPEG files
		read the header of MPO input, and cut out the first 2 jpgs


