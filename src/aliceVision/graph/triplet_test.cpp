// This file is part of the AliceVision project and is made available under
// the terms of the MPL2 license (see the COPYING.md file).

#include "aliceVision/graph/Triplet.hpp"

#include <iostream>
#include <vector>

#define BOOST_TEST_MODULE tripletFinder
#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

using namespace aliceVision::graph;

BOOST_AUTO_TEST_CASE(test_no_triplet) {

  typedef lemon::ListGraph Graph;

  // a_b_c
  Graph ga;
  Graph::Node a = ga.addNode(), b = ga.addNode(), c = ga.addNode();
  ga.addEdge(a,b);
  ga.addEdge(b,c);

  std::vector< Triplet > vec_triplets;

  BOOST_CHECK(!List_Triplets(ga, vec_triplets));
  BOOST_CHECK(vec_triplets.empty());
}

BOOST_AUTO_TEST_CASE(test_one_triplet) {

  typedef lemon::ListGraph Graph;

  {
    //
    // a_b
    // |/
    // c
    Graph ga;
    Graph::Node a = ga.addNode(), b = ga.addNode(), c = ga.addNode();
    ga.addEdge(a,b);
    ga.addEdge(a,c);
    ga.addEdge(b,c);

    std::vector< Triplet > vec_triplets;
    BOOST_CHECK(List_Triplets(ga, vec_triplets));
    BOOST_CHECK_EQUAL(1, vec_triplets.size());
    //Check the cycle values
    BOOST_CHECK_EQUAL(0,vec_triplets[0].i);
    BOOST_CHECK_EQUAL(1,vec_triplets[0].j);
    BOOST_CHECK_EQUAL(2,vec_triplets[0].k);
  }

  {
    //
    // a_b__c
    //    |/
    //    d
    Graph ga;
    Graph::Node a = ga.addNode(), b = ga.addNode(),
      c = ga.addNode(), d = ga.addNode();
    ga.addEdge(a,b);
    ga.addEdge(b,c);
    ga.addEdge(b,d);
    ga.addEdge(c,d);

    std::vector< Triplet > vec_triplets;
    BOOST_CHECK(List_Triplets(ga, vec_triplets));
    BOOST_CHECK_EQUAL(1, vec_triplets.size());
    //Check the cycle values
    BOOST_CHECK_EQUAL(1,vec_triplets[0].i);
    BOOST_CHECK_EQUAL(2,vec_triplets[0].j);
    BOOST_CHECK_EQUAL(3,vec_triplets[0].k);
  }
}

BOOST_AUTO_TEST_CASE(test_two_triplet) {

  typedef lemon::ListGraph Graph;

  {
    //
    // a__b
    // |\ |
    // | \|
    // c--d
    Graph ga;
    Graph::Node a = ga.addNode(), b = ga.addNode(),
      c = ga.addNode(), d = ga.addNode();

    ga.addEdge(a,b);
    ga.addEdge(a,c);
    ga.addEdge(a,d);
    ga.addEdge(c,d);
    ga.addEdge(b,d);

    std::vector< Triplet > vec_triplets;
    BOOST_CHECK(List_Triplets(ga, vec_triplets));
    BOOST_CHECK_EQUAL(2, vec_triplets.size());
  }

  {
    //
    // a   c
    // |\ /|
    // | b |
    // |/ \|
    // d   e
    Graph ga;
    Graph::Node a = ga.addNode(), b = ga.addNode(),
      c = ga.addNode(), d = ga.addNode(),
      e = ga.addNode();

    ga.addEdge(a,b);
    ga.addEdge(b,c);
    ga.addEdge(c,e);
    ga.addEdge(e,b);
    ga.addEdge(b,d);
    ga.addEdge(d,a);

    std::vector< Triplet > vec_triplets;
    BOOST_CHECK(List_Triplets(ga, vec_triplets));
    BOOST_CHECK_EQUAL(2, vec_triplets.size());
  }

    {
    //
    // a      c
    // |\    /|
    // | b--f |
    // |/    \|
    // d      e
    Graph ga;
    Graph::Node a = ga.addNode(), b = ga.addNode(),
      c = ga.addNode(), d = ga.addNode(),
      e = ga.addNode(), f = ga.addNode();

    ga.addEdge(a,b);
    ga.addEdge(b,f);
    ga.addEdge(f,c);
    ga.addEdge(c,e);
    ga.addEdge(e,f);
    ga.addEdge(f,b);
    ga.addEdge(b,d);
    ga.addEdge(d,a);

    std::vector< Triplet > vec_triplets;
    BOOST_CHECK(List_Triplets(ga, vec_triplets));
    BOOST_CHECK_EQUAL(2, vec_triplets.size());
  }
}


BOOST_AUTO_TEST_CASE(test_three_triplet) {

  typedef lemon::ListGraph Graph;

  {
    //
    // a   b
    // |\ /|
    // c-d-e
    // |/
    // f
    Graph ga;
    Graph::Node a = ga.addNode(), b = ga.addNode(),
      c = ga.addNode(), d = ga.addNode(),
      e = ga.addNode(), f = ga.addNode();

    ga.addEdge(a,c);
    ga.addEdge(a,d);
    ga.addEdge(c,d);
    ga.addEdge(c,f);
    ga.addEdge(f,d);
    ga.addEdge(d,b);
    ga.addEdge(b,e);
    ga.addEdge(e,d);

    std::vector< Triplet > vec_triplets;
    BOOST_CHECK(List_Triplets(ga, vec_triplets));
    BOOST_CHECK_EQUAL(3, vec_triplets.size());
  }

  {
    //
    // a        b--g--h
    // | \    / |   \/
    // |  d--e  |    i
    // | /    \ |
    // c        f
    Graph ga;
    Graph::Node a = ga.addNode(), b = ga.addNode(),
      c = ga.addNode(), d = ga.addNode(),
      e = ga.addNode(), f= ga.addNode(),
      g = ga.addNode(), h = ga.addNode(),
      i = ga.addNode();

    ga.addEdge(a,c);
    ga.addEdge(a,d);
    ga.addEdge(d,c);
    ga.addEdge(d,e);
    ga.addEdge(e,b);
    ga.addEdge(e,f);
    ga.addEdge(b,f);
    ga.addEdge(b,g);
    ga.addEdge(g,h);
    ga.addEdge(h,i);
    ga.addEdge(i,g);

    std::vector< Triplet > vec_triplets;
    BOOST_CHECK(List_Triplets(ga, vec_triplets));
    BOOST_CHECK_EQUAL(3, vec_triplets.size());
  }

    {
    //
    // a---b
    // |\  |\
    // | \ | \
    // |  \|  \
    // c---d---e
    Graph ga;
    Graph::Node a = ga.addNode(), b = ga.addNode(),
      c = ga.addNode(), d = ga.addNode(),
      e = ga.addNode();

    ga.addEdge(a,b);
    ga.addEdge(b,d);
    ga.addEdge(d,c);
    ga.addEdge(c,a);
    ga.addEdge(a,d);
    ga.addEdge(b,e);
    ga.addEdge(d,e);

    std::vector< Triplet > vec_triplets;
    BOOST_CHECK(List_Triplets(ga, vec_triplets));
    BOOST_CHECK_EQUAL(3, vec_triplets.size());
  }
}

BOOST_AUTO_TEST_CASE(test_for_triplet) {

  typedef lemon::ListGraph Graph;
 {
    //
    // a__b
    // |\/|
    // |/\|
    // c--d
    Graph ga;
    Graph::Node a = ga.addNode(), b = ga.addNode(),
      c = ga.addNode(), d = ga.addNode();

    ga.addEdge(a,b);
    ga.addEdge(a,c);
    ga.addEdge(a,d);
    ga.addEdge(c,d);
    ga.addEdge(b,d);
    ga.addEdge(c,b);

    std::vector< Triplet > vec_triplets;
    BOOST_CHECK(List_Triplets(ga, vec_triplets));
    BOOST_CHECK_EQUAL(4, vec_triplets.size());
  }
}
