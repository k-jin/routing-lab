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
  Table(int numNodes);
  map<int, map<int , std::tuple<double, int> > > lsTable;
  
  void updateRow(int rowNum);
  void updateEntry(int entryNum);
  map<int , std::tuple<double, int> >getRow(int rowNum);
  std::tuple<double,int> getEntry(int entryNum);
  
  
   
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
	unsigned numNodes;
	// index of outer vector would be indexed by neighbors and self
	// index of inner vectors would be indexed by every node in the system
	// initialize the table such that every vector<double> is length 0
	// initialize rows such that every distance is -1 to represent infinity
	std::vector< std::vector<double> > routingTable;
	
 public:
	Table(unsigned pN, unsigned nN, std::vector< std::vector<double> > rT);
	Table(unsigned pN, unsigned nN);
	Table();
	Table(const Table &rhs);
	Table & operator=(const Table &rhs);
	virtual ~Table();
	
	virtual bool RowMatches(const std::vector<double> compare, const unsigned nodeNum) const;
	
	ostream & Print(ostream &os) const;
	virtual void SetParentNode(unsigned nodeNum);
	virtual unsigned GetParentNode() const;
	virtual void SetNumNodes(unsigned number);
	virtual unsigned GetNumNodes() const;
	
	virtual void SetRow(unsigned neighborNum, std::vector<double> row);
	virtual std::vector<double> GetRow(unsigned neighborNum) const;
	virtual void SetEntry(unsigned neighborNum, unsigned destNum, double distance);
	virtual double GetEntry(unsigned neighborNum, unsigned destNum) const;
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
