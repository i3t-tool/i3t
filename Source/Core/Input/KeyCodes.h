#pragma once

/** Status of the keyboard keys and also mouse keys. */
class Keys final
{
public:
  /** Codes representing keys on the keyboard and mouse (letters, numbers, functional keys, arrows, ... )
    \todo Add handling of Ctrl-C, Ctrl-V, ...
    */
  enum Code
  {
    a = 0,
    b,
    c,
    d,
    e,
    f,
    g,
    h,
    i,
    j,
    k,
    l,
    m,
    n,
    o,
    p,
    q,
    r,
    s,
    t,
    u,
    v,
    w,
    x,
    y,
    z,
    n1,
    n2,
    n3,
    n4,
    n5,
    n6,
    n7,
    n8,
    n9,
    n0,
    altr,
    altl,
    ctrll,
    ctrlr,
    shiftl,
    shiftr,
    left,
    right,
    up,
    down,
    f1,
    f2,
    f3,
    f4,
    f5,
    f6,
    f7,
    f8,
    f9,
    f10,
    f11,
    f12,
    home,
    end,
    insert,
    del,
    pageUp,
    pageDown,
    esc,
    mouseLeft,
    mouseRight,
    mouseMiddle,
    mouseScrlUp,
    mouseScrlDown
  };

  static const char* getKeyString(int enumVal) { return keyStrings[enumVal]; }

  static const char* keyStrings[];
};
//===----------------------------------------------------------------------===//
