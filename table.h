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
	// index of outer vector would be indexed by neighbors and self
	// index of inner vectors would be indexed by every node in the system
	std::vector<vector<double>> routingTable;
 public:
	ostream & Print(ostream &os) const;
	virtual void initTable(int size);
	virtual void SetRow(unsigned neighborNum, vector<double> row);
	virtual vector<double> GetRow(unsigned neighborNum) const;
	virtual void SetEntry(unsigned neighborNum, unsigned destNum, double distance);
	virtual double GetEntry(unsigned neighborNum, unsigned destNum);
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
