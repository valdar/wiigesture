#include <hmm.h>
#define BOOST_TEST_MODULE hmm_test

#ifdef BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#endif

#ifndef BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>
#endif


BOOST_AUTO_TEST_CASE( isErgodic_test )
{
    HMM test_object(3, 5, false, 1);

    BOOST_REQUIRE( !test_object.getIsErgodic() );
}

