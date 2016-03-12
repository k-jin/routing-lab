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

  RoutingMessage();
  RoutingMessage(const RoutingMessage &rhs);
  RoutingMessage &operator=(const RoutingMessage &rhs);

  ostream & Print(ostream &os) const;
};
#endif

#if defined(DISTANCEVECTOR)
struct RoutingMessage {
  unsigned src, dst;
  map<unsigned, double> body;

  RoutingMessage();
  RoutingMessage(unsigned s, unsigned d, map<unsigned, double> b);
  RoutingMessage(const RoutingMessage &rhs);
  RoutingMessage &operator=(const RoutingMessage &rhs);

  virtual void SetSrc(unsigned nodeId);
  virtual unsigned GetSrc() const;
  virtual void SetDst(unsigned nodeId);
  virtual unsigned GetDst() const;
  virtual void SetBody(map<unsigned, double> body);
  virtual map<unsigned, double> GetBody() const;

  ostream & Print(ostream &os) const;
};
#endif


inline ostream & operator<<(ostream &os, const RoutingMessage &m) { return m.Print(os);}

#endif
