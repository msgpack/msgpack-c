AM_CPPFLAGS   = -I$(top_srcdir)/include
AM_C_CPPFLAGS = -I$(top_srcdir)/include
AM_LDFLAGS = $(top_builddir)/src/libmsgpack.la -lgtest_main -lgtest -pthread

check_PROGRAMS = \
		zone \
		pack_unpack \
		pack_unpack_c \
		streaming \
		streaming_c \
		object \
		convert \
		buffer \
		cases \
		fixint \
		fixint_c \
		version \
		msgpack_tuple \
		msgpackc_test \
		msgpack_test \
		msgpack_test_cpp11 \
		reference

TESTS = $(check_PROGRAMS)

zone_SOURCES = zone.cpp

pack_unpack_SOURCES = pack_unpack.cpp

pack_unpack_c_SOURCES = pack_unpack_c.cpp

streaming_SOURCES = streaming.cpp

streaming_c_SOURCES = streaming_c.cpp

object_SOURCES = object.cpp

convert_SOURCES = convert.cpp

buffer_SOURCES = buffer.cpp
buffer_LDADD = -lz

cases_SOURCES = cases.cpp

fixint_SOURCES = fixint.cpp

fixint_c_SOURCES = fixint_c.cpp

version_SOURCES = version.cpp

msgpack_tuple_SOURCES = msgpack_tuple.cpp

msgpackc_test_SOURCES = msgpackc_test.cpp

msgpack_test_SOURCES = msgpack_test.cpp

msgpack_test_cpp11_SOURCES = msgpack_test_cpp11.cpp

reference_SOURCES = reference.cpp

EXTRA_DIST = cases.mpac cases_compact.mpac
