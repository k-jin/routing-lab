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

#if defined(DISTANCEVECTOR)
	//TODO: Initiliazed all rows in a table to be empty vector<double>
	Table::Table(unsigned pN, unsigned nN, std::vector<std::vector<double>> rT) : 
		parentNode(pN), numNodes(nN), routingTable(rT) {}
		
	Table::Table(unsigned pN, unsigned nN) : 
		parentNode(pN), numNodes(nN) {}
	
	Table::Table()
	{}
	
	Table::Table(const Table &rhs) :
		parentNode(rhs.parentNode), numNodes(rhs.numNodes), routingTable(rhs.routingTable) {}
		
	Table & Table::operator=(const Table &rhs)
	{
		return *(new (this) Table(rhs));
	}
	
	Table::~Table()
	{}
	
	
	virtual bool RowMatches(const std::vector<double> compare, const unsigned nodeNum) const
	{
		if (compare.size() != numNodes) {
			return false;
		}
		std::vector<double> tableRow = routingTable[nodeNum];
		
		for (int i = 0; i < numNodes; i++) {
			if (compare[i] != tableRow[i]) { return false; }
		}
		return true;
	}
	
	ostream & Print(ostream &os) const
	{
		os << "Routing Table for node " << parentNode << endl;
		unsigned rtSize = routingTable.size();
		
		for(int i = 0; i < rtSize; i++) {
			std::vector<double> currRow = routingTable[i];
			unsigned rowSize = currRow.size();
			if (rowSize > 0) {
				os << "Neighbor node " << i << "\t";
				for(int j = 0; j < rowSize; j++){
					os << "Destination node: " << j << "/Distance: " << currRow[j] << "\t"; 
				}
				os << endl;
			}
		}
		return os;
		
	}
	
	virtual void SetParentNode(unsigned nodeNum) { parentNode = nodeNum; }
	virtual unsigned GetParentNode() const {return parentNode;}
	virtual void SetNumNodes(unsigned number) { numNodes = number; }
	virtual unsigned GetNumNodes() const { return numNodes; }
	virtual void SetRow(unsigned neighborNum, std::vector<double> row) 
	{
		routingTable[neighborNum] = row;
	}
	virtual std::vector<double> GetRow(unsigned neighborNum) const { return routingTable[neighborNum]; }
	virtual void SetEntry(unsigned neighborNum, unsigned destNum, double distance)
	{
		routingTable[neighborNum][destNum] = distance;
	}
	virtual double GetEntry(unsigned neighborNum, unsigned destNum) const
	{
		return routingTable[neighborNum][destNum];
	}
		
#endif
