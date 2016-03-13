#ifndef _messages
#define _messages

#include <iostream>
#include <map>

#include "node.h"
#include "link.h"

using namespace std;

#if defined(GENERIC)
struct RoutingMessage {
 public:
  ostream & Print(ostream &os) const;
};
#endif

#if defined(LINKSTATE)
struct RoutingMessage {

  unsigned src;
  unsigned dst;
  double latency;
  
  RoutingMessage();
  RoutingMessage(unsigned s, unsigned d, unsigned l);
  RoutingMessage(const RoutingMessage &rhs);
  RoutingMessage &operator=(const RoutingMessage &rhs);
  
  void SetSrc(unsigned s);
  void SetDst(unsigned d);
  void SetLatency(unsigned l);
  
  unsigned GetSrc() const;
  unsigned GetDst() const;
  unsigned GetLatency() const;
  
  

  ostream & Print(ostream &os) const;
};
#endif

#if defined(DISTANCEVECTOR)
struct RoutingMessage {

  map<unsigned, double> body;

  RoutingMessage();
  RoutingMessage(map<unsigned, double> b);
  RoutingMessage(const RoutingMessage &rhs);
  RoutingMessage &operator=(const RoutingMessage &rhs);

  void SetBody(map<unsigned, double> body);
  map<unsigned, double> GetBody() const;
  ostream & Print(ostream &os) const;
};
#endif


inline ostream & operator<<(ostream &os, const RoutingMessage &m) { return m.Print(os);}

#endif
