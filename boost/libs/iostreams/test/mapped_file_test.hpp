// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef BOOST_IOSTREAMS_TEST_MAPPED_FILE_HPP_INCLUDED
#define BOOST_IOSTREAMS_TEST_MAPPED_FILE_HPP_INCLUDED

#include <fstream>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream_facade.hpp>
#include <boost/test/test_tools.hpp>
#include "detail/temp_file.hpp"
#include "detail/verification.hpp"

void mapped_file_test()
{
    using namespace std;
    using namespace boost;
    using namespace boost::iostreams;
    using namespace boost::iostreams::test;

    //--------------Reading from a mapped_file_source-------------------------//

    {
        // Test reading from a stream_facade based on a mapped_file_source,
        // in chars.
        test_file test;
        stream_facade<mapped_file_source> first(test.name());
        ifstream second( test.name().c_str(), 
                         std::ios::in | std::ios::binary );
        BOOST_CHECK_MESSAGE(
            compare_streams_in_chars(first, second),
            "failed reading from stream_facade<mapped_file_source> in chars"
        );

        first.close();
        second.close();

        // Test reading from a stream_facade based on a mapped_file_source,
        // in chunks. (Also tests reopening the stream_facade.)
        first.open(mapped_file_source(test.name()));
        second.open( test.name().c_str(), 
                     std::ios::in | std::ios::binary );
        BOOST_CHECK_MESSAGE(
            compare_streams_in_chunks(first, second),
            "failed reading from stream_facade<mapped_file_source> in chunks"
        );
    }

    //--------------Writing to a mapped_file_sink-----------------------------//

    {
        // Test writing to a stream_facade based on a mapped_file_sink, in 
        // chars.
        uppercase_file  first, second; // Will overwrite these.
        test_file       test;

        stream_facade<mapped_file_sink> 
            out((mapped_file_sink(first.name()))); // CW8.0 workaround.
        write_data_in_chars(out);
        out.close();
        BOOST_CHECK_MESSAGE(
            compare_files(first.name(), test.name()),
            "failed writing to stream_facade<mapped_file_sink> in chars"
        );

        // Test writing to a stream_facade based on a mapped_file_sink, in 
        // chunks. (Also tests reopening the stream.)
        out.open(mapped_file_sink(second.name()));
        write_data_in_chunks(out);
        out.close();
        BOOST_CHECK_MESSAGE(
            compare_files(second.name(), test.name()),
            "failed writing to stream_facade<mapped_file_sink> in chunks"
        );
    }

    //--------------Random access with a mapped_file--------------------------//

    {
        // Test reading, writing and seeking within a stream_facade based on a 
        // mapped_file, in chars.
        test_file test;
        stream_facade<mapped_file> 
            io((mapped_file(test.name()))); // CW workaround.
        BOOST_CHECK_MESSAGE(
            test_seekable_in_chars(io),
            "failed seeking within stream_facade<mapped_file> in chars"
        );

        io.close();

        // Test reading, writing and seeking within a stream_facade based on a 
        // mapped_file, in chunks. (Also tests reopening the 
        // stream_facade.)
        io.open(mapped_file(test.name()));
        BOOST_CHECK_MESSAGE(
            test_seekable_in_chunks(io),
            "failed seeking within stream_facade<mapped_file> in chunks"
        );
    }
}

#endif // #ifndef BOOST_IOSTREAMS_TEST_MAPPED_FILE_HPP_INCLUDED
