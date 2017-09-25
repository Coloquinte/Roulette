
#define BOOST_TEST_MODULE HYPERGRAPH_TEST
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "roulette.h"

BOOST_AUTO_TEST_SUITE(simple)

BOOST_AUTO_TEST_CASE(resize_and_set) {
  Roulette<int> r;
  r.resize(1);
  r.check_consistency();
  BOOST_CHECK_EQUAL (r.get_weight(0), 0);
  r.set_weight(0, 1);
  r.check_consistency();
  BOOST_CHECK_EQUAL (r.get_weight(0), 1);
  BOOST_CHECK_EQUAL (r.total_weight(), 1);

  r.resize(3);
  r.check_consistency();
  BOOST_CHECK_EQUAL (r.get_weight(0), 1);
  BOOST_CHECK_EQUAL (r.total_weight(), 1);
  BOOST_CHECK_EQUAL (r.select(0), 0lu);

  r.set_weight(1, 2);
  r.check_consistency();
  BOOST_CHECK_EQUAL (r.total_weight(), 3);
  BOOST_CHECK_EQUAL (r.select(1), 1lu);
  
  r.resize(10000);
  r.check_consistency();
  BOOST_CHECK_EQUAL (r.total_weight(), 3);
  BOOST_CHECK_EQUAL (r.select(1), 1lu);

  r.resize(2);
  r.check_consistency();
  BOOST_CHECK_EQUAL (r.total_weight(), 3);
  BOOST_CHECK_EQUAL (r.select(1), 1lu);

  r.resize(0);
  r.check_consistency();
}

BOOST_AUTO_TEST_SUITE_END()

