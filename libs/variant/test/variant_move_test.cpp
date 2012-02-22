//-----------------------------------------------------------------------------
// boost-libs variant/test/variant_move_test.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2012
// Evan Wallace
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "boost/variant.hpp"
#include "boost/test/minimal.hpp"
#include "boost/move/move.hpp"

namespace variant_move_test {

struct MovableButNotCopyable {
    MovableButNotCopyable() : i(0) {}
    MovableButNotCopyable(int newi) : i(newi) {}
    MovableButNotCopyable(BOOST_RV_REF(MovableButNotCopyable) o) : i(o.i)
    {
        o.i = 0;
    }
    MovableButNotCopyable& operator=(BOOST_RV_REF(MovableButNotCopyable) o)
    {
        i = o.i;
        o.i = 0;
        return *this;
    }

    int i;
private:
    BOOST_MOVABLE_BUT_NOT_COPYABLE(MovableButNotCopyable)
};


struct MovableAndCopyable {
    MovableAndCopyable() {}
    MovableAndCopyable(MovableAndCopyable const& o) {
        
    }
    MovableAndCopyable& operator=(BOOST_COPY_ASSIGN_REF(MovableAndCopyable) o) {
        return *this;
    }
    MovableAndCopyable(BOOST_RV_REF(MovableAndCopyable) o) {
    
    }
    MovableAndCopyable& operator=(BOOST_RV_REF(MovableAndCopyable) o) {
        return *this;
    }

private:
    BOOST_COPYABLE_AND_MOVABLE(MovableAndCopyable)
};


} //namespace variant_move_test

int test_main(int , char* [])
{
    return 0;
}
