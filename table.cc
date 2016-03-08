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
	
	Table::Table(unsigned pN, unsigned nN, std::vector< std::vector<double> > rT) : 
		parentNode(pN), numNodes(nN), routingTable(rT) {}
		
	Table::Table(unsigned pN, unsigned nN) : 
		parentNode(pN), numNodes(nN) 
		{
			std::vector<double> empty (0,0.0);
			for (unsigned i = 0; i < numNodes; i++){
				routingTable[i] = empty;
			}
		}
	
	Table::Table()
	{
		numNodes = 0;
		parentNode = -1;
		std::vector<double> empty (0,0.0);
		for (unsigned i = 0; i < numNodes; i++){
			routingTable[i] = empty;
		}
	}
	
	Table::Table(const Table &rhs) :
		parentNode(rhs.parentNode), numNodes(rhs.numNodes), routingTable(rhs.routingTable) {}
		
	Table & Table::operator=(const Table &rhs)
	{
		return *(new (this) Table(rhs));
	}
	
	Table::~Table()
	{}
	
	
	bool Table::RowMatches(const std::vector<double> compare, const unsigned nodeNum) const
	{
		if (compare.size() != numNodes) {
			return false;
		}
		std::vector<double> tableRow = routingTable[nodeNum];
		
		for (unsigned i = 0; i < numNodes; i++) {
			if (compare[i] != tableRow[i]) { return false; }
		}
		return true;
	}
	
	ostream & Table::Print(ostream &os) const
	{
		os << "Routing Table for node " << parentNode << endl;
		unsigned rtSize = routingTable.size();
		
		for(unsigned i = 0; i < rtSize; i++) {
			std::vector<double> currRow = routingTable[i];
			unsigned rowSize = currRow.size();
			if (rowSize > 0) {
				os << "Neighbor node " << i << "\t";
				for(unsigned j = 0; j < rowSize; j++){
					os << "Destination node: " << j << "/Distance: " << currRow[j] << "\t"; 
				}
				os << endl;
			}
		}
		return os;
		
	}
	
	void Table::SetParentNode(unsigned nodeNum) { parentNode = nodeNum; }
	unsigned Table::GetParentNode() const {return parentNode;}
	void Table::SetNumNodes(unsigned number) { numNodes = number; }
	unsigned Table::GetNumNodes() const { return numNodes; }
	void Table::SetRow(unsigned neighborNum, std::vector<double> row) 
	{
		routingTable[neighborNum] = row;
	}
	std::vector<double> Table::GetRow(unsigned neighborNum) const { return routingTable[neighborNum]; }
	void Table::SetEntry(unsigned neighborNum, unsigned destNum, double distance)
	{
		if (routingTable[neighborNum].size() == 0) {
			std::vector<double> newRow (numNodes, -1.0); 
			routingTable[neighborNum] = newRow; 
		}
		routingTable[neighborNum][destNum] = distance;
	}
	double Table::GetEntry(unsigned neighborNum, unsigned destNum) const
	{
		if (routingTable[neighborNum].size() == 0) {
			return -1.0;
		}
		return routingTable[neighborNum][destNum];
	}
		
#endif
