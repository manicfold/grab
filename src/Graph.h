#include "GraphImpl.h"

#include <iostream>

class AttributePrintVisitor {
public:
  void operator()(int &i) const { std::cout << i; }
  void operator()(float &f) const { std::cout << f; }
  void operator()(std::string &s) const { std::cout << s; }
};

//! A binary visitor for variant AttributeValue, that computes the distance between
//! two values.
class DistanceVisitor {
public:
  template <typename T, typename U> double operator()(T &, U &) const {
    std::cerr << "Can't compare different types!" << std::endl;
    return 1.0;
  }
  template <typename T> double operator()(T &t1, T &t2) const {
    return t1 == t2 ? 0.0 : 1.0;
  }
};

template <typename G> class TypeTraits {
public:
  typedef typename G::NodeType       NodeType;
  typedef typename G::NodeId         NodeId;
  typedef typename G::AttributeValue AttributeValue;
  typedef typename G::AttributeList  AttributeList;
  typedef typename G::Node           Node;
  typedef typename G::Edge           Edge;
  typedef typename G::NodeList       NodeList;
  typedef typename G::EdgeList       EdgeList;
};

template <typename Graph> bool walkTheNodes(Graph &g) {
  int count = 1;
  for (auto node : g.nodes()) {
    std::cout << "node " << count++ << ":\n";
    for (auto attribute : node.second) {
      std::cout << attribute.first << ": ";
      std::visit(AttributePrintVisitor(), attribute.second);
      std::cout << "\n";
    }
    std::cout << "\n";
  }
  return true;
}

//! A distance measure on the context space for the given KnowledgeGraph.
template <typename Graph> class Distance {
  typedef typename TypeTraits<Graph>::Node Node;
  typedef typename TypeTraits<Graph>::AttributeList AttributeList;
  const Node &node_ref;
  const Graph &graph;

public:
  Distance(const Graph &g, const Node &node) : node_ref(node), graph(g) {}

  double operator()(const Node &node) const {
    double distance_sum = 0.0;
    const AttributeList &attr_ref = graph.attributes(node_ref);
    const AttributeList &attr = graph.attributes(node);
    typename AttributeList::const_iterator it_ref = attr_ref.begin();
    typename AttributeList::const_iterator it = attr.begin();
    while (it_ref != attr_ref.end() and it != attr.end()) {
      distance_sum += std::visit(DistanceVisitor(), it_ref->second, it->second);
      it_ref++;
      it++;
    }
    return distance_sum;
  }
};

template <typename Graph>
void computeDistance(Graph &g, typename TypeTraits<Graph>::Node &node_ref) {
  Distance<Graph> distance_ref(g, node_ref);
  for (auto node : g.contextNodes()) {
    std::cout << "distance = " << distance_ref(node) << "\n";
  }
}
