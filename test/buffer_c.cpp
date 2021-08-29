#include <msgpack/fbuffer.h>
#include <msgpack/zbuffer.h>
#include <msgpack/sbuffer.h>
#include <msgpack/vrefbuffer.h>

#define BOOST_TEST_MODULE buffer
#include <boost/test/unit_test.hpp>

#include <cstring>

#if defined(unix) || defined(__unix) || defined(__linux__) || defined(__APPLE__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__QNX__) || defined(__QNXTO__) || defined(__HAIKU__)
#define HAVE_SYS_UIO_H 1
#else
#define HAVE_SYS_UIO_H 0
#endif

BOOST_AUTO_TEST_CASE(zbuffer_c)
{
    msgpack_zbuffer zbuf;
    BOOST_CHECK(msgpack_zbuffer_init(&zbuf, 1, MSGPACK_ZBUFFER_INIT_SIZE));
    BOOST_CHECK_EQUAL(0, msgpack_zbuffer_write(&zbuf, "a", 1));
    BOOST_CHECK_EQUAL(0, msgpack_zbuffer_write(&zbuf, "a", 1));
    BOOST_CHECK_EQUAL(0, msgpack_zbuffer_write(&zbuf, "a", 1));
    BOOST_CHECK_EQUAL(0, msgpack_zbuffer_write(&zbuf, "", 0));

    BOOST_CHECK(msgpack_zbuffer_flush(&zbuf) != NULL);

    msgpack_zbuffer_destroy(&zbuf);
}

BOOST_AUTO_TEST_CASE(fbuffer_c)
{
#if defined(_MSC_VER)
    FILE* file;
    tmpfile_s(&file);
#else  // defined(_MSC_VER)
    FILE* file = tmpfile();
#endif // defined(_MSC_VER)

    void* fbuf = (void*)file;

    BOOST_CHECK( file != NULL );
    BOOST_CHECK_EQUAL(0, msgpack_fbuffer_write(fbuf, "a", 1));
    BOOST_CHECK_EQUAL(0, msgpack_fbuffer_write(fbuf, "a", 1));
    BOOST_CHECK_EQUAL(0, msgpack_fbuffer_write(fbuf, "a", 1));

    fflush(file);
    rewind(file);
    for (size_t i=0; i < 3; ++i) {
        int ch = fgetc(file);
        BOOST_CHECK(ch != EOF);
        BOOST_CHECK_EQUAL('a', (char) ch);
    }
    BOOST_CHECK_EQUAL(EOF, fgetc(file));
    fclose(file);
}

BOOST_AUTO_TEST_CASE(sbuffer_c)
{
    msgpack_sbuffer *sbuf;
    char *data;

    sbuf = msgpack_sbuffer_new();
    BOOST_CHECK(sbuf != NULL);
    BOOST_CHECK_EQUAL(0, msgpack_sbuffer_write(sbuf, "a", 1));
    BOOST_CHECK_EQUAL(0, msgpack_sbuffer_write(sbuf, "b", 1));
    BOOST_CHECK_EQUAL(0, msgpack_sbuffer_write(sbuf, "c", 1));
    BOOST_CHECK_EQUAL(0, msgpack_sbuffer_write(sbuf, "", 0));
    BOOST_CHECK_EQUAL(3U, sbuf->size);
    BOOST_CHECK_EQUAL(0, memcmp(sbuf->data, "abc", 3));
    data = msgpack_sbuffer_release(sbuf);
    BOOST_CHECK_EQUAL(0, memcmp(data, "abc", 3));
    BOOST_CHECK_EQUAL(0U, sbuf->size);
    BOOST_CHECK(sbuf->data == NULL);

    free(data);
    msgpack_sbuffer_free(sbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_c)
{
    const char *raw = "I was about to sail away in a junk,"
                      "When suddenly I heard"
                      "The sound of stamping and singing on the bank--"
                      "It was you and your friends come to bid me farewell."
                      "The Peach Flower Lake is a thousand fathoms deep,"
                      "But it cannot compare, O Wang Lun,"
                      "With the depth of your love for me.";
    const size_t rawlen = strlen(raw);
    msgpack_vrefbuffer *vbuf;
    const int ref_size = 24, chunk_size = 128;
    size_t slices[] = {0, 9, 10,
                    MSGPACK_VREFBUFFER_REF_SIZE,
                    MSGPACK_VREFBUFFER_REF_SIZE + 1,
                    ref_size, chunk_size + 1};
    size_t iovcnt;
    const iovec *iov;
    size_t len = 0, i;
    char *buf;

    vbuf = msgpack_vrefbuffer_new(ref_size, 0);
    for (i = 0; i < sizeof(slices) / sizeof(slices[0]); i++) {
        msgpack_vrefbuffer_write(vbuf, raw + len, slices[i]);
        len += slices[i];
    }
    EXPECT_LT(len, rawlen);
    iov = msgpack_vrefbuffer_vec(vbuf);
    iovcnt = msgpack_vrefbuffer_veclen(vbuf);

    buf = (char *)malloc(rawlen);
#if HAVE_SYS_UIO_H
    {
        int fd;
        char filename[] = "/tmp/mp.XXXXXX";

        fd = mkstemp(filename);
        EXPECT_LT(0, fd);
        writev(fd, iov, (int)iovcnt);
        len = (size_t)lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);
        read(fd, buf, len);
        BOOST_CHECK_EQUAL(0, memcmp(buf, raw, len));
        close(fd);
        unlink(filename);
    }
#else
    {
        len = 0;
        for (i = 0; i < iovcnt; i++)
        {
            EXPECT_LT(len, rawlen);
            memcpy(buf + len, iov[i].iov_base, iov[i].iov_len);
            len += iov[i].iov_len;
        }
        BOOST_CHECK_EQUAL(0, memcmp(buf, raw, len));
    }
#endif
    free(buf);
    msgpack_vrefbuffer_free(vbuf);
}
