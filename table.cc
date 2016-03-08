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

 map<int , std::tuple<double, int> > Table::getRow(int rowNum){
	 return lsTable[rowNum];
 }
 
 std::tuple<double, int> Table::getEntry(int entryNum){
	 return lsTable[rowNum][entryNum];
 }
 
 
#endif

#if defined(DISTANCEVECTOR)
	
	Table::Table(unsigned pN, map<unsigned, map<unsigned, double> > fT) : 
		parentNode(pN), forwardingTable(fT) {}
		
	Table::Table(unsigned pN) : 
		parentNode(pN) {}
	Table::Table()
	{
		parentNode = -1;
	}
	
	Table::Table(const Table &rhs) :
		parentNode(rhs.parentNode), forwardingTable(rhs.forwardingTable) {}
		
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
		map<unsigned, map<unsigned, double> > table = GetForwardingTable();
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
	unsigned Table::GetParentNode() const { return parentNode; }
	void Table::SetForwardingTable(map<unsigned, map<unsigned, double> > table) {
		forwardingTable = table;
	}
	map<unsigned, map<unsigned, double> > Table::GetForwardingTable() const { return forwardingTable; }
	void Table::SetRow(unsigned neighborId, map<unsigned, double> row) 
	{
		forwardingTable[neighborId] = row;
	}
	map<unsigned, double> Table::GetRow(unsigned neighborId) const { return GetForwardingTable()[neighborId]; }
	void Table::SetEntry(unsigned neighborId, unsigned destId, double distance)
	{
		forwardingTable[neighborId][destId] = distance;
	}
	double Table::GetEntry(unsigned neighborId, unsigned destId) const
	{
		return GetForwardingTable()[neighborId][destId];
	}
		
#endif
