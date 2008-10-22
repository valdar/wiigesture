#ifdef BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN wiiGesture
#include <boost/test/unit_test.hpp>
#endif

#ifndef BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>
#endif

#include "hmm_test.cpp"
#include "Gaussian_3d_test.cpp"
