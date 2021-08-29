#include <msgpack.hpp>

#define BOOST_TEST_MODULE zone
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(allocate_align)
{
    msgpack::zone z;
    char* start = (char*)z.allocate_align(1);
    BOOST_CHECK_EQUAL(0ul, reinterpret_cast<std::size_t>(start) % sizeof(int));
    for (std::size_t s = 1; s < sizeof(int); s <<= 1) {
        z.allocate_no_align(s);
        char* buf_a = (char*)z.allocate_align(1);
        BOOST_CHECK_EQUAL(0ul, reinterpret_cast<std::size_t>(buf_a) % sizeof(int));
    }
}

BOOST_AUTO_TEST_CASE(allocate_align_custom)
{
    msgpack::zone z;
    for (std::size_t align = 1; align < 64; align <<= 1) {
        char* start = (char*)z.allocate_align(1, align);
        BOOST_CHECK_EQUAL(0ul, reinterpret_cast<std::size_t>(start) % align);
        for (std::size_t s = 1; s < align; s <<= 1) {
            z.allocate_no_align(s);
            char* buf_a = (char*)z.allocate_align(1, align);
            BOOST_CHECK_EQUAL(0ul, reinterpret_cast<std::size_t>(buf_a) % align);
        }
    }
}

class myclass {
public:
    myclass() : num(0), str("default") { }

    myclass(int num, const std::string& str) :
        num(num), str(str) { }

    ~myclass() { }

    int num;
    std::string str;

private:
    myclass(const myclass&);
};


BOOST_AUTO_TEST_CASE(allocate)
{
    msgpack::zone z;
    myclass* m = z.allocate<myclass>();
    BOOST_CHECK_EQUAL(m->num, 0);
    BOOST_CHECK_EQUAL(m->str, "default");
}


BOOST_AUTO_TEST_CASE(allocate_constructor)
{
    msgpack::zone z;
    myclass* m = z.allocate<myclass>(7, "msgpack");
    BOOST_CHECK_EQUAL(m->num, 7);
    BOOST_CHECK_EQUAL(m->str, "msgpack");
}


static void custom_finalizer_func(void* user)
{
    myclass* m = (myclass*)user;
    delete m;
}

BOOST_AUTO_TEST_CASE(push_finalizer)
{
    msgpack::zone z;
    myclass* m = new myclass();
    z.push_finalizer(custom_finalizer_func, (void*)m);
}


BOOST_AUTO_TEST_CASE(push_finalizer_unique_ptr)
{
    msgpack::zone z;
    msgpack::unique_ptr<myclass> am(new myclass());
    z.push_finalizer(msgpack::move(am));
}


BOOST_AUTO_TEST_CASE(allocate_no_align)
{
    msgpack::zone z;
    char* buf1 = reinterpret_cast<char*>(z.allocate_no_align(4));
    char* buf2 = reinterpret_cast<char*>(z.allocate_no_align(4));
    BOOST_CHECK_EQUAL(reinterpret_cast<void*>(buf1+4), reinterpret_cast<void*>(buf2));
}
