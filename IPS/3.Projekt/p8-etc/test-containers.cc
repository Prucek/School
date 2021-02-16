// test.cc
// test efektivity standardnich kontejneru C++

const int MAXSZ = 10000000;     // limit velikosti kontejneru
const int N = 20;

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <algorithm>
#include <chrono>

template < typename Container > int test_push_back(Container & c, int sz)
{
    typename Container::value_type x(1);        // hodnota
    c.clear();
    for (int i = 0; i < sz; i++)
        c.push_back(i);         // naplneni

    unsigned ta[N];
    for (int j = 0; j < N; j++) {
        auto t2 = std::chrono::high_resolution_clock::now();
        c.push_back(x);         // operace
        auto t3 = std::chrono::high_resolution_clock::now();
        //auto t = t3 - t2 - (t2 - t1); // kompenzace rezie?
        std::chrono::duration < unsigned, std::nano > t = t3 - t2;
        ta[j] = t.count();      // zaznam mereni
    }
    std::sort(ta, ta + N);
    c.clear();
    return ta[N / 2];           /* median */
}

template < typename Container >
    int test_insert_begin(Container & c, int sz)
{
    typename Container::value_type x(0);        // hodnota
    c.clear();
    for (int i = 0; i < sz; i++)
        c.push_back(i);         // naplneni

    float ta[N];
    for (int j = 0; j < N; j++) {
        auto t2 = std::chrono::high_resolution_clock::now();
        c.insert(c.begin(), x); // operace
        auto t3 = std::chrono::high_resolution_clock::now();
        //auto t = t3 - t2 - (t2 - t1); // kompenzace
        std::chrono::duration < float, std::nano > t = t3 - t2;
        ta[j] = t.count();      // zaznam mereni
    }
    std::sort(ta, ta + N);
    c.clear();
    return ta[N / 2];           /* median */
}

template < typename Container > int test_insert(Container & c, int sz)
{
    c.clear();
    for (int i = 0; i < sz; i++)
        c.insert(i + 1);        // naplneni kontejneru

    float ta[N];
    for (int j = 0; j < N; j++) {
        typename Container::value_type x(j * sz / N);   // hodnota
        auto t2 = std::chrono::high_resolution_clock::now();
        c.insert(x);            // operace
        auto t3 = std::chrono::high_resolution_clock::now();
//        auto t = t3 - t2 - (t2 - t1); // kompenzace
        std::chrono::duration < float, std::nano > t = t3 - t2;
        ta[j] = t.count();      // zaznam mereni
    }
    std::sort(ta, ta + N);
    c.clear();
    return ta[N / 2];           /* median */
}

int main()
{
    std::vector < int >v;
    std::deque < int >d;
    std::list < int >l;
    std::set < int >s;
    for (int i = 16; i <= MAXSZ; i *= 2) {
        std::cout << i << '\t' << test_push_back(v, i)
            << '\t' << test_insert_begin(v, i)
            << '\t' << test_push_back(l, i)
            << '\t' << test_insert_begin(l, i)
            << '\t' << test_insert(s, i)
            << '\t' << test_push_back(d, i)
            << '\t' << test_insert_begin(d, i)
            << '\n';
    }
}
