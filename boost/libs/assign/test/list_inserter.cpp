// Boost.Assign library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/assign/
//



#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/functional.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include <stdexcept>
#include <cstdlib>


namespace ba = boost::assign;

template< class T >
void function_ptr( T )
{
    // do nothing
}

struct functor
{
    template< class T >
    void operator()( T ) const
    { 
        // do nothing
    }
};



void check_list_inserter()
{
    using namespace std;
    using namespace boost;
    using namespace boost::assign;
    vector<int> v;
    
    make_list_inserter( &function_ptr<int> )( 5 ),3;
    make_list_inserter( functor() )( 4 ),2;
    make_list_inserter( bind( &vector<int>::push_back, &v, _1 ) )( 6 ),4;
    BOOST_CHECK_EQUAL( v.size(), 2u );
    BOOST_CHECK_EQUAL( v[0], 6 );
    BOOST_CHECK_EQUAL( v[1], 4 );
    
    push_back( v ) = 1,2,3,4,5;
    BOOST_CHECK_EQUAL( v.size(), 7u );
    BOOST_CHECK_EQUAL( v[6], 5 );
    
    push_back( v )(6).repeat( 10, 7 )(8);
    BOOST_CHECK_EQUAL( v.size(), 19u );
    BOOST_CHECK_EQUAL( v[18], 8 );
    BOOST_CHECK_EQUAL( v[8], 7 ); 
    BOOST_CHECK_EQUAL( v[16], 7 ); 
    push_back( v ) = repeat_fun( 10, &rand );
    BOOST_CHECK_EQUAL( v.size(), 29u );
    
    push_back( v ) = 1,repeat( 10, 2 ),3;
    BOOST_CHECK_EQUAL( v.size(), 41u );
    push_back( v ) = 1,repeat_fun( 10, &rand ),2;
    BOOST_CHECK_EQUAL( v.size(), 53u );
    
    typedef map<string,int> map_t;
    typedef map_t::value_type V;
    map_t m;

    make_list_inserter( assign_detail::call_insert< map_t >( m ) )
                      ( V("bar",3) )( V("foo", 2) );
    BOOST_CHECK_EQUAL( m.size(), 2u );
    BOOST_CHECK_EQUAL( m["foo"], 2 );

    typedef vector<int>                   score_type;
    typedef map<string,score_type>        team_score_map;
    typedef std::pair<string,score_type>  score_pair;
    team_score_map team_score;
    insert( team_score )( "Team Foo",    list_of(1)(1)(0) )
                        ( "Team Bar",    list_of(0)(0)(0) )
                        ( "Team FooBar", list_of(0)(0)(1) ); 
    BOOST_CHECK_EQUAL( team_score.size(), 3u );
    BOOST_CHECK_EQUAL( team_score[ "Team Foo" ][1], 1 );
    BOOST_CHECK_EQUAL( team_score[ "Team Bar" ][0], 0 );
    
    team_score = list_of< score_pair >
                        ( "Team Foo",    list_of(1)(1)(0) )
                        ( "Team Bar",    list_of(0)(0)(0) )
                        ( "Team FooBar", list_of(0)(0)(1) );
    BOOST_CHECK_EQUAL( team_score.size(), 3u );
    BOOST_CHECK_EQUAL( team_score[ "Team Foo" ][1], 1 );
    BOOST_CHECK_EQUAL( team_score[ "Team Bar" ][0], 0 );

                        
}



#include <boost/test/included/unit_test_framework.hpp> 

using boost::unit_test_framework::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "List Test Suite" );

    test->add( BOOST_TEST_CASE( &check_list_inserter ) );

    return test;
}

