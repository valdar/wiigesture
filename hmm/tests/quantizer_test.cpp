#include <vector>
#include <iostream>
#include <boost/test/floating_point_comparison.hpp>
#include "sample_3d.h"
#include "gesture.h"
#include "quantizer.h"


BOOST_AUTO_TEST_SUITE( quantizer_test )

BOOST_AUTO_TEST_CASE( getDiscreteSequence )
{

    Sample_3d a(4.0, 0.01, 0.01);
    Sample_3d b(0.01, 0.01, 4.0);
    Sample_3d c(-4.0, 0.01, 0.01);
    Sample_3d d(0.01, 0.01, -4.0);

    int expected_classification[4] = {0, 2, 4, 6};

    Gesture gesture;

    gesture.add(a);
    gesture.add(b);
    gesture.add(c);
    gesture.add(d);

    Quantizer quantizer;

    int* result = quantizer.getDiscreteSequence(gesture);

    for(int i=0; i<4; i++){

         BOOST_REQUIRE( expected_classification[i] == result[i] );

    }


}

BOOST_AUTO_TEST_SUITE_END()
