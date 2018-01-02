#pragma once
////////////////////////

template <size_t index, typename  First, typename ...Others>
struct Tuple_element {
    using Type = typename Tuple_element<index - 1, Others...>::Type;
};

template <typename First, typename ...Others>
struct Tuple_element<0, First, Others...> {
    using Type = First;
};

template< std::size_t I, typename ... Types>
using Tuple_type_t = typename Tuple_element<I, Types...>::Type;

//////////////////////
/*template<size_t I, typename First,  typename ... Other>
struct getTupleElement;
*/
template <typename ...Types>
class Tuple {
public:

private:
};


template <class FirstType, typename ...OtherTypes>
class Tuple<FirstType, OtherTypes...> {

    /*template< int I, typename First,  typename ... Other>
    friend constexpr Tuple_type_t<I, First, Other...>&
    get(Tuple<First, Other...>& t );
*/

    template< size_t I>
    friend
    struct getTupleElement;

    public:
    template<typename First, typename ... Other>
    explicit Tuple(const First& first, const Other& ... others):element(first), others(others...) {
    }

    Tuple():element(), others() {
    }
    template<typename First, typename ... Other>
    explicit Tuple(First&& first, Other&& ... others):
            element(std::forward<First>(first)), others(std::forward<Other>(others)...){
    }


    explicit Tuple(const Tuple& otherTuple):element(otherTuple.element), others(otherTuple.others){
    }

    Tuple(Tuple&& otherTuple):element(std::move(otherTuple.element)), others(std::move(otherTuple.others)){
    }


    Tuple& operator =(const Tuple& otherTuple){
        element = otherTuple.element;
        others = otherTuple.others;
        return *this;
    }

    Tuple& operator =(Tuple&& otherTuple)
    {
        element = std::move(otherTuple.element);
        others = std::move(otherTuple.others);
        return *this;
    }

    ~Tuple(){
    }

    void swap(Tuple<FirstType, OtherTypes...>& otherTuple){
        std::swap(element, otherTuple.element);
        others.swap(otherTuple.others);
    }
    void swap(Tuple<>&){
    }

private:
    FirstType element;
    Tuple<OtherTypes...> others;
};

template<>
class Tuple<>
{
public:
    void swap(Tuple<>&) noexcept {}
};


template <class T>
struct unwrap_refwrapper
{
    using type = T;
};

template <class T>
struct unwrap_refwrapper<std::reference_wrapper<T>>{
    using type = T&;
};

template <class T>
using special_decay_t = typename unwrap_refwrapper<typename std::decay<T>::type>::type;

template <class... Types>
auto makeTuple(Types&&... args)
{
   return Tuple<special_decay_t<Types>...>(std::forward<Types>(args)...);
}
template< size_t I>
struct getTupleElement{
    template <typename T>
    static decltype(auto) get(T&& t) {
        return getTupleElement<I - 1>::get(t.others);
    };
};

template <>
struct getTupleElement<0>{
    template <typename T>
    static decltype(auto) get(T&& t) {
        return t.element;
    };
};

template <size_t I, class T>
decltype(auto) get(T &&t) {
    return getTupleElement<I>::get(std::forward<T>(t));
}


template< typename I>
struct getTupleElementT{
    template < typename >
    static decltype(auto)  get(Tuple<First, Other...>& t){
        return getTupleElementT<I>::get(t.others);
    };
};


template< typename I>
struct getTupleElementT{
    template < typename First,  typename ... Other>
    static decltype(auto)  get(Tuple<First, Other...>& t){
        return getTupleElementT<I>::get(t.others);
    };
};


/*
template <typename I, class T>
decltype(auto) get(T &&t) {
    return getTupleElementT<I,T>::get(std::forward<T>(t));
}*/
///////////////////////////////////////////////
/*
template< size_t I, typename First,  typename ... Other>
struct getTupleElement{
    constexpr static Tuple_type_t<I, First, Other...>& get(Tuple<First, Other...>& t){
        return getTupleElement<I-1, Other...>::get(t.others);
    };

    constexpr static const Tuple_type_t<I, First, Other...>& get(const Tuple<First, Other...>& t){
        return const_cast<const Tuple_type_t<I, First, Other...>&>(getTupleElement<I-1, Other...>::get(t.others));
    };

    constexpr static Tuple_type_t<I, First, Other...>&& get(Tuple<First, Other...>&& t){
        return std::move(getTupleElement<I-1, Other...>::get(t.others));
    };
};

template<typename First,  typename ... Other>
struct getTupleElement<0, First, Other...>{
    constexpr static Tuple_type_t<0, First, Other...>& get(Tuple<First, Other...>& t){
        return t.element;
    };
    constexpr const static Tuple_type_t<0, First, Other...>& get(const Tuple<First, Other...>& t){
        return t.element;
    };
    constexpr static Tuple_type_t<0, First, Other...>&& get(Tuple<First, Other...>&& t){
        return t.element;
    };
*/


///////////////////////////////





