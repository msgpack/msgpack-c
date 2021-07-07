#include <msgpack.hpp>
#include <msgpack/fbuffer.hpp>
#include <msgpack/zbuffer.hpp>

#define BOOST_TEST_MODULE buffer
#include <boost/test/unit_test.hpp>

#include <string.h>

BOOST_AUTO_TEST_CASE(sbuffer)
{
    msgpack::sbuffer sbuf;
    sbuf.write("a", 1);
    sbuf.write("a", 1);
    sbuf.write("a", 1);

    BOOST_CHECK_EQUAL(3ul, sbuf.size());
    BOOST_CHECK( memcmp(sbuf.data(), "aaa", 3) == 0 );

    sbuf.clear();
    sbuf.write("a", 1);
    sbuf.write("a", 1);
    sbuf.write("a", 1);

    BOOST_CHECK_EQUAL(3ul, sbuf.size());
    BOOST_CHECK( memcmp(sbuf.data(), "aaa", 3) == 0 );
}


BOOST_AUTO_TEST_CASE(vrefbuffer)
{
    msgpack::vrefbuffer vbuf;
    vbuf.write("a", 1);
    vbuf.write("a", 1);
    vbuf.write("a", 1);

    const msgpack::iovec* vec = vbuf.vector();
    size_t veclen = vbuf.vector_size();

    msgpack::sbuffer sbuf;
    for(size_t i=0; i < veclen; ++i) {
        sbuf.write((const char*)vec[i].iov_base, vec[i].iov_len);
    }

    BOOST_CHECK_EQUAL(3ul, sbuf.size());
    BOOST_CHECK( memcmp(sbuf.data(), "aaa", 3) == 0 );


    vbuf.clear();
    vbuf.write("a", 1);
    vbuf.write("a", 1);
    vbuf.write("a", 1);

    vec = vbuf.vector();
    veclen = vbuf.vector_size();

    sbuf.clear();
    for(size_t i=0; i < veclen; ++i) {
        sbuf.write((const char*)vec[i].iov_base, vec[i].iov_len);
    }

    BOOST_CHECK_EQUAL(3ul, sbuf.size());
    BOOST_CHECK( memcmp(sbuf.data(), "aaa", 3) == 0 );
}

BOOST_AUTO_TEST_CASE(zbuffer)
{
    msgpack::zbuffer zbuf;
    zbuf.write("a", 1);
    zbuf.write("a", 1);
    zbuf.write("a", 1);
    zbuf.write("", 0);

    zbuf.flush();
}

BOOST_AUTO_TEST_CASE(fbuffer)
{
#if defined(_MSC_VER)
    FILE* file;
    tmpfile_s(&file);
#else  // defined(_MSC_VER)
    FILE* file = tmpfile();
#endif // defined(_MSC_VER)
    BOOST_CHECK( file != NULL );

    msgpack::fbuffer fbuf(file);
    BOOST_CHECK_EQUAL(file, fbuf.file());

    fbuf.write("a", 1);
    fbuf.write("a", 1);
    fbuf.write("a", 1);

    fflush(file);
    rewind(file);
    for (size_t i=0; i < 3; ++i) {
        int ch = fgetc(file);
        BOOST_CHECK(ch != EOF);
        BOOST_CHECK_EQUAL('a', static_cast<char>(ch));
    }
    BOOST_CHECK_EQUAL(EOF, fgetc(file));
    fclose(file);
}
