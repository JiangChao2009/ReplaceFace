This isn't especially practical, but it was a decent exercise in OpenCV and
cURL with the Instagram API in C/C++. (It was a project for Drexel's CS 283.)

See the demonstration at

http://youtu.be/bBZfifQDS9M


BUILD

Makefile targets

 * replace_face: (the default) compile
 * clean: delete some files


USAGE

After compiling, run replace_face with no arguments for the syntax.

Choose a source and destination picture in one of 3 ways:

 * choose an existing file,
 * take a picture if possible, or
 * use pictures from Instagram account, specified by username.

Faces are detected in pictures with OpenCV, using
"cascade/haarcascade_frontalface_alt.xml", which was copied from the
"data/haarcascades" subdirectory of the OpenCV directory. Click on a face oval
to adjust its dimensions. If a source image face is selected, clicking on a
face in the destination window will paste the source face on it.

Note: To use Instagram, you will need to obtain a client id and secret from
Instagram, and fill in those values near the top of src/main.cpp.
