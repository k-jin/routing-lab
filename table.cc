#include "table.h"

#if defined(GENERIC)
ostream & Table::Print(ostream &os) const
{
  // WRITE THIS
  os << "Table()";
  return os;
}
#endif

#if defined(LINKSTATE)

#endif

//TODO: probably don't need numNeighbors
#if defined(DISTANCEVECTOR)
	
	Table::Table(unsigned pN, unsigned nN, map<unsigned, map<unsigned, double> > rT) : 
		parentNode(pN), numNeighbors(nN), routingTable(rT) {}
		
	Table::Table(unsigned pN) : 
		parentNode(pN), numNeighbors(0)
		{}

	Table::Table(unsigned pN, unsigned nN) : 
		parentNode(pN), numNeighbors(nN)
		{}

	Table::Table(unsigned pN, map<unsigned, map<unsigned, double> > rT) : 
		parentNode(pN), numNeighbors(rT.size()), routingTable(rT) {}
	
	Table::Table()
	{
		parentNode = -1;
	}
	
	Table::Table(const Table &rhs) :
		parentNode(rhs.parentNode), numNeighbors(rhs.numNeighbors), routingTable(rhs.routingTable) {}
		
	Table & Table::operator=(const Table &rhs)
	{
		return *(new (this) Table(rhs));
	}
	
	Table::~Table()
	{}
	
	
	// bool Table::RowMatches(const std::deque< std::tuple< int, double > > compare, const unsigned nodeId) const
	// {
	// 	std::vector<double> tableRow = routingTable[nodeId];
	// 	if (compare.size() != tableRow.size()) {
	// 		return false;
	// 	}
		
	// 	for (unsigned i = 0; i < numNodes; i++) {
	// 		if (compare[i] != tableRow[i]) { return false; }
	// 	}
	// 	return true;
	// }
	
	ostream & Table::Print(ostream &os) const
	{
		os << "Routing Table for node " << parentNode << endl;
		map<unsigned, map<unsigned, double> > table = GetRoutingTable();
		map<unsigned, map<unsigned, double> >::iterator rowIt = table.begin();
		for(; rowIt != table.end(); ++rowIt){
			os << "Neighbor node: " << rowIt->first << "\t";
			map<unsigned, double> currRow = rowIt->second;
			map<unsigned, double>::iterator entryIt = currRow.begin();
			for(; entryIt != currRow.end(); ++entryIt){
				unsigned currDest = entryIt->first;
				double currDist = entryIt->second;
				os << "Destination node: " << currDest << "/Distance: " << currDist << "\t";
			}
			os << endl;
		}
		
		
		return os;
		
	}
	
	void Table::SetParentNode(unsigned nodeId) { parentNode = nodeId; }
	unsigned Table::GetParentNode() const {return parentNode;}
	void Table::SetNumNeighbors(unsigned number) { numNeighbors = number; }
	unsigned Table::GetNumNeighbors() const {return numNeighbors;}
	void Table::SetRoutingTable(map<unsigned, map<unsigned, double> > table) {
		routingTable = table;
	}
	map<unsigned, map<unsigned, double> > Table::GetRoutingTable() const { return routingTable; }
	void Table::SetRow(unsigned neighborId, map<unsigned, double> row) 
	{
		routingTable[neighborId] = row;
	}
	map<unsigned, double> Table::GetRow(unsigned neighborId) const { return GetRoutingTable()[neighborId]; }
	void Table::SetEntry(unsigned neighborId, unsigned destId, double distance)
	{
		routingTable[neighborId][destId] = distance;
	}
	double Table::GetEntry(unsigned neighborId, unsigned destId) const
	{
		return GetRoutingTable()[neighborId][destId];
	}
		
#endif
