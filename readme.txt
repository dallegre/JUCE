To build in linux:

Had to add libgtk-3.0 and libwebkit2gtk-4.0, maybe other packages..

In code blocks you need to add "`pkg-config --libs gtk+-3.0 webkit2gtk-4.0`"
to compiler settings, linker settings, and "`pkg-config --cflags gtk+-3.0 webkit2gtk-4.0`" to compiler settings, compiler settings, other compiler options.

Had to remove the video module.

Have to update module paths.


