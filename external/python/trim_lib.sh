echo "-----------------------------trim python2.7 dir -------------------"
LIB_PATH=$PWD/build_mips
rm -rf $LIB_PATH/usr/include $LIB_PATH/usr/share/aclocal \
        $LIB_PATH/usr/lib/pkgconfig $LIB_PATH/usr/share/pkgconfig \
         $LIB_PATH/usr/lib/cmake $LIB_PATH/usr/share/cmake
rm -rf $LIB_PATH/usr/share/gdb
rm -rf $LIB_PATH/usr/man $LIB_PATH/usr/share/man
rm -rf $LIB_PATH/usr/info $LIB_PATH/usr/share/info
rm -rf $LIB_PATH/usr/doc $LIB_PATH/usr/share/doc
rm -rf $LIB_PATH/usr/share/gtk-doc
rm -rf $LIB_PATH/usr/lib/python2.7/bsddb
rm -rf $LIB_PATH/usr/lib/python2.7/config/
rm -rf $LIB_PATH/usr/lib/python2.7/ensurepip
rm -rf $LIB_PATH/usr/lib/python2.7/idle_test
rm -rf $LIB_PATH/usr/lib/python2.7/curses
rm -rf $LIB_PATH/usr/lib/python2.7/lib-tk
rm -rf $LIB_PATH/usr/lib/python2.7/lib2to3
rm -rf $LIB_PATH/usr/lib/python2.7/sqlite3
rm -rf $LIB_PATH/usr/lib/python2.7/xml
rm -rf $LIB_PATH/usr/lib/python2.7/pydoc_data
rm -rf $LIB_PATH/usr/lib/python2.7/idlelib/help.html
rm -rf $LIB_PATH/usr/lib/python2.7/idlelib/Icons
rm -rf $LIB_PATH/usr/lib/python2.7/idlelib/idle_test
rmdir $LIB_PATH/usr/share 2>/dev/null
find $LIB_PATH/usr/{lib,share}/ -name '*.cmake' -print0 | xargs -0 rm -f
find $LIB_PATH/usr/lib \( -name '*.a' -o -name '*.la' \) -print0 | xargs -0 rm -f
find $LIB_PATH/usr/lib/ -name '*.py' -print0 | xargs -0 rm -f
find $LIB_PATH/usr/lib/ -name '*.pyo' -print0 | xargs -0 rm -f
find $LIB_PATH/usr/lib/ -name 'test' -print0 | xargs -0 rm -rf
find $LIB_PATH/usr/lib/ -name 'tests' -print0 | xargs -0 rm -rf
rm -rf $LIB_PATH/usr/lib/luarocks
find $LIB_PATH -type f \( -perm /111 -o -name '*.so*' \) -not \( -name 'libpthread*.so*' -o -name '*.ko' \) -print | xargs mips-linux-gnu-strip --remove-section=.comment --remove-section=.note 2>/dev/null || true
if test -d $LIB_PATH/lib/modules; then find $LIB_PATH/lib/modules -type f -name '*.ko' | xargs -r mips-linux-gnu-strip --remove-section=.comment --remove-section=.note --strip-unneeded; fi
find $LIB_PATH/lib -type f -name 'libpthread*.so*' | xargs -r mips-linux-gnu-strip --remove-section=.comment --remove-section=.note --strip-debug  
