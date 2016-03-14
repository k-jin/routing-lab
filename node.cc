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
//	cerr << "before send rm " << *m << endl;
  context->SendToNeighbor(this, n, m);
  //cerr << "sent " << m->GetSrc() << endl;
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
    //cerr << *this<<": Link Update: "<<*l<<endl;
    Table* table = GetRoutingTable();
//    cerr << "BEFORE" << *table << endl;
    map<unsigned, map<unsigned, double> > forwardingTable = table->GetForwardingTable();
    deque<Node*> neighbors = *this->GetNeighbors();
    deque<Node*>::iterator neighborsIt = neighbors.begin();
    unsigned nodeId = this->GetNumber();
    unsigned destId = l->GetDest();
    double newLat = l->GetLatency();
//    cerr << "NODE=" << nodeId << endl;
    // For the updated link, we want to update the new shortest paths to each link within the distance vector
    // Getting own distance vector
    map<unsigned, double> nodeDv = table->GetRow(nodeId);
    map<unsigned, double> neighborDv = table->GetRow(destId);
    // Updating the distance for that neighbor node
//    if(nodeDv.count(destId) == 0) { nodeDv[destId] = newLat; }
    nodeDv[destId] = newLat;
    //cerr << "nodeDv[destId] " << nodeDv[destId] <<endl;
    double minDist2Dest = nodeDv[destId];
    map<unsigned, map<unsigned, double> >::iterator it = forwardingTable.begin();
    for(; it != forwardingTable.end(); ++it) {
    	unsigned neighborId = it->first;
    	if(neighborId != nodeId) {
				map<unsigned, double> currDv = it->second;
				if(nodeDv.count(neighborId) == 0) { nodeDv[neighborId] = numeric_limits<double>::infinity(); }
				if(currDv.count(destId) == 0) { currDv[destId] = numeric_limits<double>::infinity(); }
				minDist2Dest = min(minDist2Dest, nodeDv[neighborId] + currDv[destId]);
    	}
    }
    nodeDv[destId] = minDist2Dest;
    
    map<unsigned, double>::iterator entryIt = nodeDv.begin();
    for(; entryIt != nodeDv.end(); ++entryIt){
      unsigned currDest = entryIt->first;
      double minDist = entryIt->second;
      // EDGE CASE: if neighborDv[currDest] does not exist
      // initialize neighborDv[currDest] to infinity
      if(neighborDv.count(currDest) == 0) {neighborDv[currDest] = numeric_limits<double>::infinity();}
      
      double minDist2Dest1 = minDist;
      it = forwardingTable.begin();
      for(; it != forwardingTable.end(); ++it) {
      	unsigned neighborId = it->first;
      	if(neighborId != nodeId) {
  				map<unsigned, double> currDv = it->second;
  				if(nodeDv.count(neighborId) == 0) { nodeDv[neighborId] = numeric_limits<double>::infinity(); }
  				if(currDv.count(destId) == 0) { currDv[destId] = numeric_limits<double>::infinity(); }
  				minDist2Dest1 = min(minDist2Dest1, nodeDv[neighborId] + currDv[destId]);
      	}
      }
      nodeDv[currDest] = minDist2Dest1;
      
    }
//    cerr << "nodeDv=";
//    PrintRow(nodeDv);
//    cerr << "neighborDv=";
//    PrintRow(neighborDv);
    	this->routingTable.SetRow(nodeId, nodeDv);
    	this->routingTable.SetRow(destId, neighborDv);
    
			RoutingMessage* msg = new RoutingMessage();
			msg->SetSrc(nodeId);
			msg->SetDv(nodeId);
			//msg->SetDst(currNeighbor.GetNumber());
			msg->SetBody(nodeDv);
//			cerr << "Sending own DV: " << *msg << endl;
			SendToNeighbors(msg);

    
    //if(this->GetNumber() == 1) {
    	//cerr << "FOR NODE: " << nodeId << endl << *GetRoutingTable() <<endl;	
//    }
  }


  void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
  {
  	bool printErr = false;
  	//if(this->GetNumber() == 0) {printErr = true;}

      // Copy of routing table
      Table* table = GetRoutingTable();
      if (printErr){
      	cerr << "Table before: " << *table << endl;
      	cerr << "INCOMING MSG: " << *m << endl;
      }
      		
      // Get neighbors of current node and init iterator
      deque<Node*> neighbors = *this->GetNeighbors();
      deque<Node*>::iterator neighborsIt = neighbors.begin();

      // mId is the DV of the node that is in the incoming message
      unsigned mId = m->GetDv();
      unsigned nodeId = this->GetNumber();
      // DV of node
      map<unsigned, double> nodeDv = table->GetRow(nodeId);
      // Current DV of node dvId in current node
      map<unsigned, double> nodeCurrDv = table->GetRow(mId);
      // Newly received DV for node with srcId
      map<unsigned, double> newDv = m->GetBody();
      map<unsigned, double>::iterator newDvIt = newDv.begin();
      
      bool updated = false;

      // Update table with new DV for node srcId
      //this->routingTable.SetRow(mId, newDv);
      for(; newDvIt != newDv.end(); ++newDvIt) {
      	unsigned id = newDvIt->first;
      	double dist = newDvIt->second;
      	this->routingTable.SetEntry(mId, id, dist);
      }
      
      newDvIt = newDv.begin();
      // Update own DV by checking if there are new minimums
      for(; newDvIt != newDv.end(); ++newDvIt) {
        unsigned newDestId = newDvIt->first;
        double newDistance = newDvIt->second;
//        if(nodeDv.count(currDestId) == 0){nodeDv[currDestId] == numeric_limits<double>::infinity(); }
        
        if(nodeDv.count(newDestId) == 0){
        	nodeDv[newDestId] = numeric_limits<double>::infinity();
          updated = true;
        } //else{
      	if(printErr) { cerr << "ORIGINAL NODEDV[newDestId] " << nodeDv[newDestId]<< endl;}

        if(table->GetEntry(nodeId, mId) + table->GetEntry(mId, newDestId) == nodeDv[newDestId]) {
        	
        	double checkDistance = nodeDv[newDestId];
        	unsigned shortestCounter = 0;
        	map<unsigned, double> origNodeDv = table->GetRow(nodeId);
        	map<unsigned, map<unsigned, double> >::iterator rowIt= table->GetForwardingTable().begin();
        	for(; rowIt != table->GetForwardingTable().end(); ++rowIt) {
        		unsigned currneighbor = rowIt->first;
        		if(currneighbor != nodeId){
        			map<unsigned, double> currdv = rowIt->second;
          		if(currdv[newDestId] + origNodeDv[currneighbor] == checkDistance) {shortestCounter++;}

        		}
        	}
        	if(printErr) {cerr << shortestCounter << endl;}
        	if(shortestCounter < 1) {
        		if(printErr) {cerr << "route 1" << endl;}
        		nodeDv[newDestId] = nodeDv[mId] + newDistance;
        	} else {
        		if(printErr) {cerr << "route 2" << endl;}
            nodeDv[newDestId] = min(nodeDv[newDestId], nodeDv[mId] + newDistance);
        	}
        }else {
      		if(printErr) {cerr << "route 3" << endl;}
          nodeDv[newDestId] = min(nodeDv[newDestId], nodeDv[mId] + newDistance);
      	}
        if(printErr) { cerr << "NEW NODEDV[newDestId]" << nodeDv[newDestId] << endl;}
        if (table->GetEntry(nodeId, newDestId) != nodeDv[newDestId]){
        	if(printErr) { cerr << "TRUEEE" << endl;}
        	updated = true;
        }
//      }
        
      }
      if(printErr) {PrintRow(nodeDv);}
      
      if(updated){
      	this->routingTable.SetRow(nodeId, nodeDv);
        RoutingMessage* rm = new RoutingMessage(nodeId, nodeId, nodeId, nodeDv);
					RoutingMessage* msg = new RoutingMessage();
					msg->SetSrc(nodeId);
					msg->SetDv(nodeId);
//            msg->SetDst(currNeighbor.GetNumber());
					msg->SetBody(nodeDv);
//            cerr << "Sending updated dv: " << *msg << endl;
					SendToNeighbors(msg);
      }
      if(printErr){
      	cerr << "After updated msg: " << *GetRoutingTable() << endl;	
      }
//    }
  }

  void Node::TimeOut()
  {
    cerr << *this << " got a timeout: ignored"<<endl;
  }


  Node *Node::GetNextHop(const Node *destination) const
  {
//  	cerr << "here?" << endl;
    Table table = *GetRoutingTable();
//    cerr << "NODE: " << this->GetNumber() << endl;
    cerr << table << endl;
    map<unsigned, map<unsigned, double> > forwardingTable = table.GetForwardingTable();
    map<unsigned, map<unsigned, double> >::iterator tableIt = forwardingTable.begin();

    // initialized node to be returned, will be returned with next node in path
    Node* returnNode = new Node(*this);

    // destination id of final node
    unsigned destId = destination->GetNumber();
    
//    cerr << "DEST: " << destId << endl;

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
//          cerr << "Neighbor " << *returnNode << endl;
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
//          	cerr << "Not neighbor: " << *returnNode << endl;
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

  void Node::PrintRow(map<unsigned, double> row) {
  	for(map<unsigned, double>::iterator it = row.begin(); it != row.end(); ++it) {
  		unsigned id = it->first;
  		double dist = it->second;
  		cerr << "\t node=" << id << ",dist=" << dist << endl;
  	}
  }

  void Node::PrintTable(map<unsigned, map<unsigned, double> > table) {
  	for(map<unsigned, map<unsigned, double> >::iterator it = table.begin(); it != table.end(); ++it){
  		unsigned id = it->first;
  		map<unsigned, double> row = it->second;
  		cerr << "DV of node=" << id << endl;
  		PrintRow(row);
  	}
  }

  ostream & Node::Print(ostream &os) const
  {
    os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw;
    return os;
}
#endif
