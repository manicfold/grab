#include <iostream>
#include "gtest/gtest.h"
#include "Graph.h"

TEST(Graph, Load) {
  Graph::AttributeList attr_1 = {{Graph::POI, "home"}, {Graph::ID, 2}, {Graph::POI, "2019-09-18"}};
  Graph::AttributeList attr_2 = {{Graph::POI, "work"}, {Graph::ID, 4}, {Graph::POI, "2019-09-18"}};
  Graph::AttributeList attr_3 = {{Graph::POI, "home"}, {Graph::ID, 3}, {Graph::POI, "2019-09-17"}};
  Graph::AttributeList attr_4 = {{Graph::POI, "shop"}, {Graph::ID, 4}, {Graph::POI, "2019-09-17"}};

  Graph::NodeType context_node = Graph::CONTEXT_NODE;

  Graph::NodeList nodes = { {context_node, attr_1},
                     {context_node, attr_2},
                     {context_node, attr_3},
                     {context_node, attr_4}};
  Graph::EdgeList edges = { {0,1}, {1,2}, {2,3} };

  Graph g(nodes, edges);

  walkTheNodes<Graph>(g);

  computeDistance<Graph>(g, g.nodes()[0]);
}
