README for the QT based MDIS wizard
------------------------------------

The MDIS wizard contains all CPUs and carrier boards for configuring a system.



Building MDIS Wizard:
---------------------

The MDIS Wizzard`s code is based on fairly old QT4 code with QT3 support in it. To build it a compatible
compilation sysroot is necessary. This sysroot comes from an old software server at MeN (used to
be at 192.1.1.22) and it contains a Qt 4.6.0 compilation in it and all the static libraries that are
needed to create the binary (X11, fontconfig, etc.).

The location of these sysroot needs to be defined in the `builddist.env` file. The variable `QTSYSROOT`
tells the Makefile where to find all the necessary dependencies and tools.

build steps:

<modify .cpp or .h files as needed>

cd NATIVE/DIST_GCC3/
source builddist.env
make

(the warnings at the end can be ignored, they result from libraries mismatching on the swserver)

These warnings at the end can be ignored:

/usr/local/Trolltech/Qt-4.6.0/lib/libQtCore.a(qlibrary_unix.o): In function `QLibraryPrivate::load_sys()':
qlibrary_unix.cpp:(.text+0x412): warning: Using 'dlopen' in statically linked applications requires at runtime the shared libraries from the glibc version used for linking
/usr/local/Trolltech/Qt-4.6.0/lib/libQtCore.a(qfsfileengine_unix.o): In function `QFSFileEngine::owner(QAbstractFileEngine::FileOwner) const':
qfsfileengine_unix.cpp:(.text+0x878): warning: Using 'getgrgid_r' in statically linked applications requires at runtime the shared libraries from the glibc version used for linking
/usr/local/Trolltech/Qt-4.6.0/lib/libQt3Support.a(q3filedialog.o): In function `Q3FileDialog::setDir(QString const&)':
q3filedialog.cpp:(.text+0x1363a): warning: Using 'getpwnam_r' in statically linked applications requires at runtime the shared libraries from the glibc version used for linking
/usr/local/Trolltech/Qt-4.6.0/lib/libQtCore.a(qfsfileengine_unix.o): In function `QFSFileEngine::owner(QAbstractFileEngine::FileOwner) const':
qfsfileengine_unix.cpp:(.text+0x708): warning: Using 'getpwuid_r' in statically linked applications requires at runtime the shared libraries from the glibc version used for linking
/usr/X11/lib/libX11.a(x11trans.o): In function `_X11TransSocketOpen':
(.text+0x50c): warning: Using 'getaddrinfo' in statically linked applications requires at runtime the shared libraries from the glibc version used for linking
/usr/X11/lib/libX11.a(x11trans.o): In function `_X11TransSocketUNIXConnect':
(.text+0xf1c): warning: Using 'gethostbyname' in statically linked applications requires at runtime the shared libraries from the glibc version used for linking
/usr/X11/lib/libX11.a(x11trans.o): In function `_X11TransSocketINETConnect':
(.text+0x1e1b): warning: Using 'getservbyname' in statically linked applications requires at runtime the shared libraries from the glibc version used for linking
ts@swserver:~/work/LINUX/TOOLS/MDISWIZ/NATIVE/DIST_GCC3>

Notice
----------------------
Before releasing new release please update common version string (*mdiswizVersion*) and OS version string (e.g. for Linux: *mdiswizLinuxVersion*).
