// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef BOOST_IOSTREAMS_TEST_READ_SEEKABLE_SEQUENCE_HPP_INCLUDED
#define BOOST_IOSTREAMS_TEST_READ_SEEKABLE_SEQUENCE_HPP_INCLUDED

#include <fstream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/test/test_tools.hpp>
#include "detail/temp_file.hpp"
#include "detail/verification.hpp"

void read_seekable_sequence_test()
{
    using namespace std;
    using namespace boost;
    using namespace boost::iostreams;
    using namespace boost::iostreams::test;

    test_file        file;     
    test_sequence<>  seq;

    {
        filtering_stream<seekable>  first(seq.begin(), seq.end(), 0);
        ifstream                    second(file.name().c_str());
        BOOST_CHECK_MESSAGE(
            compare_streams_in_chars(first, second),
            "failed reading from filtering_stream<seekable> based on a"
            "sequence in chars with no buffer"
        );
    }

    {
        filtering_stream<seekable>  first(seq.begin(), seq.end(), 0);
        ifstream                    second(file.name().c_str());
        BOOST_CHECK_MESSAGE(
            compare_streams_in_chunks(first, second),
            "failed reading from filtering_stream<seekable> based on a"
            "sequence in chunks with no buffer"
        );
    }

    {
        filtering_stream<seekable>  first(seq.begin(), seq.end());
        ifstream                    second(file.name().c_str());
        BOOST_CHECK_MESSAGE(
            compare_streams_in_chars(first, second),
            "failed reading from filtering_stream<seekable> based on a"
            "sequence in chars with large buffer"
        );
    }

    {
        filtering_stream<seekable>  first(seq.begin(), seq.end());
        ifstream                    second(file.name().c_str());
        BOOST_CHECK_MESSAGE(
            compare_streams_in_chunks(first, second),
            "failed reading from filtering_stream<seekable> based on a"
            "sequence in chunks with large buffer"
        );
    }
}

#endif // #ifndef BOOST_IOSTREAMS_TEST_READ_SEEKABLE_SEQUENCE_HPP_INCLUDED
