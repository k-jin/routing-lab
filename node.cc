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
    map<unsigned, map<unsigned, double> > table = GetRoutingTable()->GetForwardingTable();
    map<unsigned, map<unsigned, double> >::iterator tableIt = table.begin();
    Node returnNode = *this;
    unsigned destId = destination->GetNumber();
    deque<Link*> links = *GetOutgoingLinks();
    double minDist = table.GetEntry(number, destId);
    unsigned linksSize = links.size();

    for(int i = 0; i < linksSize; i++) {
      Link currLink = links[i];
      if(currLink.GetDest() == destId) {
        if(minDist == currLink.GetLatency()) {
          returnNode.SetNumber(destId);
          return returnNode;
        }
      }
    }

    for(; tableIt != table.end(); ++tableIt) {
      unsigned currNeighbor = tableIt->first;
      map<unsigned, double> currRow = tableIt->second;
      map<unsigned, double>::iterator rowIt = currRow.begin();
      for(; rowIt != currRow.end(); ++rowIt) {
        unsigned currDest = rowIt->first;
        double currDistance = rowIt->second;
        if (currDest == destId) {
          double neighborDist = table.GetEntry(returnNode.GetNumber(), destId);
          double totalDist = neighborDist + currDistance;
          if (totalDist == minDist) {
            returnNode.SetNumber(currDest);
            return returnNode;
          }
        }
      }
    }
    cerr << *this << "couldn't GetNextHop" << endl;
    return -1;

    
  }

  Table *Node::GetRoutingTable() const
  {
    return &routingTable;
  }



  ostream & Node::Print(ostream &os) const
  {
    os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw;
    return os;
}
#endif
