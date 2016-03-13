#include "messages.h"


#if defined(GENERIC)
ostream &RoutingMessage::Print(ostream &os) const
{
  os << "RoutingMessage()";
  return os;
}
#endif


#if defined(LINKSTATE)

ostream &RoutingMessage::Print(ostream &os) const
{
  return os;
}

RoutingMessage::RoutingMessage()
{}


RoutingMessage::RoutingMessage(const RoutingMessage &rhs)
{}

#endif


#if defined(DISTANCEVECTOR)

ostream &RoutingMessage::Print(ostream &os) const
{
	os << "RoutingMsg SRC=" << src << endl << "RoutingMSG DST=" << dst << endl;
  os << "DV for " << src << endl;
  map<unsigned, double> body = GetBody();
  map<unsigned, double>::iterator bodyIt = body.begin();
  for(; bodyIt != body.end(); ++bodyIt){
  	unsigned currDest = bodyIt->first;
  	double currDist = bodyIt->second;
  	os << "Destination node=" << currDest << "/Distance=" << currDist << endl;
  }
  return os;
}	

RoutingMessage::RoutingMessage()
{}

RoutingMessage::RoutingMessage(unsigned s, unsigned d, map<unsigned, double> b) : src(s), dst(d), body(b) {}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs)
{}

void RoutingMessage::SetSrc(unsigned s) {src = s;}
unsigned RoutingMessage::GetSrc() const {return src; }
void RoutingMessage::SetDst(unsigned d) {dst = d;}
unsigned RoutingMessage::GetDst() const {return dst;}
void RoutingMessage::SetBody(map<unsigned, double> b) { body = b; }
map<unsigned, double> RoutingMessage::GetBody() const { return body; }

#endif