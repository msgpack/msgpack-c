#include <msgpack.hpp>

#define BOOST_TEST_MODULE streaming
#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_CASE(basic)
{
    msgpack::sbuffer buffer;

    msgpack::packer<msgpack::sbuffer> pk(&buffer);
    pk.pack(1);
    pk.pack(2);
    pk.pack(3);

    const char* input = buffer.data();
    const char* const eof = input + buffer.size();

    msgpack::unpacker pac;
    msgpack::object_handle oh;

    int count = 0;
    while(count < 3) {
        pac.reserve_buffer(32*1024);

        // read buffer into pac.buffer() upto
        // pac.buffer_capacity() bytes.
        size_t len = 1;
        memcpy(pac.buffer(), input, len);
        input += len;

        pac.buffer_consumed(len);

        while(pac.next(oh)) {
            msgpack::object obj = oh.get();
            switch(count++) {
            case 0:
                BOOST_CHECK_EQUAL(1, obj.as<int>());
                break;
            case 1:
                BOOST_CHECK_EQUAL(2, obj.as<int>());
                break;
            case 2:
                BOOST_CHECK_EQUAL(3, obj.as<int>());
                return;
            }
        }

        BOOST_CHECK(input < eof);
    }
}

// obsolete
#if MSGPACK_DEFAULT_API_VERSION == 1

BOOST_AUTO_TEST_CASE(basic_pointer)
{
    msgpack::sbuffer buffer;

    msgpack::packer<msgpack::sbuffer> pk(&buffer);
    pk.pack(1);
    pk.pack(2);
    pk.pack(3);

    const char* input = buffer.data();
    const char* const eof = input + buffer.size();

    msgpack::unpacker pac;
    msgpack::object_handle oh;

    int count = 0;
    while(count < 3) {
        pac.reserve_buffer(32*1024);

        // read buffer into pac.buffer() upto
        // pac.buffer_capacity() bytes.
        size_t len = 1;
        memcpy(pac.buffer(), input, len);
        input += len;

        pac.buffer_consumed(len);

#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
        while(pac.next(&oh)) {
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
            msgpack::object obj = oh.get();
            switch(count++) {
            case 0:
                BOOST_CHECK_EQUAL(1, obj.as<int>());
                break;
            case 1:
                BOOST_CHECK_EQUAL(2, obj.as<int>());
                break;
            case 2:
                BOOST_CHECK_EQUAL(3, obj.as<int>());
                return;
            }
        }

        BOOST_CHECK(input < eof);
    }
}

#endif // MSGPACK_DEFAULT_API_VERSION == 1

#if !defined(MSGPACK_USE_CPP03)

BOOST_AUTO_TEST_CASE(move)
{
    msgpack::sbuffer buffer;

    msgpack::packer<msgpack::sbuffer> pk(&buffer);
    pk.pack(1);
    pk.pack(2);
    pk.pack(3);

    const char* input = buffer.data();
    const char* const eof = input + buffer.size();

    msgpack::unpacker pac;
    msgpack::object_handle oh;

    int count = 0;
    while(count < 3) {
        msgpack::unpacker pac_in(std::move(pac));
        pac_in.reserve_buffer(32*1024);

        // read buffer into pac_in.buffer() upto
        // pac_in.buffer_capac_inity() bytes.
        size_t len = 1;
        memcpy(pac_in.buffer(), input, len);
        input += len;

        pac_in.buffer_consumed(len);

        while(pac_in.next(oh)) {
            msgpack::object obj = oh.get();
            switch(count++) {
            case 0:
                BOOST_CHECK_EQUAL(1, obj.as<int>());
                break;
            case 1:
                BOOST_CHECK_EQUAL(2, obj.as<int>());
                break;
            case 2:
                BOOST_CHECK_EQUAL(3, obj.as<int>());
                return;
            }
        }

        BOOST_CHECK(input < eof);
        pac = std::move(pac_in);
    }
}

#endif // !defined(MSGPACK_USE_CPP03)

class event_handler {
public:
    event_handler(std::istream& input) : input(input) { }
    ~event_handler() { }

    void on_read()
    {
        while(true) {
            pac.reserve_buffer(32*1024);

            size_t len = static_cast<size_t>(input.readsome(pac.buffer(), static_cast<std::streamsize>(pac.buffer_capacity())));

            if(len == 0) {
                return;
            }

            pac.buffer_consumed(len);

            msgpack::object_handle oh;
            while(pac.next(oh)) {
                on_message(oh.get(), msgpack::move(oh.zone()));
            }

            if(pac.message_size() > 10*1024*1024) {
                throw std::runtime_error("message is too large");
            }
        }
    }

    void on_message(msgpack::object obj, msgpack::unique_ptr<msgpack::zone>)
    {
        BOOST_CHECK_EQUAL(expect, obj.as<int>());
    }

    int expect;

private:
    std::istream& input;
    msgpack::unpacker pac;
};

BOOST_AUTO_TEST_CASE(event)
{
    std::stringstream stream;
    msgpack::packer<std::ostream> pk(&stream);

    event_handler handler(stream);

    pk.pack(1);
    handler.expect = 1;
    handler.on_read();

    pk.pack(2);
    handler.expect = 2;
    handler.on_read();

    pk.pack(3);
    handler.expect = 3;
    handler.on_read();
}

// obsolete
#if MSGPACK_DEFAULT_API_VERSION == 1

// backward compatibility
BOOST_AUTO_TEST_CASE(basic_compat)
{
    std::ostringstream stream;
    msgpack::packer<std::ostream> pk(&stream);

    pk.pack(1);
    pk.pack(2);
    pk.pack(3);

    std::istringstream input(stream.str());

    msgpack::unpacker pac;

    int count = 0;
    while(count < 3) {
        pac.reserve_buffer(32*1024);

        size_t len = static_cast<size_t>(input.readsome(pac.buffer(), static_cast<std::streamsize>(pac.buffer_capacity())));
        pac.buffer_consumed(len);

        while(pac.execute()) {
            msgpack::unique_ptr<msgpack::zone> z(pac.release_zone());
            msgpack::object obj = pac.data();
            pac.reset();

            switch(count++) {
            case 0:
                BOOST_CHECK_EQUAL(1, obj.as<int>());
                break;
            case 1:
                BOOST_CHECK_EQUAL(2, obj.as<int>());
                break;
            case 2:
                BOOST_CHECK_EQUAL(3, obj.as<int>());
                return;
            }

        }
    }
}


// backward compatibility
class event_handler_compat {
public:
    event_handler_compat(std::istream& input) : input(input) { }
    ~event_handler_compat() { }

    void on_read()
    {
        while(true) {
            pac.reserve_buffer(32*1024);

            size_t len = static_cast<size_t>(input.readsome(pac.buffer(), static_cast<std::streamsize>(pac.buffer_capacity())));

            if(len == 0) {
                return;
            }

            pac.buffer_consumed(len);

            while(pac.execute()) {
                msgpack::unique_ptr<msgpack::zone> z(pac.release_zone());
                msgpack::object obj = pac.data();
                pac.reset();
                on_message(obj, msgpack::move(z));
            }

            if(pac.message_size() > 10*1024*1024) {
                throw std::runtime_error("message is too large");
            }
        }
    }

    void on_message(msgpack::object obj, msgpack::unique_ptr<msgpack::zone>)
    {
        BOOST_CHECK_EQUAL(expect, obj.as<int>());
    }

    int expect;

private:
    std::istream& input;
    msgpack::unpacker pac;
};

BOOST_AUTO_TEST_CASE(event_compat)
{
    std::stringstream stream;
    msgpack::packer<std::ostream> pk(&stream);

    event_handler_compat handler(stream);

    pk.pack(1);
    handler.expect = 1;
    handler.on_read();

    pk.pack(2);
    handler.expect = 2;
    handler.on_read();

    pk.pack(3);
    handler.expect = 3;
    handler.on_read();
}

#endif // !defined(MSGPACK_USE_CPP03)

// https://github.com/msgpack/msgpack-c/issues/1181
template <typename Unpacker>
void reserve_buffer_overflow_rewound_impl()
{
    Unpacker pac(MSGPACK_NULLPTR, MSGPACK_NULLPTR, 8);

    // off == COUNTER_SIZE path: size + used would wrap
    std::size_t request = std::numeric_limits<std::size_t>::max() - 2;
    BOOST_CHECK_THROW(pac.reserve_buffer(request), std::bad_alloc);

    // a sane request still works
    pac.reserve_buffer(64);
    BOOST_CHECK_GE(pac.buffer_capacity(), static_cast<std::size_t>(64));
}

template <typename Unpacker>
void reserve_buffer_overflow_not_rewound_impl()
{
    Unpacker pac(MSGPACK_NULLPTR, MSGPACK_NULLPTR, 8);

    // consume part of the buffer so off != COUNTER_SIZE
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> pk(&sbuf);
    pk.pack(1);
    pk.pack(2);

    pac.reserve_buffer(sbuf.size());
    std::memcpy(pac.buffer(), sbuf.data(), sbuf.size());
    pac.buffer_consumed(sbuf.size());

    msgpack::object_handle oh;
    BOOST_CHECK(pac.next(oh));
    BOOST_CHECK_EQUAL(oh.get().as<int>(), 1);

    std::size_t request = std::numeric_limits<std::size_t>::max() - 2;
    BOOST_CHECK_THROW(pac.reserve_buffer(request), std::bad_alloc);

    // remaining data must still be parsable
    BOOST_CHECK(pac.next(oh));
    BOOST_CHECK_EQUAL(oh.get().as<int>(), 2);
}

BOOST_AUTO_TEST_CASE(reserve_buffer_overflow_rewound)
{
    reserve_buffer_overflow_rewound_impl<msgpack::unpacker>();
}

BOOST_AUTO_TEST_CASE(reserve_buffer_overflow_rewound_v1)
{
    reserve_buffer_overflow_rewound_impl<msgpack::v1::unpacker>();
}

BOOST_AUTO_TEST_CASE(reserve_buffer_overflow_not_rewound)
{
    reserve_buffer_overflow_not_rewound_impl<msgpack::unpacker>();
}

BOOST_AUTO_TEST_CASE(reserve_buffer_overflow_not_rewound_v1)
{
    reserve_buffer_overflow_not_rewound_impl<msgpack::v1::unpacker>();
}
