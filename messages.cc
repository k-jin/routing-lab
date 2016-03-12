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
  os << "Routing body message=" << body << endl;
  return os;
}

RoutingMessage::RoutingMessage()
{}

RoutingMessage::RoutingMessage(unsigned s, unsigned d, map<unsigned, double> b) : src(s), dst(d), body(b) {}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs)
{}

void RoutingMessage::SetSrc(unsigned s) {src = s;}
unsigned RoutingMessage::GetSrc() {return src; }
void RoutingMessage::SetDst(unsigned d) {dst = d;}
unsigned RoutingMessage::GetDst() {return dst;}
void RoutingMessage::SetBody(map<unsigned, double> b) { body = b; }
map<unsigned, double> RoutingMessage::GetBody() const { return body; }

#endif