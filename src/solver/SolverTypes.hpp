#pragma once

#include <vector>

namespace asatar
{
    typedef int Var;

    struct Lit
    {
        int x;

        Lit() : x(-1) {}
        Lit(Var var, bool sign = false) : x(var)
        {
            x = (x << 1) | (int)sign;
        }

        bool operator == (Lit p) const { return x == p.x; }
        bool operator != (Lit p) const { return x != p.x; }
        bool operator <  (Lit p) const { return x < p.x; }
    
        inline bool eval(bool b)    { return (x & 1) ^ (int)b; }
    };

    inline Lit operator ~(Lit p)    { Lit q; q.x = p.x ^ 1; return q; }
    inline Var var(Lit p)           { return p.x >> 1; }
    inline bool sign(Lit p)         { return p.x & 1; }
    inline int toInt(Lit p)         { return p.x; }

    inline Lit mkLit(Var var, bool sign = false) { Lit q(var, sign); return q; }
    
    typedef std::vector<Lit> Clause;
};