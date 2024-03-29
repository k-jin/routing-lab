#ifndef _messages
#define _messages

#include <iostream>
#include <map>
#include <vector>
#include <set>
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
  
  set<unsigned> seenNodeIds;
  
  RoutingMessage();
  RoutingMessage(unsigned s, unsigned d, unsigned l);
  RoutingMessage(const RoutingMessage &rhs);
  RoutingMessage &operator=(const RoutingMessage &rhs);
  
  void SetSeenNodeIds(set<unsigned> newSet);
  void SetSrc(unsigned s);
  void SetDst(unsigned d);
  void SetLatency(unsigned l);
  
  set<unsigned> GetSeenNodeIds() const;
  unsigned GetSrc() const;
  unsigned GetDst() const;
  unsigned GetLatency() const;
  
  

  ostream & Print(ostream &os) const;
};
#endif

#if defined(DISTANCEVECTOR)
struct RoutingMessage {
  unsigned src, dst, dv;
  map<unsigned, double> body;

  RoutingMessage();
  RoutingMessage(const unsigned s, const unsigned d, const unsigned dvr, const map<unsigned, double> b);
  RoutingMessage(const RoutingMessage &rhs);
  RoutingMessage &operator=(const RoutingMessage &rhs);

  virtual void SetSrc(unsigned nodeId);
  virtual unsigned GetSrc() const;
  virtual void SetDst(unsigned nodeId);
  virtual unsigned GetDst() const;
  virtual void SetDv(unsigned nodeId);
  virtual unsigned GetDv() const;
  virtual void SetBody(map<unsigned, double> body);
  virtual map<unsigned, double> GetBody() const;

  ostream & Print(ostream &os) const;
};
#endif


inline ostream & operator<<(ostream &os, const RoutingMessage &m) { return m.Print(os);}

#endif
