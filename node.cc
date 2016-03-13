#include "node.h"
#include "context.h"
#include "error.h"
#include "topology.h"


Node::Node(const unsigned n, SimulationContext *c, double b, double l) : 
    number(n), context(c), bw(b), lat(l) 
{
	routingTable.SetEntry(n,n,0);
	routingTable.SetParentNode(n);
}

Node::Node() 
{ throw GeneralException(); }

Node::Node(const Node &rhs) : 
  number(rhs.number), context(rhs.context), bw(rhs.bw), lat(rhs.lat), routingTable(rhs.routingTable) {}

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
  context->SendToNeighbors(this, m);
}

void Node::SendToNeighbor(const Node *n, const RoutingMessage *m)
{
  context->SendToNeighbor(this, n, m);
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

  // TODO: DONT SEND DV MESSAGE TO LINK THAT IS BEING UPDATED
  void Node::LinkHasBeenUpdated(const Link *l)
  {
    // update our table
    // send out routing mesages
    cerr << *this<<": Link Update: "<<*l<<endl;
    Table* table = GetRoutingTable();
    cerr << "Initial table: " << *table << endl;
    deque<Node*> neighbors = *this->GetNeighbors();
    deque<Node*>::iterator neighborsIt = neighbors.begin();
    Link newLink = *l;
    unsigned nodeId = this->GetNumber();
    unsigned destId = newLink.GetDest();
    double newLat = newLink.GetLatency();

    // update forwarding table entry 
    // table->SetEntry(*this->GetNumber(), destId, newLat);
    // For the updated link, we want to update the new shortest paths to each link within the distance vector
    // Getting own distance vector
    map<unsigned, double> nodeDv = table->GetRow(nodeId);
    map<unsigned, double> neighborDv = table->GetRow(destId);
    // Updating the distance for that neighbor node
    nodeDv[destId] = newLat;

    map<unsigned, double>::iterator entryIt = nodeDv.begin();
    for(; entryIt != nodeDv.end(); ++entryIt){
      unsigned currDest = entryIt->first;
      double minDist = entryIt->second;
//      cerr << "Current destination node: " << currDest << endl << "Min Distance: " << minDist << endl;
      // EDGE CASE: if neighborDv[currDest] does not exist
      // initialize neighborDv[currDest] to infinity

//    	cerr << "current neighborDv[currDest]" << neighborDv[currDest] << endl;
    	// TODO: update neighbor's DV with routing message with their own updated DV
      if(neighborDv.count(currDest) == 0) {
        neighborDv[currDest] = numeric_limits<double>::infinity();
//        cerr << "initialized neighborDv[currDest]" << neighborDv[currDest] << endl;
      }
//      cerr << "nodeDv[currDest] current minimum distance for dest node: " << nodeDv[currDest] << endl;
//      cerr << "current minDist (should be equal to above): " << minDist << endl;
//      cerr << "new distance: " << newLat+neighborDv[currDest] << endl;
      nodeDv[currDest] = min(minDist, newLat + neighborDv[currDest]);
//      cerr << "updated nodeDv[currDest]: " << nodeDv[currDest] << endl;
    }
    
//    table->SetRow(this->GetNumber(), nodeDv);
//    table->SetRow(destId, neighborDv);
    
    this->routingTable.SetRow(nodeId, nodeDv);
    this->routingTable.SetRow(destId, neighborDv);
    
    RoutingMessage* nodeRm = new RoutingMessage(nodeId, nodeId, nodeDv);
    RoutingMessage* neighborRm = new RoutingMessage(destId, destId, neighborDv);

    for(; neighborsIt != neighbors.end(); ++neighborsIt) {
      Node currNeighbor = **neighborsIt;
      if(currNeighbor.GetNumber() != destId) {
        RoutingMessage msg = RoutingMessage(*nodeRm);
        msg.SetDst(currNeighbor.GetNumber());
        SendToNeighbor(&currNeighbor, &msg);
      } 
      RoutingMessage neighMsg = RoutingMessage(*neighborRm);
      neighMsg.SetDst(currNeighbor.GetNumber());
      SendToNeighbor(&currNeighbor, &neighMsg);
      
    }
    cerr << "FOR NODE: " << nodeId << endl << *GetRoutingTable() <<endl;

  }


  void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
  {
    RoutingMessage msg = *m;
    if(this->GetNumber() == msg.GetDst()){
      // Copy of routing table
      Table* table = GetRoutingTable();
      //cerr << "PROCESSING INCOMING" << *table << endl;
      		
      // Get neighbors of current node and init iterator
      deque<Node*> neighbors = *this->GetNeighbors();
      deque<Node*>::iterator neighborsIt = neighbors.begin();

      unsigned srcId = msg.GetSrc();
      unsigned nodeId = this->GetNumber();
      // DV of node
      map<unsigned, double> nodeDv = table->GetRow(nodeId);
      // Newly received DV for node with srcId
      map<unsigned, double> newDv = msg.GetBody();
      map<unsigned, double>::iterator dvIt = newDv.begin();
      bool updated = false;

      // Update table with new DV for node srcId
//      table->SetRow(srcId, newDv);
      this->routingTable.SetRow(srcId, newDv);
      
      // Update own DV by checking if there are new minimums
      for(; dvIt != newDv.end(); ++dvIt) {
        unsigned currDestId = dvIt->first;
        double currDistance = dvIt->second;
        if(nodeDv.count(currDestId) == 0){
          nodeDv[currDestId] = currDistance;
          updated = true;
        } else {
          nodeDv[currDestId] = min(nodeDv[currDestId], nodeDv[srcId] + newDv[currDestId]);
          if (table->GetEntry(nodeId, currDestId) != nodeDv[currDestId]){
            updated = true;
          }
        }
      }
      
      if(updated){
//        table->SetRow(nodeId, nodeDv);
      	this->routingTable.SetRow(nodeId, nodeDv);
        RoutingMessage* rm = new RoutingMessage(nodeId, nodeId, nodeDv);
        for(; neighborsIt != neighbors.end(); ++neighborsIt) {
          Node currNeighbor = **neighborsIt;
          // Unsure if we should or should not send the new DV back to the src
          if(currNeighbor.GetNumber() != srcId) {
            RoutingMessage msg = RoutingMessage(*rm);
            msg.SetDst(currNeighbor.GetNumber());
            SendToNeighbor(&currNeighbor, &msg);
          }
        }
      }
      //cerr << "POST INCOME " << *GetRoutingTable() << endl;

      
    }
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

    // number of outgoing links
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
    //cerr << table << endl;
    //cerr << "Destination node: " << *destination << endl;
    //cerr << *GetRoutingTable() << endl;
    // error, the destination node can't be found in the routing table
    cerr << *this << " couldn't GetNextHop, couldn''t find dest node in routing table" << endl;
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
