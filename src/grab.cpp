#include <iostream>
#include "gtest/gtest.h"
#include "Graph.h"

TEST(Graph, Load) {
  GraphTraits::AttributeList attr_1 = { {"poi", "home"}, {"id", 2}, {"date", "2019-09-18"} };
  GraphTraits::AttributeList attr_2 = { {"poi", "work"}, {"id", 4}, {"date", "2019-09-18"} };
  GraphTraits::AttributeList attr_3 = { {"poi", "home"}, {"id", 3}, {"date", "2019-09-17"} };
  GraphTraits::AttributeList attr_4 = { {"poi", "shop"}, {"id", 4}, {"date", "2019-09-17"} };

  GraphTraits::NodeType context_node = 1;

  GraphTraits::NodeList nodes = { {context_node, attr_1},
                     {context_node, attr_2},
                     {context_node, attr_3},
                     {context_node, attr_4}};
  GraphTraits::EdgeList edges = { {0,1}, {1,2}, {2,3} };

  Graph g(nodes, edges);

  walkTheNodes<Graph, GraphTraits>(g);
}
