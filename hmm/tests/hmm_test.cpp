#include "hmm.h"
#define BOOST_TEST_MODULE HMM_test

BOOST_AUTO_TEST_SUITE( HMM_test )

BOOST_AUTO_TEST_CASE( isErgodic_test )
{
    HMM test_object(3, 5, false, 1);

    BOOST_REQUIRE( !test_object.getIsErgodic() );
}

BOOST_AUTO_TEST_SUITE_END()
