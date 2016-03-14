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
	  
	  //now we want to get the fields of the link
	  // save them into the linkTable
	  this->routingTable.setEntry(l->GetSrc(),l->GetDest(),l->GetLatency());
	  
	  
	  // send a routing message
	  RoutingMessage *rm = new RoutingMessage(l->GetSrc(),l->GetDest(),l->GetLatency());
	  
	  SendToNeighbors(rm);
	}


	void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
	{
	  cerr << *this << " Routing Message: "<<*m<<endl;
	  
	  this->routingTable.setEntry(m->GetSrc(),m->GetDst(),m->GetLatency());
	  set<unsigned> mySeenNodeIds= m->GetSeenNodeIds();
	  
	  //when we receive a routing message, then we check to see if this node has already recvd this rm before
	  // each rm has a set of seenNodeIds
	  //we check if this node's id is in that set
	  //if not in the set, then we send a new routing message to this nodes neighbours
	  //thus, a rm is not sent by a node, the second time it has received it
	  
	  if(mySeenNodeIds.count(this->GetNumber())==0){
		  RoutingMessage *rm = new RoutingMessage(m->GetSrc(),m->GetDst(),m->GetLatency());
		  mySeenNodeIds.insert(this->GetNumber()); //adding this nodeId to the set of seenNodeIds
		  rm->SetSeenNodeIds(mySeenNodeIds);
		  SendToNeighbors(rm);
	  }
	  
	  
	}

	void Node::TimeOut()
	{
	  //cerr << *this << " got a timeout: ignored"<<endl;
	}

	Node *Node::GetNextHop(const Node *destination) const
	{
	  // WRITE
	  
		Table table = *GetRoutingTable();
		map<unsigned, map<unsigned ,double > > linkTable = table.GetLinkTable();
		map<unsigned, map<unsigned , double > >::iterator tableIt = linkTable.begin();

		
		Node* returnNode = new Node(*this);

		//number of the destination node
		unsigned destId = destination->GetNumber();
		
		//number of source node
		unsigned srcId= this->GetNumber();
		//row in linkTable corresponding to this node. Row contains the neighbours of this node and the cost of the links to them
		map<unsigned , double > srcRow= table.getRow(srcId);
		
		
		if(srcId==destId){
			return returnNode;
		}
		
		//this is the number of nodes in the graph at this point
		unsigned numNodes=linkTable.size();
		
		unsigned tDist=0;
		
		//initialize vectors with default values before we start djikstras
		 map<unsigned, bool> visited ; //<node num, bool visited>
		 map<unsigned, unsigned> pred; //<node num, node predecessor>
		 map<unsigned, double> dists; //<node number, distance from src>
		 
		 //setting the initial values for all nodes
		 for(map<unsigned, map<unsigned ,double > >::iterator bigIt= linkTable.begin(); bigIt!=linkTable.end(); ++bigIt){
			visited[bigIt->first]=false;
			pred[bigIt->first]=-10;
			dists[bigIt->first]=-100;
			
		 }
		 
		 //setting the values for the starting src node
		 visited[srcId]=true;
		 pred[srcId]=-10;
		 dists[srcId]=0;
		 
		 //start djikstras now
		 
		 //it->first is key, it->second is value
		
		//so now we want to set the predecessors and the distance for all the nodes relative to the src
		 
		
		 //now we want to go through srcRow and set the costs relative to the src node
		 // then we do setRow so that the table is updated
		 
		 for(map<unsigned, double>::iterator itr= srcRow.begin(); itr!=srcRow.end(); ++itr){
			 
			  dists[itr->first]=itr->second; //updating the distacne
			  pred[itr->first]=srcId;  //settig the pred of all the elts in the srcRow to be the src itself
				
		 }
		 
		 
		 for(map<unsigned, double>::iterator itr= dists.begin(); itr!=dists.end(); ++itr){
			 //we want to go through all the nodes we have and then find the one with the lowest dist from src
			 
			 //we want to find the min of the priority queue (but we arent using a priority q, we are brute forcing"
			 
			 unsigned minNumber;
			 double minDist= (double) 9999; //arbitrary high value
			 
			 for(map<unsigned, double>::iterator it2= dists.begin(); it2!=dists.end(); ++it2){\
				if(minDist > it2->second && visited[it2->first]==false){
					minNumber=it2->first;
					minDist=it2->second;
					
				}
				
			 }
			 //we have found the node with the min dist from u. now lets update the distances of its neighbours
			 visited[minNumber]=true;
			 if(minDist!=(double) 9999){
					tDist=minDist;
			 }
			 
			 
			
			 map<unsigned , double > minNumberRow= table.getRow(minNumber);
			 for(map<unsigned, double>::iterator it3=minNumberRow.begin(); it3!=minNumberRow.end(); ++it3){
				 //double altCost= it3->second + dists[minNumber];
				 double altCost= it3->second + tDist;
				 if(dists[it3->first] > altCost && visited[it3->first]==false){
					 dists[it3->first]=altCost;
					 pred[it3->first]= minNumber;
				 }
				 
			 }
			 
			 
			 
			 
		 }
		 
		 //finished the djikstras part
		 
		 // now we want to return the next hop. lol finally
		 unsigned tempDestId=destId;
		 unsigned secondLast = pred[destId];
		 //we want to trace back to the src folowing the pred trail
		 while(secondLast!=srcId){
			 tempDestId=secondLast;
			 secondLast=pred[tempDestId];
		 }
		 //tempDestId is now the next hop. woohoo		 
		 deque<Node*> *srcNeighbours2 = context->GetNeighbors(this);
		 //~ //iterate through the neighbours to find the Node with the matching Node Number
		 //~ //use Node.matches
		 for(deque <Node*>::iterator dit3=srcNeighbours2->begin();dit3!=srcNeighbours2->end();++dit3){
			 Node temp2=**dit3;
			 if(tempDestId==temp2.GetNumber()){
				return new Node(temp2); 
			 }		 
		 }	
		 returnNode->SetNumber(tempDestId);
		 return returnNode;		
		

	Table *Node::GetRoutingTable() const
	{
	  // WRITE
	  return new Table(routingTable);
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
    Table* table = GetRoutingTable();
    map<unsigned, map<unsigned, double> > forwardingTable = table->GetForwardingTable();
    map<unsigned, map<unsigned, double> >::iterator tableIt = forwardingTable.begin();
    Link newLink = *l;
    unsigned destId = newLink.GetDest();
    double newLat = newLink.GetLatency();

    // update forwarding table entry 
    // table->SetEntry(*this->GetNumber(), destId, newLat);
    // For the updated link, we want to update the new shortest paths to each link within the distance vector
    // Getting own distance vector
    map<unsigned, double> currRow = table->GetRow(this->GetNumber());
    map<unsigned, double> neighborRow = table->GetRow(destId);
    // Updating the distance for that neighbor node
    currRow[destId] = newLat;

    map<unsigned, double>::iterator entryIt = currRow.begin();
    for(; entryIt != currRow.end(); ++entryIt){
      unsigned currDest = entryIt->first;
      double minDist = entryIt->second;
      // EDGE CASE: if neighborRow[currDest] does not exist
      // initialize neighborRow[currDest] to infinity
      if(neighborRow.count(currDest) == 0) {
        neighborRow[currDest] = numeric_limits<double>::infinity();
      }
      currRow[currDest] = min(minDist, newLat + neighborRow[currDest]);
    }

    



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
