#include <msgpack.hpp>
#include <gtest/gtest.h>

class compatibility {
public:
	compatibility() : str1("default"), str2("default") { }

	std::string str1;
	std::string str2;

	MSGPACK_DEFINE(str1, str2);
};

TEST(convert, compatibility_less)
{
	std::vector<std::string> src(1);
	src[0] = "kumofs";

	msgpack::zone z;
	msgpack::object obj(src, &z);

	compatibility c;
	EXPECT_NO_THROW( obj.convert(&c) );

	EXPECT_EQ("kumofs",  c.str1);
	EXPECT_EQ("default", c.str2);
}

TEST(convert, compatibility_more)
{
	std::vector<std::string> src(3);
	src[0] = "kumofs";
	src[1] = "mpio";
	src[2] = "cloudy";

	msgpack::zone z;
	msgpack::object obj(src, &z);

	compatibility to;
	EXPECT_NO_THROW( obj.convert(&to) );

	EXPECT_EQ("kumofs", to.str1);
	EXPECT_EQ("mpio",   to.str2);
}


class enum_member {
public:
	enum_member() : flag(A) { }

	enum flags_t {
		A = 0,
		B = 1,
	};

	flags_t flag;

	MSGPACK_DEFINE(flag);
};

MSGPACK_ADD_ENUM(enum_member::flags_t);

TEST(convert, enum_member)
{
	enum_member src;
	src.flag = enum_member::B;

	msgpack::zone z;
	msgpack::object obj(src, &z);

	enum_member to;
	EXPECT_NO_THROW( obj.convert(&to) );

	EXPECT_EQ(enum_member::B, to.flag);
}


struct benchmark {
#define field_init(n) f##n(1<<(n))
	benchmark()
		: field_init(0)
		, field_init(1)
		, field_init(2)
		, field_init(3)
		, field_init(4)
		, field_init(5)
		, field_init(6)
		, field_init(7)
		, field_init(8)
		, field_init(9)
		, field_init(10)
		, field_init(11)
		, field_init(12)
		, field_init(13)
		, field_init(14)
		, field_init(15)
		, field_init(16)
		, field_init(17)
		, field_init(18)
		, field_init(19)
		, field_init(20)
		, field_init(21)
		, field_init(22)
		, field_init(23)
		, field_init(24)
		, field_init(25)
		, field_init(26)
		, field_init(27)
		, field_init(28)
		, field_init(29)
	{ }
#undef field_init

	int f0;
	int f1;
	int f2;
	int f3;
	int f4;
	int f5;
	int f6;
	int f7;
	int f8;
	int f9;
	int f10;
	int f11;
	int f12;
	int f13;
	int f14;
	int f15;
	int f16;
	int f17;
	int f18;
	int f19;
	int f20;
	int f21;
	int f22;
	int f23;
	int f24;
	int f25;
	int f26;
	int f27;
	int f28;
	int f29;

	MSGPACK_DEFINE(f0, f1, f2, f3, f4,
				   f5, f6, f7, f8, f9,
				   f10, f11, f12, f13, f14,
				   f15, f16, f17, f18, f19,
				   f20, f21, f22, f23, f24,
				   f25, f26, f27, f28, f29);


	bool operator==(const benchmark& o) const
	{
		return f0 == o.f0
			&& f1 == o.f1
			&& f2 == o.f2
			&& f3 == o.f3
			&& f4 == o.f4
			&& f5 == o.f5
			&& f6 == o.f6
			&& f7 == o.f7
			&& f8 == o.f8
			&& f9 == o.f9
			&& f10 == o.f10
			&& f11 == o.f11
			&& f12 == o.f12
			&& f13 == o.f13
			&& f14 == o.f14
			&& f15 == o.f15
			&& f16 == o.f16
			&& f17 == o.f17
			&& f18 == o.f18
			&& f19 == o.f19
			&& f20 == o.f20
			&& f21 == o.f21
			&& f22 == o.f22
			&& f23 == o.f23
			&& f24 == o.f24
			&& f25 == o.f25
			&& f26 == o.f26
			&& f27 == o.f27
			&& f28 == o.f28
			&& f29 == o.f29
			;
	}
};

std::ostream& operator<<(std::ostream& o, const benchmark& m)
{
	return o << "benchmark("
			 << m.f0 << ','
			 << m.f1 << ','
			 << m.f2 << ','
			 << m.f3 << ','
			 << m.f4 << ','
			 << m.f5 << ','
			 << m.f6 << ','
			 << m.f7 << ','
			 << m.f8 << ','
			 << m.f9 << ','
			 << m.f10 << ','
			 << m.f11 << ','
			 << m.f12 << ','
			 << m.f13 << ','
			 << m.f14 << ','
			 << m.f15 << ','
			 << m.f16 << ','
			 << m.f17 << ','
			 << m.f18 << ','
			 << m.f19 << ','
			 << m.f20 << ','
			 << m.f21 << ','
			 << m.f22 << ','
			 << m.f23 << ','
			 << m.f24 << ','
			 << m.f25 << ','
			 << m.f26 << ','
			 << m.f27 << ','
			 << m.f28 << ','
			 << m.f29 << ')';
		;
}


TEST(convert, benchmark)
{
	benchmark m1;

	msgpack::sbuffer sbuf;
	msgpack::pack(sbuf, m1);

	msgpack::zone z;
	msgpack::object obj;

	msgpack::unpack_return ret =
		msgpack::unpack(sbuf.data(), sbuf.size(), NULL, &z, &obj);
	EXPECT_EQ(ret, msgpack::UNPACK_SUCCESS);

	benchmark m2;
	for (long int i = 0; i < 100; ++i)
		for (long int j = 0; j < 1000000; ++j) {
			obj.convert(&m2);
			EXPECT_EQ(m1, m2);
		}
}

