#include "node.h"
#include "context.h"
#include "error.h"
#include "topology.h"


Node::Node(const unsigned n, SimulationContext *c, double b, double l) : 
    number(n), context(c), bw(b), lat(l) 
{}

Node::Node() 
{ throw GeneralException(); }

Node::Node(const Node &rhs) : 
  number(rhs.number), context(rhs.context), bw(rhs.bw), lat(rhs.lat) {}

Node & Node::operator=(const Node &rhs) 
{
  return *(new(this)Node(rhs));
}

void Node::SetNumber(const unsigned n) 
{ number=n;}

unsigned Node::GetNumber() const 
{ return number;}

void Node::SetLatency(const double l)
{ lat=l;}

double Node::GetLatency() const 
{ return lat;}

void Node::SetBW(const double b)
{ bw=b;}

double Node::GetBW() const 
{ return bw;}

Node::~Node()
{}

// Implement these functions  to post an event to the event queue in the event simulator
// so that the corresponding node can recieve the ROUTING_MESSAGE_ARRIVAL event at the proper time
void Node::SendToNeighbors(const RoutingMessage *m)
{
}

void Node::SendToNeighbor(const Node *n, const RoutingMessage *m)
{

}

deque<Node*> *Node::GetNeighbors()
{
  return context->GetNeighbors(this);
}

void Node::SetTimeOut(const double timefromnow)
{
  context->TimeOut(this,timefromnow);
}


bool Node::Matches(const Node &rhs) const
{
  return number==rhs.number;
}


#if defined(GENERIC)
  void Node::LinkHasBeenUpdated(const Link *l)
  {
    cerr << *this << " got a link update: "<<*l<<endl;
    //Do Something generic:
    SendToNeighbors(new RoutingMessage);
  }


  void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
  {
    cerr << *this << " got a routing messagee: "<<*m<<" Ignored "<<endl;
  }


  void Node::TimeOut()
  {
    cerr << *this << " got a timeout: ignored"<<endl;
  }

  Node *Node::GetNextHop(const Node *destination) const
  {
    return 0;
  }

  Table *Node::GetRoutingTable() const
  {
    return new Table;
  }


  ostream & Node::Print(ostream &os) const
  {
    os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<")";
    return os;
  }

#endif

#if defined(LINKSTATE)


  void Node::LinkHasBeenUpdated(const Link *l)
  {
    cerr << *this<<": Link Update: "<<*l<<endl;
  }


  void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
  {
    cerr << *this << " Routing Message: "<<*m;
  }

  void Node::TimeOut()
  {
    cerr << *this << " got a timeout: ignored"<<endl;
  }

  Node *Node::GetNextHop(const Node *destination) const
  {
    // WRITE
    return 0;
  }

  Table *Node::GetRoutingTable() const
  {
    // WRITE
    return 0;
  }


  ostream & Node::Print(ostream &os) const
  {
    os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<")";
    return os;
  }
#endif


#if defined(DISTANCEVECTOR)

  void Node::LinkHasBeenUpdated(const Link *l)
  {
    // update our table
    // send out routing mesages
    cerr << *this<<": Link Update: "<<*l<<endl;
  }


  void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
  {

  }

  void Node::TimeOut()
  {
    cerr << *this << " got a timeout: ignored"<<endl;
  }


  Node *Node::GetNextHop(const Node *destination) const
  {
    Table table = *GetRoutingTable();
    map<unsigned, map<unsigned, double> > forwardingTable = table.GetForwardingTable();
    map<unsigned, map<unsigned, double> >::iterator tableIt = forwardingTable.begin();
    // initialized node to be returned, will be returned with next node in path
    Node* returnNode = new Node(*this);
    // destination id of final node
    unsigned destId = destination->GetNumber();
    // links are all outgoing links from current node
    deque<Link*> links = *context->Topology::GetOutgoingLinks(returnNode);
    // number of outgoing linkes
    unsigned linksSize = links.size();
    // shortest distance to destination node
    double minDist = table.GetEntry(number, destId);

    // iterate through links to check for direct link to destination node
    // if direct link exists, check if latency is equal to minDist
          // if so, return returnNode with destId
    for(unsigned i = 0; i < linksSize; i++) {
      Link currLink = *links[i];
      if(currLink.GetDest() == destId) {
        if(minDist == currLink.GetLatency()) {
          returnNode->SetNumber(destId);
          return returnNode;
        }
      }
    }

    // iterate through forwarding table 
    // check if each neighbor has path to destination node
      // if so, check to see if cost(path to neighbor) + cost(neighbor to dest) == minDist
        // if so, return node with currNeighbor
    for(; tableIt != forwardingTable.end(); ++tableIt) {
      unsigned currNeighbor = tableIt->first;
      map<unsigned, double> currRow = tableIt->second;
      map<unsigned, double>::iterator rowIt = currRow.begin();
      for(; rowIt != currRow.end(); ++rowIt) {
        unsigned currDest = rowIt->first;
        double currDistance = rowIt->second;
        if (currDest == destId) {
          double neighborDist = table.GetEntry(returnNode->GetNumber(), destId);
          double totalDist = neighborDist + currDistance;
          if (totalDist == minDist) {
            returnNode->SetNumber(currNeighbor);
            return returnNode;
          }
        }
      }
    }

    // error, the destination node can't be found in the routing table
    cerr << *this << "couldn't GetNextHop, couldn''t find dest node in routing table" << endl;
    return new Node();

    
  }

  Table *Node::GetRoutingTable() const
  {
    return new Table(routingTable);
  }



  ostream & Node::Print(ostream &os) const
  {
    os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw;
    return os;
}
#endif
