#include <vector>
#include <iostream>
#include <utility>
#include <string>
#include <boost/variant.hpp>

template<typename G>
class TypeTraits {
 public:
  typedef typename G::NodeType NodeType;
  typedef typename G::NodeId NodeId;
  typedef typename G::Attribute Attribute;
  typedef typename G::AttributeList AttributeList;
  typedef typename G::Node Node;
  typedef typename G::Edge Edge;
  typedef typename G::NodeList NodeList;
  typedef typename G::EdgeList EdgeList;
};

class AttributePrintVisitor : public boost::static_visitor<>
{
 public:
  void operator()(int& i) const { std::cout << i; } 
  void operator()(float& f) const { std::cout << f; } 
  void operator()(std::string& s) const { std::cout << s; } 
};

class DistanceVisitor : public boost::static_visitor<double>
{
 public:
  template<typename T, typename U>
    double operator() (T&, U&) const { std::cerr << "Can't compare different types!" << std::endl; return 1.0; }
  template<typename T>
    double operator() (T& t1, T& t2) const { return t1 == t2 ? 0.0 : 1.0; }
};

class Graph {
 public:
  typedef unsigned int NodeType;
  typedef size_t NodeId;
  typedef boost::variant<int, float, std::string> Attribute;
  typedef std::map<std::string, Attribute> AttributeList;
  typedef std::pair<NodeType, AttributeList> Node;
  typedef std::pair<NodeId, NodeId> Edge;
  typedef std::vector<Node> NodeList ;
  typedef std::vector<Edge> EdgeList;

  Graph(const NodeList& nodes, const EdgeList& edges)
   : _nodes(nodes), _edges(edges)
  {}

  NodeList nodes() { return _nodes; }

 private:
  NodeList _nodes;
  EdgeList _edges;

};

template<typename Graph>
  bool walkTheNodes (Graph& g)
{
  int count = 1;
  for (auto node : g.nodes()) {
    std::cout << "node " << count++ << ":\n";
    for (auto attribute : node.second) {
      std::cout << attribute.first << ": ";
      boost::apply_visitor(AttributePrintVisitor(), attribute.second);
      std::cout << "\n";
    }
    std::cout << "\n";
  }
  return true;
}

template<typename Graph>
class Distance {
  typedef typename TypeTraits<Graph>::Node Node;
  Node node_ref;

 public:
 distance(const Node& node) : node_ref(node) {}

  double operator()(const Node& node) const {
    double distance_sum = 0.0;
    Node::const_iterator it_ref = node_ref.begin();
    Node::const_iterator it = node.begin();
    while (it_ref != node_ref.end() and it != node.end()) {
      distance_sum += boost::apply_visitor(DistanceVisitor(), node_ref->second, node->second);
    }
    return distance_sum;
  }
};

template<typename Graph>
bool computeDistance(Graph& g, typename TypeTraits<Graph>::Node& node_ref)
{
  Distance<Graph> distance_ref(node_ref);
  for (auto node : g.nodes()) {
    std::cout << "distance = " << distance_ref(node) << "\n";
  }
  return true;
}

