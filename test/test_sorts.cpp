#include <gtest/gtest.h>
#include <concepts>
#include <sorting_algorithms.hpp>

struct BubbleSortFunc
{
    template<typename... Args>
    void operator()( Args &&...args )
    {

        kts::BubbleSort( std::forward<Args>( args )... );
    }
};

struct SelectionSortFunc
{
    template<typename... Args>
    void operator()( Args &&...args )
    {
        kts::SelectionSort( std::forward<Args>( args )... );
    }
};

struct InsertionSortFunc
{
    template<typename... Args>
    void operator()( Args &&...args )
    {
        kts::InsertionSort( std::forward<Args>( args )... );
    }
};

struct MergeSortFunc
{
    template<typename... Args>
    void operator()( Args &&...args )
    {
        kts::MergeSort( std::forward<Args>( args )... );
    }
};

struct QuickSortFunc
{
    template<typename... Args>
    void operator()( Args &&...args )
    {
        kts::QuickSort( std::forward<Args>( args )... );
    }
};

struct HeapSortFunc
{
    template<typename... Args>
    void operator()( Args &&...args )
    {
        kts::HeapSort( std::forward<Args>( args )... );
    }
};

template<std::invocable Func>
class SortTest : public ::testing::Test
{
protected:
    Func func{};
};

using SortTestTypes =
    ::testing::Types<BubbleSortFunc, SelectionSortFunc, InsertionSortFunc,
                     MergeSortFunc, QuickSortFunc, HeapSortFunc>;
TYPED_TEST_SUITE( SortTest, SortTestTypes );

TYPED_TEST( SortTest, SortsZeroInts )
{
    std::deque<int> dq = {};
    std::vector<int> v = {};
    std::array<int, 0> arr = {};
    this->func( dq.begin(), dq.end() );
    EXPECT_TRUE( dq.empty() );
    this->func( v.begin(), v.end() );
    EXPECT_TRUE( v.empty() );
    this->func( arr.begin(), arr.end() );
    EXPECT_TRUE( arr.empty() );
}

TYPED_TEST( SortTest, SortsOneInt )
{
    std::deque dq = { 1 };
    std::vector v = { 2 };
    std::array arr = { 3 };
    this->func( dq.begin(), dq.end() );
    EXPECT_TRUE( ( dq.size() == 1 && dq.front() == 1 ) );
    this->func( v.begin(), v.end() );
    EXPECT_TRUE( ( v.size() == 1 && v.front() == 2 ) );
    this->func( arr.begin(), arr.end() );
    EXPECT_TRUE( ( arr.size() == 1 && arr.front() == 3 ) );
}

TYPED_TEST( SortTest, SortsTenInts )
{
    static constexpr int initial[] = { 31, 54, 23, 6, 4, 23, 6, 4, 2, 3 };
    std::deque dq{ std::from_range, initial };
    std::vector v{ std::from_range, initial };
    std::array<int, std::size( initial )> arr{};
    std::ranges::copy( initial, arr.begin() );

    const auto sorted = [] {
        std::vector vec( std::from_range, initial );
        std::ranges::sort( vec );
        return vec;
    }();

    this->func( dq.begin(), dq.end() );
    EXPECT_TRUE( std::ranges::equal( dq, sorted ) );
    this->func( v.begin(), v.end() );
    EXPECT_TRUE( std::ranges::equal( v, sorted ) );
    this->func( arr.begin(), arr.end() );
    EXPECT_TRUE( std::ranges::equal( arr, sorted ) );
}

TYPED_TEST( SortTest, SortsThousandRandomInts )
{
    std::deque<int> dq( 1000 );
    std::vector<int> v( 1000 );
    std::array<int, 1000> arr{};

    std::random_device rd;
    std::default_random_engine rng{ rd() };
    std::uniform_int_distribution dist;

    for ( auto &val : dq )
        val = dist( rng );
    for ( auto &val : v )
        val = dist( rng );
    for ( auto &val : arr )
        val = dist( rng );

    this->func( dq.begin(), dq.end() );
    EXPECT_TRUE( std::ranges::is_sorted( dq ) );
    this->func( v.begin(), v.end() );
    EXPECT_TRUE( std::ranges::is_sorted( v ) );
    this->func( arr.begin(), arr.end() );
    EXPECT_TRUE( std::ranges::is_sorted( arr ) );
}