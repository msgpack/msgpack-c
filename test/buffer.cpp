#include <msgpack.hpp>
#include <msgpack/fbuffer.hpp>
#include <msgpack/zbuffer.hpp>

#define BOOST_TEST_MODULE buffer
#include <boost/test/unit_test.hpp>

#include <sstream>
#include <string>
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


BOOST_AUTO_TEST_CASE(sbuffer_read_from)
{
    const std::string src = "abcdefghij";

    std::istringstream is(src);
    msgpack::sbuffer sbuf;

    // A part of the stream is appended to the buffer.
    BOOST_CHECK_EQUAL(4ul, sbuf.read_from(is, 4));
    BOOST_CHECK_EQUAL(4ul, sbuf.size());
    BOOST_CHECK( memcmp(sbuf.data(), "abcd", 4) == 0 );

    // A request larger than the remaining data reads up to the end of the stream.
    BOOST_CHECK_EQUAL(6ul, sbuf.read_from(is, 100));
    BOOST_CHECK_EQUAL(10ul, sbuf.size());
    BOOST_CHECK( memcmp(sbuf.data(), "abcdefghij", 10) == 0 );

    // len == 0 reads nothing.
    BOOST_CHECK_EQUAL(0ul, sbuf.read_from(is, 0));
    BOOST_CHECK_EQUAL(10ul, sbuf.size());

    // Reading at the end of the stream reads nothing.
    is.clear();
    BOOST_CHECK_EQUAL(0ul, sbuf.read_from(is, 1));
    BOOST_CHECK_EQUAL(10ul, sbuf.size());

    // The buffer grows as needed.
    msgpack::sbuffer small(1);
    std::istringstream is2(src);
    BOOST_CHECK_EQUAL(10ul, small.read_from(is2, 10));
    BOOST_CHECK_EQUAL(10ul, small.size());
    BOOST_CHECK( memcmp(small.data(), "abcdefghij", 10) == 0 );
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
    fbuf.write("", 0);

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
