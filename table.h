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
#include <map>


class Table {
	// parent node of routing table
	unsigned parentNode;
	// index of outer map is row index, where index is the node id (unsigned)
	// index of inner map is the destination node (unsigned) and the value is the distance (double)
	map<unsigned, map<unsigned, double> > routingTable;
	
 public:
	Table();
	Table(unsigned pN);
	Table(unsigned pN, map<unsigned, map<unsigned, double> > rT);
	Table(const Table &rhs);
	Table & operator=(const Table &rhs);
	virtual ~Table();
	
	// virtual bool RowMatches(const std::deque< std::tuple< int, double > > compare, const unsigned nodeId) const;
	
	ostream & Print(ostream &os) const;
	virtual void SetParentNode(unsigned nodeId);
	virtual unsigned GetParentNode() const;
	virtual void SetRoutingTable(map<unsigned, map<unsigned, double> > table);
	virtual map<unsigned, map<unsigned, double> > GetRoutingTable() const;
	virtual void SetRow(unsigned neighborId, map<unsigned, double> row);
	virtual map<unsigned, double> GetRow(unsigned neighborId) const;
	virtual void SetEntry(unsigned neighborId, unsigned destId, double distance);
	virtual double GetEntry(unsigned neighborId, unsigned destId) const;
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
