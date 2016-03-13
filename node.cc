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
	  cerr << *this << " Routing Message: "<<*m;
	  
	  
	  
	  RoutingMessage *rm = new RoutingMessage(m->GetSrc(),m->GetDst(),m->GetLatency());
	  SendToNeighbors(rm);
	  
	}

	void Node::TimeOut()
	{
	  cerr << *this << " got a timeout: ignored"<<endl;
	}

	Node *Node::GetNextHop(const Node *destination) const
	{
	  // WRITE
	  //return 0;
		Table table = *GetRoutingTable();
		map<unsigned, map<unsigned ,double > > linkTable = table.GetLinkTable();
		map<unsigned, map<unsigned , double > >::iterator tableIt = linkTable.begin();

		
		Node* returnNode = new Node(*this);

		
		unsigned destId = destination->GetNumber();

		
		unsigned srcId= this->GetNumber();
		map<unsigned , double > srcRow= linkTable->GetRow(this->getNumber());
		
		if(srcId==destId){
			return returnNode;
		}
		
		//this is the number of nodes in the graph at this point
		unsigned numNodes=linkTable.size();
		
		//FORGOT TO INITIALIZE THE MAPS FROM THE TABLE
		
		//initialize vectors with default values before we start djikstras
		 map<unsigned, bool> visited ; //<node num, bool visited>
		 map<unsigned, unsigned> pred; //<node num, node predecessor>
		 map<unsigned, double> dists; //<node number, distance from src>
		 
		 //setting the values for the startig node
		 visited[srcId]=true;
		 pred[srcId]=-100;
		 dists[srcId]=0;
		 
		 //start djikstras now
		 
		 //it->first is key, it->second is value
		
		//ok. so now we want to set the predecessors and the distance for all the nodes relative to the src
		 
		// map<unsigned, double> *srcRow = linkTable->GetRow(srcId);
		 //now we want to go through srcRow and set the costs relative to the src node
		 // then we do setRow so that the table is updated
		 
		 for(map<unsigned, double>::iterator itr= srcRow.begin(); itr!=srcRow.end(); ++itr){
			 //only do this if itr is a neigbour of src
			 deque<Node*> srcNeighbours = *this->Node::GetNeighbors();
			 
			 for(std::deque<Node*>::iterator dit=srcNeighbours.begin();dit!=srcNeighbours().end();++dit){
				 if(dit->getNumber()==itr->first)
				 {
					  dists[itr->first]=itr->second; //updating the distacne
					  pred[itr->first]=srcId;  //settig the pred of all the elts in the srcRow to be the src itself
				 }
			 }
			 
			
		 }
		 
		 
		 for(map<unsigned, double>::iterator itr= dists.begin(); itr!=dists.end(); ++itr){
			 //we want to go through all the nodes we have and then find the one with the lowest dist from src
			 
			 //we want to find the min of the priority queue
			 
			 unsigned minNumber;
			 double minDist= (double) INT_MAX;
			 
			 for(map<unsigned, double>::iterator it2= dists.begin(); it2!=dists.end(); ++it2){\
				if(minDist > it2->second && visited[it2->first]==false){
					minNumber=it2->first;
					minDist=it2->second;
					
				}
				
			 }
			 //we have found the node with the min dist from u. now lets update the distances of its neighbours
			 visited[minNumber]=true;
			 
			 //alt= dist[u] + length(u,v)
			 
			 //~ if(minDist<(double) INT_MAX){
				 //~ udist=minDist;
			 //~ }
			 
			 for(map<unsigned, double>::iterator it3=linkTable->GetRow(minNumber).begin(); it3!=linkTable->getRow(minNumber).end(); ++it3){
				 double altCost= it3->second + dists[minNumber];
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
		 
		 deque<Node*> srcNeighbours2 = this->Node::GetNeighbors();
		 //iterate through the neighbours to find the Node with the matching Node Number
		 //use Node.matches
		 for(std::deque <Node*>::iterator dit3=srcNeighbours2.begin();dit3!=srcNeighbours2().end();++dit3){
			 if(tempDestId==dit3->GetNumber()){
				return new Node(dit3); 
			 }
			 
		
			 
		 }
		 
		
		
		 return new Node();
		
		//finished alas!
		
		
	  
	}

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
