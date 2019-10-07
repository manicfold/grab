#include <map>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

class Graph {
public:
  enum NodeType { CONTEXT_NODE, ACTION_NODE };
  enum AttributeType { UNKNOWN, TIME, POI, ID, TEMPERATURE, DATE };

  typedef size_t                                   NodeId;
  typedef size_t                                   EdgeId;
  typedef std::variant<int, float, std::string>    AttributeValue;
  typedef std::pair<AttributeType, AttributeValue> Attribute;
  typedef std::map<AttributeType, AttributeValue>  AttributeList;
  typedef std::pair<NodeType, AttributeList>       Node;
  typedef std::pair<NodeId, NodeId>                Edge;
  typedef std::vector<Node>                        NodeList;
  typedef NodeList::const_iterator                 NodeConstIterator;
  typedef std::vector<Edge>                        EdgeList;

  Graph(const NodeList &nodes, const EdgeList &edges)
      : _nodes(nodes), _edges(edges) {}

  NodeList nodes() { return _nodes; }
  NodeList contextNodes()
  {
    NodeList contextNodes;
    for (auto node : _nodes) {
      if (node.first == CONTEXT_NODE) {
        contextNodes.push_back(node);
      }
    }
    return contextNodes;
  };

  AttributeType createType(const std::string& name)
  {
    return UNKNOWN;
  }

  NodeId addNode(const NodeType &node_type)
  {
    _nodes.push_back({node_type, AttributeList()});
    return _nodes.size();
  }

  NodeId addAttribute(const NodeId &node_id, const AttributeType& attr_type, const AttributeValue& attr_value)
  {
    _nodes[node_id].second[attr_type] = attr_value;
    return node_id;
  }

  EdgeId createEdge(const NodeId &head, const NodeId &tail)
  {
    _edges.push_back({head, tail});
    return _edges.size();
  }

  const AttributeList &attributes(const NodeId &node) const {
    return _nodes[node].second;
  }
  const AttributeList &attributes(const Node &node) const {
    return node.second;
  }

private:
  NodeList _nodes;
  EdgeList _edges;
};
