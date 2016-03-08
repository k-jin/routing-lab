#ifndef _table
#define _table


#include <iostream>

using namespace std;

#if defined(GENERIC)
class Table {
  // Students should write this class

 public:
  ostream & Print(ostream &os) const;
};
#endif


#if defined(LINKSTATE)
class Table {
  // Students should write this class
 public:
  ostream & Print(ostream &os) const;
};
#endif

#if defined(DISTANCEVECTOR)

#include <deque>
#include <vector>

class Table {
	// parent node of routing table
	unsigned parentNode;
	// total number of nodes in the system
	// also length of each internal vector<double>
	//unsigned numNodes;
	// TODO: probably don't need numNeighbors
	// number of neighbors 
	unsigned numNeighbors;
	// index of outer vector is row index, where index is the node id
	// deque is the row itself
	// tuple<int, double> is the current shortest distance (double) to the node id (int)
	std::map<unsigned, std::map<unsigned, double> > routingTable;
	// std::vector< std::deque< std::tuple< int, double > > > routingTable;
	
 public:
	Table();
	Table(unsigned pN);
	Table(unsigned pN, unsigned nN);
	Table(unsigned pN, std::map<unsigned, std::map<unsigned, double> > rT);
	Table(unsigned pN, unsigned nN,	std::map<unsigned, std::map<unsigned, double> > rT);
	Table(const Table &rhs);
	Table & operator=(const Table &rhs);
	virtual ~Table();
	
	// virtual bool RowMatches(const std::deque< std::tuple< int, double > > compare, const unsigned nodeId) const;
	
	ostream & Print(ostream &os) const;
	virtual void SetParentNode(unsigned nodeId);
	virtual unsigned GetParentNode() const;
	virtual void SetNumNeighbors(unsigned number);
	virtual unsigned GetNumNeighbors() const;
	virtual void SetRow(unsigned neighborId, std::map<unsigned, double> row);
	virtual std::map<unsigned, double> GetRow(unsigned neighborId) const;
	virtual void SetEntry(unsigned neighborId, unsigned destId, double distance);
	virtual double GetEntry(unsigned neighborId, unsigned destId) const;
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
