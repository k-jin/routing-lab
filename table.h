#ifndef _table
#define _table


#include <iostream>
#include <map>

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
  // each node needs a list to measure distances and predecessorss from other nodes
  //so there will be NxN table. size is static
  // thable updated at every step
  //entry tuple is < double cost, int predecessor>
 public:
 
  Table();
  Table(unsigned numNodes);
  Table(const Table &rhs);
  Table & operator=(const Table &rhs);
  virtual ~Table();
	
  map<unsigned, map<unsigned , std::tuple<double, unsigned> > > lsTable;
  
  void updateRow(unsigned rowNum);
  void updateEntry(unsigned entryNum);
  map<unsigned , std::tuple<double, unsigned> >getRow(unsigned rowNum);
  std::tuple<double,unsigned> getEntry(unsigned entryNum);
 
  ostream & Print(ostream &os) const;
};
#endif

#if defined(DISTANCEVECTOR)

#include <deque>
#include <map>


class Table {
	// parent node of forwarding table
	unsigned parentNode;
	// index of outer map is row index, where index is the node id (unsigned)
	// index of inner map is the destination node (unsigned) and the value is the distance (double)
	map<unsigned, map<unsigned, double> > forwardingTable;
	
 public:
	Table();
	Table(unsigned pN);
	Table(unsigned pN, map<unsigned, map<unsigned, double> > fT);
	Table(const Table &rhs);
	Table & operator=(const Table &rhs);
	virtual ~Table();
	
	// virtual bool RowMatches(const std::deque< std::tuple< int, double > > compare, const unsigned nodeId) const;
	
	ostream & Print(ostream &os) const;
	virtual void SetParentNode(unsigned nodeId);
	virtual unsigned GetParentNode() const;

	virtual void SetForwardingTable(map<unsigned, map<unsigned, double> > table);
	virtual map<unsigned, map<unsigned, double> > GetForwardingTable() const;
	virtual void SetRow(unsigned neighborId, map<unsigned, double> row);
	virtual map<unsigned, double> GetRow(unsigned neighborId) const;
	virtual void SetEntry(unsigned neighborId, unsigned destId, double distance);
	virtual double GetEntry(unsigned neighborId, unsigned destId) const;
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
