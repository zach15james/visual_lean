// hott.hpp - Homotopy Type Theory Foundation
// Martin-Löf Dependent Type Theory + EoP demo - to be modified in the future !! 
// (c) 2025 Zachary R. James

#ifndef HOTT_HPP
#define HOTT_HPP

#include <concepts>
#include <functional>
#include <type_traits>


struct Nat;
struct Int;
struct Rat;
struct Real;



namespace hott {

// EoP Ch1: Regular types (equality, copy, assign)
template<typename T>
concept Regular = std::equality_comparable<T> && 
                  std::copyable<T> && 
                  std::default_initializable<T>;

template<typename T>
concept TotallyOrdered = Regular<T> && std::totally_ordered<T>;

// Identity type: Id_A(a,b) := proof that a = b
template<Regular A>
struct Id 
{
    A lhs, rhs;
    bool holds;
    
    constexpr Id(A a, A b) : lhs(a), rhs(b), holds(a == b) {}
    
    // refl : (a : A) → Id A a a
    static constexpr Id refl(A a) { return Id(a, a); }
    
    // sym : Id a b → Id b a
    constexpr Id sym() const { return Id(rhs, lhs); }
    
    // trans : Id a b → Id b c → Id a c
    constexpr Id trans(const Id& other) const 
        requires std::same_as<A, decltype(other.lhs)>
    { return Id(lhs, other.rhs); }
    
    // J eliminator (path induction)
    template<typename P>
    constexpr P elim(P base) const { return holds ? base : P{}; }
    
    constexpr bool proven() const { return holds; }
};

// Σ-type: dependent pair Σ(x:A).B(x)
template<Regular A, typename B>
struct Sigma 
{
    A fst;
    B snd;
    
    constexpr Sigma(A a, B b) : fst(a), snd(b) {}
    
    constexpr bool operator==(const Sigma& other) const 
        requires std::equality_comparable<B>
    { return fst == other.fst && snd == other.snd; }
};

// function extensionality
template<Regular A, Regular B>
struct FunExt 
{
    using Fn = std::function<B(A)>;
    static constexpr bool pointwise_equal(Fn f, Fn g, A x) { return f(x) == g(x); }
};

// transport : Id A a b → P(a) → P(b)
template<Regular A, typename P>
constexpr P transport(const Id<A>& path, P pa) { return path.holds ? pa : P{}; }

// equivalence (≃)
template<Regular A, Regular B>
struct Equiv 
{
    using FwdFn = std::function<B(A)>;
    using BwdFn = std::function<A(B)>;
    
    FwdFn to;
    BwdFn from;
    
    constexpr Equiv(FwdFn f, BwdFn g) : to(f), from(g) {}
    constexpr bool is_equiv(A a) const { return from(to(a)) == a; }
};

// univalence: (A ≃ B) → (A = B)
template<Regular A, Regular B>
struct Univalence 
{
    static constexpr bool axiom(Equiv<A,B> equiv, A a) { return equiv.is_equiv(a); }
};

// compile-time proof system
template<bool P>
struct Proof 
{
    static constexpr bool value = P;
    static_assert(P, "proof failed");
};

template<bool P, bool Q>
using And = Proof<P && Q>;

template<bool P, bool Q>
using Or = Proof<P || Q>;

template<bool P, bool Q>
using Implies = Proof<!P || Q>;

template<bool P>
using Not = Proof<!P>;

// universal quantification
template<typename F, size_t N>
struct Forall 
{
    static constexpr bool value = []() consteval {
        for(size_t i = 0; i < N; ++i) { if(!F{}(i)) return false; }
        return true;
    }();
};

// algebraic structures (EoP)
template<typename T>
concept Magma = Regular<T> && requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

template<typename T>
concept Semigroup = Magma<T>;

template<typename T>
concept Monoid = Semigroup<T> && requires {
    { T::zero() } -> std::convertible_to<T>;
};

template<typename T>
concept Group = Monoid<T> && requires(T a) {
    { a.negate() } -> std::convertible_to<T>;
};

template<typename T>
concept Ring = Group<T> && requires(T a, T b) {
    { a * b } -> std::convertible_to<T>;
    { T::one() } -> std::convertible_to<T>;
};

template<typename T>
concept Field = Ring<T> && requires(T a) {
    { a.inverse() } -> std::convertible_to<T>;
};

template<typename T>
concept OrderedField = Field<T> && TotallyOrdered<T>;

// inner product space
template<typename V>
concept InnerProductSpace = requires(V u, V v) {
    { u + v } -> std::convertible_to<V>;
    { u.inner(v) } -> std::convertible_to<typename V::Scalar>;
    requires Field<typename V::Scalar>;
};

// verification helpers for inner product properties
template<InnerProductSpace V>
struct VerifyInnerProduct 
{
    using S = typename V::Scalar;
    
    // ⟨u,v⟩ = ⟨v,u⟩
    static constexpr bool commutative(V u, V v) { return u.inner(v) == v.inner(u); }
    
    // ⟨au + bv, w⟩ = a⟨u,w⟩ + b⟨v,w⟩
    static constexpr bool linear(S a, V u, S b, V v, V w) {
        auto lhs = (u.scale(a) + v.scale(b)).inner(w);
        auto rhs = u.inner(w).scale(a) + v.inner(w).scale(b);
        return lhs == rhs;
    }
    
    // ⟨v,v⟩ ≥ 0
    static constexpr bool positive_definite(V v) { return v.inner(v) >= S::zero(); }
    
    // |⟨u,v⟩|² ≤ ⟨u,u⟩⟨v,v⟩
    static constexpr bool cauchy_schwarz(V u, V v) {
        auto uv = u.inner(v);
        auto uu = u.inner(u);
        auto vv = v.inner(v);
        return (uv * uv) <= (uu * vv);
    }
};

// number tower: ℕ → ℤ → ℚ → ℝ
template<size_t Level>
struct NumberLevel {};

template<typename T>
struct NextLevel;

// Were causing issues 
//struct Nat;
//struct Int;
//struct Rat;
//struct Real;

template<> struct NextLevel<Nat> { using type = Int; };
template<> struct NextLevel<Int> { using type = Rat; };
template<> struct NextLevel<Rat> { using type = Real; };

// injection: A → NextLevel<A>
template<typename A>
concept HasInjection = Regular<A> && requires(A a) {
    { a.inject() } -> std::convertible_to<typename NextLevel<A>::type>;
};


} // namespace hott

#endif // HOTT_HPP