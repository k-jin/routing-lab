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
  os << "RM: S="<<src<<" D="<<dst<<" L="<<latency<<" "<<endl;	
  return os;
}

RoutingMessage::RoutingMessage()
{}
RoutingMessage::RoutingMessage(unsigned s, unsigned d, unsigned l):src(s),dst(d),latency(l)
{}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs):src(rhs.src),dst(rhs.dst),latency(rhs.latency)
{}

  void RoutingMessage::SetSeenNodeIds(set<unsigned> newIdSet){
	  seenNodeIds=newIdSet;
  }
  void RoutingMessage::SetSrc(unsigned s) {src=s;}
  void RoutingMessage::SetDst(unsigned d){dst=d;}
  void RoutingMessage::SetLatency(unsigned l){latency=l;}
  
  set<unsigned> RoutingMessage::GetSeenNodeIds() const{
	  return seenNodeIds;
  }
  unsigned RoutingMessage::GetSrc() const {return src;}
  unsigned RoutingMessage::GetDst() const {return dst;}
  unsigned RoutingMessage::GetLatency() const {return latency;}

#endif


#if defined(DISTANCEVECTOR)

ostream &RoutingMessage::Print(ostream &os) const
{
	os << "RoutingMsg SRC=" << src << endl << "RoutingMSG DST=" << dst << endl;
  os << "DV for " << dv << endl;
  map<unsigned, double> body = GetBody();
  map<unsigned, double>::iterator bodyIt = body.begin();
  for(; bodyIt != body.end(); ++bodyIt){
  	unsigned currDest = bodyIt->first;
  	double currDist = bodyIt->second;
  	os << "dest=" << currDest << "/dist=" << currDist << "\t";
  }
  return os;
}	

RoutingMessage::RoutingMessage()
{}

RoutingMessage::RoutingMessage(const unsigned s, const unsigned d, const unsigned dvr, const map<unsigned, double> b) : src(s), dst(d), dv(dvr), body(b) {}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs) //: src(rhs.src), dst(rhs.dst), dv(rhs.dv), body(rhs.body)
{}

void RoutingMessage::SetSrc(unsigned s) {src = s;}
unsigned RoutingMessage::GetSrc() const {return src; }
void RoutingMessage::SetDst(unsigned d) {dst = d;}
unsigned RoutingMessage::GetDst() const {return dst;}
void RoutingMessage::SetDv(unsigned d) {dv = d;}
unsigned RoutingMessage::GetDv() const {return dv; }
void RoutingMessage::SetBody(map<unsigned, double> b) { body = b; }
map<unsigned, double> RoutingMessage::GetBody() const { return body; }


#endif
