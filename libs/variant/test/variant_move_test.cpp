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


struct CopyableAndMovable {
    CopyableAndMovable() {}
    CopyableAndMovable(CopyableAndMovable const& o) {
        
    }
    CopyableAndMovable& operator=(BOOST_COPY_ASSIGN_REF(CopyableAndMovable) o) {
        return *this;
    }
    CopyableAndMovable(BOOST_RV_REF(CopyableAndMovable) o) {
    
    }
    CopyableAndMovable& operator=(BOOST_RV_REF(CopyableAndMovable) o) {
        return *this;
    }

private:
    BOOST_COPYABLE_AND_MOVABLE(CopyableAndMovable)
};

void run() {
    
    {
        boost::variant<int> a;
    }
        //Construct from element -
        //boost::variant<MovableButNotCopyable> b(MovableButNotCopyable());
        //Move construct -
        //boost::variant<MovableButNotCopyable>(boost::move(a));
        
        //Assign --
        //Assign from element
        //b = MovableButNotCopyable();
        //Move assign
        //a = boost::move(b);
        
        //CopyableAndMovable --
        
        
    {
        //Many elements
        //int, MovableButNotCopyable
        //MovableButNotCopyable, MovableButNotCopyable
        //CopyableAndMovable, MovableButNotCopyable
    }
}

} //namespace variant_move_test

int test_main(int , char* [])
{
    variant_move_test::run();
    return 0;
}
