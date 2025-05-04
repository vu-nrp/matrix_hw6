#include <map>
#include <tuple>
#include <cassert>
#include <sstream>
#include <iostream>

//!
//! \brief to_str - value to string representation function
//! \param val - input value
//! \return - string representation
//!

template <typename T>
std::string to_str(const T &val)
{
    std::ostringstream oss;
    oss << val;
    return oss.str();
}

//!
//! \brief Index - matrix indexing type
//!

using Index = int;

//!
//! \brief Coord - 2d matrix coordinate
//!

using Coord = std::pair<Index, Index>;

//!
//! \brief Proxy - struct predeclaration
//!

template<typename T, const T def>
struct Proxy;

//!
//! \brief The Matrix struct - main 2D matrix struct
//!

template<typename T, const T def>
struct Matrix
{
    static constexpr T Def {def};

    Proxy<T, def> operator[](const Index &x)
    {
        return Proxy<T, def>(this, x);
    }

    size_t size() const
    {
        return m_matrix.size();
    }

    T find(const Coord &coord) const
    {
        const auto it = m_matrix.find(coord);
        return (it == m_matrix.cend())? Def : it->second;
    }

    void remove(const Coord &coord)
    {
        m_matrix.erase(m_matrix.find(coord));
    }

    void insert(const Coord &coord, const T &value)
    {
        m_matrix.emplace(coord, value);
    }


    struct Iterator
    {
        using Type = typename std::map<Coord, T>::iterator;

        Iterator(Type matrixIter) :
            m_iterator(matrixIter)
        {
        }

        const Type &iterator() const
        {
            return m_iterator;
        }

        void operator++()
        {
            m_iterator++;
        }

        std::tuple<Index, Index, T> operator*() const
        {
            return std::make_tuple(m_iterator->first.first, m_iterator->first.second, m_iterator->second);
        }

        bool operator!=(const Iterator &other) const
        {
            return m_iterator != other.iterator();
        }

    private:
        Type m_iterator;

    };

    Iterator begin()
    {
        return Iterator(m_matrix.begin());
    }

    Iterator end()
    {
        return Iterator(m_matrix.end());
    }

private:
    std::map<Coord, T> m_matrix;

};

//!
//! \brief The Proxy struct - matrix proxy struct for access (operator[]) and setup values
//!

template<typename T, const T def>
struct Proxy
{
    Proxy(Matrix<T, def> *matrix, const Index &x) :
        m_matrix(matrix),
        m_element(def),
        m_x(x)
    {
    }

    Proxy &operator[](const Index &y)
    {
        m_coord = {m_x, y};
        m_element = m_matrix->find(m_coord);
        return *this;
    }

    void operator=(const T &value)
    {
        if (value != Matrix<T, def>::Def) {
            if (m_element != Matrix<T, def>::Def)
                m_element = value;
            else
                m_matrix->insert(m_coord, value);
        } else if (m_element != Matrix<T, def>::Def)
            m_matrix->remove(m_coord);
    }

    operator T() const
    {
        return m_element;
    }

private:
    Matrix<T, def> *m_matrix {nullptr};
    std::remove_const_t<T> m_element;
    Coord m_coord;
    Index m_x;

};

//!
//! \brief main - main app function
//!

int main()
{
//    std::cout << "Home work #6" << std::endl;

    // type & def value
    static const auto DefValue {0};
    Matrix<decltype(DefValue), DefValue> matrix;

    // tmp vars
    Index x, y;
    std::remove_const_t<decltype(DefValue)> v;

    // test 1
    assert(matrix.size() == 0);

    // test 2
    const auto &a = matrix[0][0];
    assert(a == DefValue);
    assert(matrix.size() == 0);

    // test 3
    matrix[100][100] = 100;
    assert(matrix[100][100] == 100);
    assert(matrix.size() == 1);

    std::string test;
    for (const auto &item: matrix) {
        std::tie(x, y, v) = item;
        test += ("[" + to_str(x) + "][" + to_str(y) + "]: " + to_str(v));
    }
    assert(test.compare("[100][100]: 100") == 0);

    // test 4
    matrix[100][100] = DefValue;
    assert(matrix[100][100] == DefValue);
    assert(matrix.size() == 0);

    // fill
    static const auto MaxIndex {9};
    for (auto index = 0; index <= MaxIndex; ++index) {
        matrix[index][index] = index;
        const auto tmpIndex = MaxIndex - index;
        matrix[index][tmpIndex] = tmpIndex;
    }

    // print 1 - 8
    static const Coord PrintRange {1, 8};
    for (auto i = PrintRange.first; i <= PrintRange.second; i++)
        for (auto j = PrintRange.first; j <= PrintRange.second; j++)
            std::cout << matrix[i][j] << ((j == PrintRange.second) ? "\n" : " ");
    std::cout << std::endl;

    // occupied count
    std::cout << "occupied count: " << matrix.size() << std::endl;
    std::cout << std::endl;

    // all occupied items
    for (const auto &item: matrix){
        std::tie(x, y, v) = item;
        std::cout << ("[" + to_str(x) + "][" + to_str(y) + "]: " + to_str(v)) << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
