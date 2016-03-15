#ifndef _table
#define _table


#include <iostream>
#include <map>
//#include <tuple>

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
  
  map<unsigned ,map<unsigned , double> > linkTable;
 public:
  
  Table();
  
  Table(const Table &rhs);
  Table(map<unsigned, map<unsigned, double> > lT);
  Table & operator=(const Table &rhs);
  virtual ~Table();
	
  
  
  void setRow(unsigned rowNum, map<unsigned , double > newRow );
  void setEntry(unsigned rowNum, unsigned entryNum, double newEntry);
  
  map<unsigned , double> getRow(unsigned rowNum);
  double getEntry(unsigned rowNum, unsigned entryNum);
  
  
  virtual map<unsigned, map<unsigned, double> > GetLinkTable() const;
 
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
