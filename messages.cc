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
RoutingMessage::RoutingMessage(unsigned s, unsigned d, unsigned l):src(s),dst(d),latency(l)
{}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs):src(rhs.src),dst(rhs.dst),latency(rhs.latency)
{}
  void SetSrc(unsigned src) {src=s;}
  void SetDst(unsigned dst){dst=d;}
  void SetLatency(unsigned latency){latency=l;}
  
  unsigned GetSrc() const {return src;}
  unsigned GetDst() const {return dst;}
  unsigned GetLatency() const {return latency;}

#endif


#if defined(DISTANCEVECTOR)

ostream &RoutingMessage::Print(ostream &os) const
{
  os << "Routing body message=" << body << endl;
  return os;
}

RoutingMessage::RoutingMessage()
{}

RoutingMessage::RoutingMessage(map<unsigned, double> b) : body(b) {}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs)
{}

void RoutingMessage::SetBody(map<unsigned, double> b) { body = b; }
map<unsigned, double> RoutingMessage::GetBody() const { return body; }

#endif
