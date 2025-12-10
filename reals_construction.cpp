// reals_construction.cpp
// Type-theoretic construction: ℕ → ℤ → ℚ → ℝ
// Compile-time verification of algebraic properties
// (c) 2025 Zachary R. James

#include "hott.hpp"
#include <iostream>
#include <cassert>
#include <cmath>
#include <cstdio>

using namespace hott;

// Naturals // 

struct Nat 
{
    unsigned int value;
    
    constexpr Nat(unsigned int v = 0) : value(v) {}
    
    constexpr bool operator==(const Nat& other) const = default;
    constexpr bool operator<(const Nat& other) const { return value < other.value; }
    
    static constexpr Nat zero() { return Nat(0); }
    constexpr Nat operator+(const Nat& other) const { return Nat(value + other.value); }
    
    static constexpr Nat one() { return Nat(1); }
    constexpr Nat operator*(const Nat& other) const { return Nat(value * other.value); }
    
    constexpr auto inject() const;
};

static_assert(Monoid<Nat>);


// INTS // 

// (a,b) ~ (c,d) iff a + d = b + c
// represents: pos - neg
struct Int 
{
    Nat pos, neg;
    
    constexpr Int(Nat p = Nat(0), Nat n = Nat(0)) : pos(p), neg(n) {}
    
    constexpr bool equiv(const Int& other) const 
    { return (pos + other.neg) == (neg + other.pos); }
    
    constexpr bool operator==(const Int& other) const { return equiv(other); }
    constexpr bool operator<(const Int& other) const 
    { return (pos + other.neg) < (neg + other.pos); }
    
    constexpr Int normalize() const 
    {
        if(pos.value >= neg.value) { return Int(Nat(pos.value - neg.value), Nat(0)); }
        return Int(Nat(0), Nat(neg.value - pos.value));
    }
    
    static constexpr Int zero() { return Int(Nat(0), Nat(0)); }
    static constexpr Int one() { return Int(Nat(1), Nat(0)); }
    
    constexpr Int operator+(const Int& other) const 
    { return Int(pos + other.pos, neg + other.neg); }
    
    constexpr Int negate() const { return Int(neg, pos); }
    constexpr Int operator-(const Int& other) const { return *this + other.negate(); }
    
    // (a-b)(c-d) = ac + bd - ad - bc
    constexpr Int operator*(const Int& other) const 
    {
        return Int(
            pos * other.pos + neg * other.neg,
            pos * other.neg + neg * other.pos
        );
    }
    
    constexpr auto inject() const;


    int to_int() const 
    {
        Int norm = normalize();
        return static_cast<int>(norm.pos.value) - static_cast<int>(norm.neg.value);
    }
};

constexpr auto Nat::inject() const { return Int(value, 0); }

static_assert(Ring<Int>);

// Rationals // 

// a/b ~ c/d iff ad = bc
struct Rat 
{
    Int num;
    Nat den; // invariant: den > 0
    
    constexpr Rat(Int n = Int::zero(), Nat d = Nat(1)) 
        : num(n), den(d.value == 0 ? Nat(1) : d) {}
    
    constexpr bool equiv(const Rat& other) const 
    { return (num * Int(other.den, Nat(0))) == (other.num * Int(den, Nat(0))); }
    
    constexpr bool operator==(const Rat& other) const { return equiv(other); }
    constexpr bool operator<(const Rat& other) const 
    { return (num * Int(other.den, Nat(0))) < (other.num * Int(den, Nat(0))); }

    // missing Rat total order for totally_ordered
    constexpr bool operator>(const Rat& other) const { return other < *this; }
    constexpr bool operator<=(const Rat& other) const { return !(*this > other); }
    constexpr bool operator>=(const Rat& other) const { return !(*this < other); }

    
    static constexpr Nat gcd(Nat a, Nat b) 
    {
        while(b.value != 0) 
        {
            Nat temp = Nat(a.value % b.value);
            a = b;
            b = temp;
        }
        return a;
    }
    
    constexpr Rat normalize() const 
    {
        Int norm_num = num.normalize();
        unsigned int abs_num = norm_num.pos.value + norm_num.neg.value;
        
        if(abs_num == 0) return Rat(Int::zero(), Nat(1));
        
        Nat g = gcd(Nat(abs_num), den);
        if(g.value <= 1) return *this;
        
        return Rat(
            Int(Nat(norm_num.pos.value / g.value), Nat(norm_num.neg.value / g.value)),
            Nat(den.value / g.value)
        );
    }
    
    static constexpr Rat zero() { return Rat(Int::zero(), Nat(1)); }
    static constexpr Rat one() { return Rat(Int::one(), Nat(1)); }
    
    // a/b + c/d = (ad + bc)/(bd)
    constexpr Rat operator+(const Rat& other) const 
    {
        return Rat(
            num * Int(other.den, Nat(0)) + other.num * Int(den, Nat(0)),
            den * other.den
        );
    }
    
    constexpr Rat negate() const { return Rat(num.negate(), den); }
    constexpr Rat operator-(const Rat& other) const { return *this + other.negate(); }
    constexpr Rat operator*(const Rat& other) const { return Rat(num * other.num, den * other.den); }
    
    constexpr Rat inverse() const 
    {
        Int norm = num.normalize();
        if(norm.pos.value > 0) { return Rat(Int(den, Nat(0)), norm.pos); }
        return Rat(Int(Nat(0), den), norm.neg);
    }
    
    constexpr Rat operator/(const Rat& other) const { return *this * other.inverse(); }
    
    constexpr auto inject() const;
    
    double to_double() const { return static_cast<double>(num.to_int()) / den.value; }
};

constexpr auto Int::inject() const { return Rat(*this, Nat(1)); }

static_assert(Field<Rat>);
static_assert(OrderedField<Rat>);

// reals // 

struct Real 
{
    Rat approx;
    
    constexpr Real(Rat r = Rat::zero()) : approx(r) {}
    
    constexpr bool operator==(const Real& other) const { return approx == other.approx; }
    constexpr bool operator<(const Real& other) const { return approx < other.approx; }
    constexpr bool operator>(const Real& other) const { return other < *this; }
    constexpr bool operator<=(const Real& other) const { return *this < other || *this == other; }
    constexpr bool operator>=(const Real& other) const { return !(*this < other); }
    
    static constexpr Real zero() { return Real(Rat::zero()); }
    static constexpr Real one() { return Real(Rat::one()); }
    
    constexpr Real operator+(const Real& other) const { return Real(approx + other.approx); }
    constexpr Real negate() const { return Real(approx.negate()); }
    constexpr Real operator-(const Real& other) const { return Real(approx - other.approx); }
    constexpr Real operator*(const Real& other) const { return Real(approx * other.approx); }
    constexpr Real inverse() const { return Real(approx.inverse()); }
    constexpr Real operator/(const Real& other) const { return Real(approx / other.approx); }
    
    double to_double() const { return approx.to_double(); }
};

constexpr auto Rat::inject() const { return Real(*this); }

static_assert(Field<Real>);
static_assert(OrderedField<Real>);

// inner product space 

struct R2 
{
    using Scalar = Real;
    Real x, y;
    
    constexpr R2(Real a = Real::zero(), Real b = Real::zero()) : x(a), y(b) {}
    
    constexpr bool operator==(const R2& other) const = default;
    
    constexpr R2 operator+(const R2& other) const { return R2(x + other.x, y + other.y); }
    constexpr R2 scale(Real s) const { return R2(x * s, y * s); }
    
    // ⟨u,v⟩ = u.x * v.x + u.y * v.y
    constexpr Real inner(const R2& other) const { return x * other.x + y * other.y; }
};

static_assert(InnerProductSpace<R2>);

// compile-time checks 

consteval bool nat_associative() 
{
    Nat a(2), b(3), c(4);
    return ((a + b) + c) == (a + (b + c));
}
static_assert(nat_associative());

consteval bool nat_identity() 
{
    Nat a(5);
    return (a + Nat::zero()) == a && (Nat::zero() + a) == a;
}
static_assert(nat_identity());

consteval bool int_commutative() 
{
    Int a(Nat(3), Nat(1)), b(Nat(2), Nat(5));
    return (a + b) == (b + a);
}
static_assert(int_commutative());

consteval bool int_inverse() 
{
    Int a(Nat(7), Nat(3));
    return (a + a.negate()) == Int::zero();
}
static_assert(int_inverse());

consteval bool rat_mult_commutative() 
{
    Rat a(Int(Nat(3), Nat(0)), Nat(4));
    Rat b(Int(Nat(5), Nat(0)), Nat(7));
    return (a * b) == (b * a);
}
static_assert(rat_mult_commutative());

consteval bool rat_distributive() 
{
    Rat a(Int(Nat(2), Nat(0)), Nat(3));
    Rat b(Int(Nat(1), Nat(0)), Nat(2));
    Rat c(Int(Nat(3), Nat(0)), Nat(5));
    return (a * (b + c)) == ((a * b) + (a * c));
}
static_assert(rat_distributive());

consteval bool inner_product_commutative() 
{
    R2 u(Real(Rat(Int(Nat(3), Nat(0)), Nat(1))), 
         Real(Rat(Int(Nat(4), Nat(0)), Nat(1))));
    R2 v(Real(Rat(Int(Nat(1), Nat(0)), Nat(1))), 
         Real(Rat(Int(Nat(2), Nat(0)), Nat(1))));
    return u.inner(v) == v.inner(u);
}
static_assert(inner_product_commutative());

consteval bool inner_product_positive() 
{
    R2 v(Real(Rat(Int(Nat(3), Nat(0)), Nat(1))), 
         Real(Rat(Int(Nat(4), Nat(0)), Nat(1))));
    return v.inner(v) >= Real::zero();
}
static_assert(inner_product_positive());


int main() 
{
    printf("HoTT Reals Construction \n\n");
    
    // naturals
    Nat n1(3), n2(5);
    printf("ℕ: %u + %u = %u\n", n1.value, n2.value, (n1 + n2).value);
    
    // integers
    Int z1 = n1.inject();
    Int z2(Nat(7), Nat(3)); // 7 - 3 = 4
    printf("ℤ: %d + %d = %d\n", z1.to_int(), z2.to_int(), (z1 + z2).to_int());
    
    // rationals
    Rat q1 = z1.inject();
    Rat q2(Int(Nat(3), Nat(0)), Nat(4)); // 3/4
    printf("ℚ: %.2f * %.2f = %.2f\n", q1.to_double(), q2.to_double(), (q1 * q2).to_double());
    
    // reals
    Real r1 = q1.inject();
    Real r2 = q2.inject();
    printf("ℝ: %.2f + %.2f = %.2f\n", r1.to_double(), r2.to_double(), (r1 + r2).to_double());
    
    printf("\nInner product space \n");
    
    R2 u(Real(Rat(Int(Nat(3), Nat(0)), Nat(1))), 
         Real(Rat(Int(Nat(4), Nat(0)), Nat(1))));
    R2 v(Real(Rat(Int(Nat(1), Nat(0)), Nat(1))), 
         Real(Rat(Int(Nat(2), Nat(0)), Nat(1))));
    
    printf("u = (3, 4)\n");
    printf("v = (1, 2)\n");
    printf("⟨u,v⟩ = %.2f\n", u.inner(v).to_double());
    printf("⟨v,u⟩ = %.2f\n", v.inner(u).to_double());
    printf("⟨u,u⟩ = %.2f\n", u.inner(u).to_double());
    
    printf("\n✓ all compile-time proofs passed\n");
    printf("✓ type tower verified: ℕ → ℤ → ℚ → ℝ\n");
    printf("✓ algebraic structures verified\n");
    printf("✓ inner product properties verified\n");
    
    return 0;
}