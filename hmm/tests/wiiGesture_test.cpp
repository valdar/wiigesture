/*// for BOOST testing
#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;

// the test classes
#include "hmm_test.cpp"

// test program entry point
test_suite* init_unit_test_suite(int /* argc *//*, char** /* argv *///)
/*{
   // create the top test suite
   boost::shared_ptr<test_suite> top_test_suite(BOOST_TEST_SUITE("wiiGesture test suite"));

   // add test suites to the top test suite
   top_test_suite->add(new hmm_test_suite());

   return top_test_suite.shared_ptr;
}
*/
