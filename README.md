JIGSAW
======

A jigsaw puzzle creator/solver/game

by:
Ed Salisbury <ed@edsalisbury.net>
Matt Goralczyk <matthew.goralczyk@ucdenver.edu>
James Brayton <james.brayton@gmail.com>

Created: April, 2012

How to compile:
---------------
$ make

This will create three programs:
* make_pieces
* match_pieces
* join_pieces

How to run:
-----------

### Make puzzle:

$ make_pieces <image_file> <number of pieces> <variance>

### This will do the following:
* Load in the image
* Remove any transparency
* Replace black (0,0,0) with (1,1,1)
* Replace blue (0, 0, 255) with (0, 0, 254)
* Draw blue lines for edges, and black lines for sides
* Draw appropriate number of lines using trig functions and variance (the length of the opposite side)
* Places data files into folder <image_name> (without extension)
* Data files for each piece: #.png, #.rot, #.xy, where # is the piece number, .rot is the rotation file, and .xy is the xy coordinates of the sides of the piece

### Match pieces:

$ match_pieces <image_dir> <piece1> <piece2>

### This will do the following:
* Load in the two rotation files and reverse the second one
* Append a terminating character to each string, and then build a generalised suffix tree with the strings
* When the longest substring is found, it will return a percentage matched, the offsets in the rotation files, and the length of the substring

### Join pieces:

$ join_pieces <image_dir> <piece1> <piece2> <offset1> <offset2> <length>

### This will do the following:
* Load in both XY files
* Check to make sure that there is no overlap
* Generate a new image with the appropriate side joined
* Generate new XY and rotation files

More Info:
----------

* The programs were developed and tested on Ubuntu Linux 11.10, using gcc version 4.6.1.
* The programs were tested using the "valgrind" utility, which has confirmed that there are no memory leaks.
