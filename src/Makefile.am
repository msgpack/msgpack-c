lib_LTLIBRARIES = libmsgpack.la

AM_CPPFLAGS = -I../include

libmsgpack_la_SOURCES = \
		unpack.c \
		objectc.c \
		version.c \
		vrefbuffer.c \
		zone.c

if ENABLE_GCC_CXX_ATOMIC
	CXXFLAGS="$CXXFLAGS -DENABLE_GCC_CXX_ATOMIC"
endif


# -version-info CURRENT:REVISION:AGE
libmsgpack_la_LDFLAGS = -version-info 4:0:0 -no-undefined


# backward compatibility
lib_LTLIBRARIES += libmsgpackc.la

libmsgpackc_la_SOURCES = \
		unpack.c \
		objectc.c \
		version.c \
		vrefbuffer.c \
		zone.c

libmsgpackc_la_LDFLAGS = -version-info 2:0:0 -no-undefined


nobase_include_HEADERS = \
		../include/msgpack/pack_define.h \
		../include/msgpack/pack_template.h \
		../include/msgpack/unpack_define.h \
		../include/msgpack/unpack_template.h \
		../include/msgpack/util.h \
		../include/msgpack/sysdep.h \
		../include/msgpack/gcc_atomic.h \
		../include/msgpack.h \
		../include/msgpack/sbuffer.h \
		../include/msgpack/version.h \
		../include/msgpack/version_master.h \
		../include/msgpack/vrefbuffer.h \
		../include/msgpack/zbuffer.h \
		../include/msgpack/fbuffer.h \
		../include/msgpack/pack.h \
		../include/msgpack/unpack.h \
		../include/msgpack/object.h \
		../include/msgpack/zone.h

nobase_include_HEADERS += \
		../include/msgpack/predef.h \
		../include/msgpack/predef/make.h \
		../include/msgpack/predef/platform.h \
		../include/msgpack/predef/other.h \
		../include/msgpack/predef/platform/windows_phone.h \
		../include/msgpack/predef/platform/windows_desktop.h \
		../include/msgpack/predef/platform/mingw.h \
		../include/msgpack/predef/platform/windows_store.h \
		../include/msgpack/predef/platform/windows_runtime.h \
		../include/msgpack/predef/architecture/x86.h \
		../include/msgpack/predef/architecture/arm.h \
		../include/msgpack/predef/architecture/convex.h \
		../include/msgpack/predef/architecture/parisc.h \
		../include/msgpack/predef/architecture/sys390.h \
		../include/msgpack/predef/architecture/x86/32.h \
		../include/msgpack/predef/architecture/x86/64.h \
		../include/msgpack/predef/architecture/m68k.h \
		../include/msgpack/predef/architecture/sys370.h \
		../include/msgpack/predef/architecture/rs6k.h \
		../include/msgpack/predef/architecture/mips.h \
		../include/msgpack/predef/architecture/pyramid.h \
		../include/msgpack/predef/architecture/alpha.h \
		../include/msgpack/predef/architecture/superh.h \
		../include/msgpack/predef/architecture/z.h \
		../include/msgpack/predef/architecture/blackfin.h \
		../include/msgpack/predef/architecture/sparc.h \
		../include/msgpack/predef/architecture/ppc.h \
		../include/msgpack/predef/architecture/ia64.h \
		../include/msgpack/predef/other/endian.h \
		../include/msgpack/predef/language/objc.h \
		../include/msgpack/predef/language/stdcpp.h \
		../include/msgpack/predef/language/stdc.h \
		../include/msgpack/predef/detail/endian_compat.h \
		../include/msgpack/predef/detail/comp_detected.h \
		../include/msgpack/predef/detail/platform_detected.h \
		../include/msgpack/predef/detail/os_detected.h \
		../include/msgpack/predef/detail/test.h \
		../include/msgpack/predef/detail/_exception.h \
		../include/msgpack/predef/detail/_cassert.h \
		../include/msgpack/predef/version_number.h \
		../include/msgpack/predef/compiler.h \
		../include/msgpack/predef/compiler/borland.h \
		../include/msgpack/predef/compiler/kai.h \
		../include/msgpack/predef/compiler/hp_acc.h \
		../include/msgpack/predef/compiler/ibm.h \
		../include/msgpack/predef/compiler/sgi_mipspro.h \
		../include/msgpack/predef/compiler/digitalmars.h \
		../include/msgpack/predef/compiler/llvm.h \
		../include/msgpack/predef/compiler/sunpro.h \
		../include/msgpack/predef/compiler/watcom.h \
		../include/msgpack/predef/compiler/comeau.h \
		../include/msgpack/predef/compiler/edg.h \
		../include/msgpack/predef/compiler/pgi.h \
		../include/msgpack/predef/compiler/microtec.h \
		../include/msgpack/predef/compiler/metaware.h \
		../include/msgpack/predef/compiler/palm.h \
		../include/msgpack/predef/compiler/iar.h \
		../include/msgpack/predef/compiler/intel.h \
		../include/msgpack/predef/compiler/clang.h \
		../include/msgpack/predef/compiler/visualc.h \
		../include/msgpack/predef/compiler/ekopath.h \
		../include/msgpack/predef/compiler/gcc_xml.h \
		../include/msgpack/predef/compiler/compaq.h \
		../include/msgpack/predef/compiler/dignus.h \
		../include/msgpack/predef/compiler/tendra.h \
		../include/msgpack/predef/compiler/greenhills.h \
		../include/msgpack/predef/compiler/mpw.h \
		../include/msgpack/predef/compiler/metrowerks.h \
		../include/msgpack/predef/compiler/diab.h \
		../include/msgpack/predef/compiler/gcc.h \
		../include/msgpack/predef/os/unix.h \
		../include/msgpack/predef/os/bsd.h \
		../include/msgpack/predef/os/solaris.h \
		../include/msgpack/predef/os/windows.h \
		../include/msgpack/predef/os/hpux.h \
		../include/msgpack/predef/os/macos.h \
		../include/msgpack/predef/os/os400.h \
		../include/msgpack/predef/os/beos.h \
		../include/msgpack/predef/os/aix.h \
		../include/msgpack/predef/os/bsd/dragonfly.h \
		../include/msgpack/predef/os/bsd/net.h \
		../include/msgpack/predef/os/bsd/free.h \
		../include/msgpack/predef/os/bsd/bsdi.h \
		../include/msgpack/predef/os/bsd/open.h \
		../include/msgpack/predef/os/cygwin.h \
		../include/msgpack/predef/os/irix.h \
		../include/msgpack/predef/os/ios.h \
		../include/msgpack/predef/os/amigaos.h \
		../include/msgpack/predef/os/vms.h \
		../include/msgpack/predef/os/linux.h \
		../include/msgpack/predef/os/qnxnto.h \
		../include/msgpack/predef/os/haiku.h \
		../include/msgpack/predef/os/android.h \
		../include/msgpack/predef/architecture.h \
		../include/msgpack/predef/os.h \
		../include/msgpack/predef/library.h \
		../include/msgpack/predef/version.h \
		../include/msgpack/predef/language.h \
		../include/msgpack/predef/library/c/_prefix.h \
		../include/msgpack/predef/library/c/uc.h \
		../include/msgpack/predef/library/c/gnu.h \
		../include/msgpack/predef/library/c/vms.h \
		../include/msgpack/predef/library/c/zos.h \
		../include/msgpack/predef/library/c.h \
		../include/msgpack/predef/library/std/_prefix.h \
		../include/msgpack/predef/library/std/cxx.h \
		../include/msgpack/predef/library/std/sgi.h \
		../include/msgpack/predef/library/std/libcomo.h \
		../include/msgpack/predef/library/std/stdcpp3.h \
		../include/msgpack/predef/library/std/vacpp.h \
		../include/msgpack/predef/library/std/stlport.h \
		../include/msgpack/predef/library/std/dinkumware.h \
		../include/msgpack/predef/library/std/msl.h \
		../include/msgpack/predef/library/std/roguewave.h \
		../include/msgpack/predef/library/std/modena.h \
		../include/msgpack/predef/library/std.h

if ENABLE_CXX
nobase_include_HEADERS += \
		../include/msgpack.hpp \
		../include/msgpack/adaptor/adaptor_base.hpp \
		../include/msgpack/adaptor/bool.hpp \
		../include/msgpack/adaptor/boost/fusion.hpp \
		../include/msgpack/adaptor/boost/optional.hpp \
		../include/msgpack/adaptor/boost/string_ref.hpp \
		../include/msgpack/adaptor/char_ptr.hpp \
		../include/msgpack/adaptor/check_container_size.hpp \
		../include/msgpack/adaptor/cpp11/array.hpp \
		../include/msgpack/adaptor/cpp11/array_char.hpp \
		../include/msgpack/adaptor/cpp11/forward_list.hpp \
		../include/msgpack/adaptor/cpp11/shared_ptr.hpp \
		../include/msgpack/adaptor/cpp11/tuple.hpp \
		../include/msgpack/adaptor/cpp11/unique_ptr.hpp \
		../include/msgpack/adaptor/cpp11/unordered_map.hpp \
		../include/msgpack/adaptor/cpp11/unordered_set.hpp \
		../include/msgpack/adaptor/define.hpp \
		../include/msgpack/adaptor/deque.hpp \
		../include/msgpack/adaptor/detail/cpp03_define_map.hpp \
		../include/msgpack/adaptor/detail/cpp03_define_array.hpp \
		../include/msgpack/adaptor/detail/cpp03_msgpack_tuple.hpp \
		../include/msgpack/adaptor/detail/cpp11_define_array.hpp \
		../include/msgpack/adaptor/detail/cpp11_define_map.hpp \
		../include/msgpack/adaptor/detail/cpp11_msgpack_tuple.hpp \
		../include/msgpack/adaptor/ext.hpp \
		../include/msgpack/adaptor/fixint.hpp \
		../include/msgpack/adaptor/float.hpp \
		../include/msgpack/adaptor/int.hpp \
		../include/msgpack/adaptor/list.hpp \
		../include/msgpack/adaptor/map.hpp \
		../include/msgpack/adaptor/msgpack_tuple.hpp \
		../include/msgpack/adaptor/nil.hpp \
		../include/msgpack/adaptor/pair.hpp \
		../include/msgpack/adaptor/raw.hpp \
		../include/msgpack/adaptor/v4raw.hpp \
		../include/msgpack/adaptor/set.hpp \
		../include/msgpack/adaptor/string.hpp \
		../include/msgpack/adaptor/tr1/unordered_map.hpp \
		../include/msgpack/adaptor/tr1/unordered_set.hpp \
		../include/msgpack/adaptor/vector.hpp \
		../include/msgpack/adaptor/vector_bool.hpp \
		../include/msgpack/adaptor/vector_char.hpp \
		../include/msgpack/cpp_config.hpp \
		../include/msgpack/detail/cpp03_zone.hpp \
		../include/msgpack/detail/cpp11_zone.hpp \
		../include/msgpack/fbuffer.hpp \
		../include/msgpack/iterator.hpp \
		../include/msgpack/meta.hpp \
		../include/msgpack/object.hpp \
		../include/msgpack/object_fwd.hpp \
		../include/msgpack/pack.hpp \
		../include/msgpack/sbuffer.hpp \
		../include/msgpack/type.hpp \
		../include/msgpack/unpack.hpp \
		../include/msgpack/version.hpp \
		../include/msgpack/versioning.hpp \
		../include/msgpack/vrefbuffer.hpp \
		../include/msgpack/zbuffer.hpp \
		../include/msgpack/zone.hpp
endif

EXTRA_DIST = \
		../erb/cpp03_zone.hpp.erb \
		../erb/cpp03_define.hpp.erb \
		../erb/cpp03_msgpack_tuple.hpp.erb


doxygen_c:
	cat ../Doxyfile > Doxyfile_c
	echo "FILE_PATTERNS      = *.h" >> Doxyfile_c
	echo "OUTPUT_DIRECTORY   = doc_c" >> Doxyfile_c
	echo "INPUT              = ../include" >> Doxyfile_c
	echo "EXTRACT_ALL        = YES" >> Doxyfile_c
	echo "PROJECT_NAME       = \"MessagePack for C\"" >> Doxyfile_c
	doxygen Doxyfile_c

doxygen_cpp:
	cat ../Doxyfile > Doxyfile_cpp
	echo "FILE_PATTERNS      = *.hpp" >> Doxyfile_cpp
	echo "OUTPUT_DIRECTORY   = doc_cpp" >> Doxyfile_cpp
	echo "INPUT              = ../include" >> Doxyfile_cpp
	echo "EXTRACT_ALL        = YES" >> Doxyfile_cpp
	echo "PROJECT_NAME       = \"MessagePack for C++\"" >> Doxyfile_cpp
	doxygen Doxyfile_cpp

doxygen: doxygen_c doxygen_cpp
