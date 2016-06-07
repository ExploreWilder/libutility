/**
 * @file utility/enum-io.hpp
 * @author Vaclav Blazek <vaclav.blazek@citationtech.net>
 *
 * Enum IO support. Generates operator<<(ostream, t) and operator<<(istream, T)
 * based on supplied mapping.
 *
 * Enum value <-> string mapping uses Boost.Preprocessor sequences. Mapping is
 * sequence of 1- or 2-element sequences, one for each enum value.
 *
 * To generate I/O stream operators for your enum class EnumType { a, b, c }
 * write:
 *
 * UTILITY_GENERATE_ENUM_IO(EnumType,
 *                          ((a))
 *                          ((b))
 *                          ((c))
 *                          )
 *
 * NB: notice no semicolon at the end of macro call!
 *
 * If string representation is different than C++ identifier add second element
 * to mapping tupple, e.g. for ImageOrientation::left_top represented as
 * "left-top" write: ((left_top)("left-top"))
 *
 * These support functions are also generated:
 *     std::initializer_list<Type> enumerationValues(Type)
 *     const char* enumerationString(Type)
 */

#ifndef utility_enum_io_hpp_included_
#define utility_enum_io_hpp_included_

#include <iostream>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/algorithm/string/predicate.hpp>

#define UTILITY_DETAIL_fromEnum_element1(r,Type,value)              \
    case Type::BOOST_PP_SEQ_ELEM(0, value):                         \
    return os << BOOST_PP_STRINGIZE(BOOST_PP_SEQ_ELEM(0, value));

#define UTILITY_DETAIL_fromEnum_element2(r,Type,value)   \
    case Type::BOOST_PP_SEQ_ELEM(0, value):              \
    return os << BOOST_PP_SEQ_ELEM(1, value);

#define UTILITY_DETAIL_fromEnum_element3(r,Type,value)   \
    UTILITY_DETAIL_fromEnum_element2(r,Type,value)

#define UTILITY_DETAIL_fromEnum_element4(r,Type,value)  \
    UTILITY_DETAIL_fromEnum_element2(r,Type,value)

#define UTILITY_DETAIL_fromEnum_element5(r,Type,value)  \
    UTILITY_DETAIL_fromEnum_element2(r,Type,value)

#define UTILITY_DETAIL_fromEnum_element6(r,Type,value)  \
    UTILITY_DETAIL_fromEnum_element2(r,Type,value)

#define UTILITY_DETAIL_fromEnum_element7(r,Type,value)  \
    UTILITY_DETAIL_fromEnum_element2(r,Type,value)

#define UTILITY_DETAIL_fromEnum_element(r,type,value)                   \
    BOOST_PP_CAT(UTILITY_DETAIL_fromEnum_element,BOOST_PP_SEQ_SIZE(value)) \
    (r,type,value)

#define UTILITY_DETAIL_toEnum_element1(r,Type,value)                \
    if (compare(s, BOOST_PP_STRINGIZE(BOOST_PP_SEQ_ELEM(0, value)))) {  \
        out = Type::BOOST_PP_SEQ_ELEM(0, value);                    \
        return is;                                                  \
    }

#define UTILITY_DETAIL_toEnum_element2(r,Type,value)                \
    if (compare(s, BOOST_PP_SEQ_ELEM(1, value))) {                  \
        out = Type::BOOST_PP_SEQ_ELEM(0, value);                    \
        return is;                                                  \
    }

#define UTILITY_DETAIL_toEnum_element3(r,Type,value)                \
    UTILITY_DETAIL_toEnum_element2(r,Type,value)                    \
    if (compare(s, BOOST_PP_SEQ_ELEM(2, value))) {                  \
        out = Type::BOOST_PP_SEQ_ELEM(0, value);                    \
        return is;                                                  \
    }

#define UTILITY_DETAIL_toEnum_element4(r,Type,value)                \
    UTILITY_DETAIL_toEnum_element3(r,Type,value)                    \
    if (compare(s, BOOST_PP_SEQ_ELEM(3, value))) {                  \
        out = Type::BOOST_PP_SEQ_ELEM(0, value);                    \
        return is;                                                  \
    }

#define UTILITY_DETAIL_toEnum_element5(r,Type,value)                \
    UTILITY_DETAIL_toEnum_element4(r,Type,value)                    \
    if (compare(s, BOOST_PP_SEQ_ELEM(4, value))) {                  \
        out = Type::BOOST_PP_SEQ_ELEM(0, value);                    \
        return is;                                                  \
    }

#define UTILITY_DETAIL_toEnum_element6(r,Type,value)                \
    UTILITY_DETAIL_toEnum_element5(r,Type,value)                    \
    if (compare(s, BOOST_PP_SEQ_ELEM(5, value))) {                  \
        out = Type::BOOST_PP_SEQ_ELEM(0, value);                    \
        return is;                                                  \
    }

#define UTILITY_DETAIL_toEnum_element7(r,Type,value)                \
    UTILITY_DETAIL_toEnum_element6(r,Type,value)                    \
    if (compare(s, BOOST_PP_SEQ_ELEM(6, value))) {                  \
        out = Type::BOOST_PP_SEQ_ELEM(0, value);                    \
        return is;                                                  \
    }

#define UTILITY_DETAIL_toEnum_element(r,type,value)                     \
    BOOST_PP_CAT(UTILITY_DETAIL_toEnum_element,BOOST_PP_SEQ_SIZE(value)) \
    (r,type,value)

#define UTILITY_DETAIL_value(Type,value)                                \
    Type::BOOST_PP_SEQ_ELEM(0, value)

#define UTILITY_DETAIL_name1(Type,value)                                \
    BOOST_PP_STRINGIZE(BOOST_PP_SEQ_ELEM(0, value))

#define UTILITY_DETAIL_name2(Type,value)                                \
    BOOST_PP_SEQ_ELEM(1, value)

#define UTILITY_DETAIL_name3(Type,value)                                \
    UTILITY_DETAIL_name2(Type,value)

#define UTILITY_DETAIL_name4(Type,value)                                \
    UTILITY_DETAIL_name2(Type,value)

#define UTILITY_DETAIL_name5(Type,value)                                \
    UTILITY_DETAIL_name2(Type,value)

#define UTILITY_DETAIL_name6(Type,value)                                \
    UTILITY_DETAIL_name2(Type,value)

#define UTILITY_DETAIL_name7(Type,value)                                \
    UTILITY_DETAIL_name2(Type,value)

#define UTILITY_DETAIL_name(Type,value)                                 \
    BOOST_PP_CAT(UTILITY_DETAIL_name,BOOST_PP_SEQ_SIZE(value))          \
    (Type,value)

#define UTILITY_DETAIL_comma_value(r,Type,value)                        \
    , UTILITY_DETAIL_value(Type,value)

#define UTILITY_DETAIL_comma_name(r,Type,value)                         \
    ", " UTILITY_DETAIL_name(Type,value)

#define UTILITY_DETAIL_untyped_value(Type,value)                        \
    BOOST_PP_SEQ_ELEM(0, value)

#define UTILITY_DETAIL_comma_untyped_value(r,Type,value)                \
    , UTILITY_DETAIL_untyped_value(Type,value)

#define UTILITY_DETAIL_toEnum_compare0                                  \
    auto compare([](const std::string &l, const std::string &r) {       \
            return l == r;                                              \
        });

#define UTILITY_DETAIL_toEnum_compare1                                  \
    auto compare([](const std::string &l, const std::string &r) {       \
            return boost::algorithm::iequals(l, r);                    \
        });

#define UTILITY_GENERATE_ENUM_IO_IMPL(Type, seq, ci)                    \
    template <typename E, typename T>                                   \
    inline std::basic_ostream<E, T>&                                    \
    operator<<(std::basic_ostream<E, T> &os, const Type &value)         \
    {                                                                   \
        switch (value) {                                                \
        BOOST_PP_SEQ_FOR_EACH(UTILITY_DETAIL_fromEnum_element, Type, seq) \
        }                                                               \
        os.setstate(std::ios::failbit);                                 \
        return os;                                                      \
    }                                                                   \
                                                                        \
    template <typename E, typename T>                                   \
    inline std::basic_istream<E, T>&                                    \
    operator>>(std::basic_istream<E, T> &is, Type &out)                 \
    {                                                                   \
        std::string s;                                                  \
        is >> s;                                                        \
        BOOST_PP_CAT(UTILITY_DETAIL_toEnum_compare,ci)                  \
        BOOST_PP_SEQ_FOR_EACH(UTILITY_DETAIL_toEnum_element, Type, seq) \
        out = {};                                                       \
        is.setstate(std::ios::failbit);                                 \
        return is;                                                      \
    }                                                                   \
                                                                        \
    inline std::initializer_list<Type> enumerationValues(Type)          \
    {                                                                   \
        return {                                                        \
            UTILITY_DETAIL_value(Type, BOOST_PP_SEQ_HEAD(seq))          \
            BOOST_PP_SEQ_FOR_EACH(UTILITY_DETAIL_comma_value            \
                                  , Type, BOOST_PP_SEQ_TAIL(seq))       \
        };                                                              \
    }                                                                   \
                                                                        \
    inline const char* enumerationString(Type)                          \
    {                                                                   \
        return UTILITY_DETAIL_name(Type, BOOST_PP_SEQ_HEAD(seq))        \
            BOOST_PP_SEQ_FOR_EACH(UTILITY_DETAIL_comma_name, Type       \
                                  , BOOST_PP_SEQ_TAIL(seq));            \
    }

/** Generate I/O code for any enum (stream operator<< and operator>>).
 */
#define UTILITY_GENERATE_ENUM_IO(Type, seq)                             \
    UTILITY_GENERATE_ENUM_IO_IMPL(Type, seq, 0)

/** Case-insensitive equivalent of UTILITY_GENERATE_ENUM_IO.
 */
#define UTILITY_GENERATE_ENUM_IO_CI(Type, seq)                          \
    UTILITY_GENERATE_ENUM_IO_IMPL(Type, seq, 1)

/** Generate enum class and I/O stuff at one go
 */
#define UTILITY_GENERATE_ENUM(Type, seq)                                \
    enum class Type {                                                   \
        UTILITY_DETAIL_untyped_value(Type, BOOST_PP_SEQ_HEAD(seq))      \
        BOOST_PP_SEQ_FOR_EACH(UTILITY_DETAIL_comma_untyped_value        \
                              , Type, BOOST_PP_SEQ_TAIL(seq))           \
    };                                                                  \
                                                                        \
    UTILITY_GENERATE_ENUM_IO(Type, seq)

#endif // utility_enum_io_hpp_included_
