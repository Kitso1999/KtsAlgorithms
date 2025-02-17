#ifndef KTS_SORTINGALGORITHMS_H_
#define KTS_SORTINGALGORITHMS_H_

#include <algorithm>
#include <random>
#include <concepts>
#include <iterator>

namespace kts
{

template<std::bidirectional_iterator Iter, std::sentinel_for<Iter> Sent,
         std::predicate<std::iter_value_t<Iter>, std::iter_value_t<Iter>> Pred>
void BubbleSort( Iter first, Sent last, Pred pred )
{
    for ( ; first != last; --last ) {
        bool swapped = false;

        for ( auto left = first, right = std::next( left ); right != last;
              ++left, ++right )
            if ( pred( *right, *left ) ) {
                std::iter_swap( left, right );
                swapped = true;
            }

        if ( !swapped )
            break;
    }
}

template<std::bidirectional_iterator Iter, std::sentinel_for<Iter> Sent>
void BubbleSort( Iter first, Sent last )
{
    BubbleSort( first, last, std::less{} );
}

template<std::forward_iterator Iter, std::sentinel_for<Iter> Sent,
         std::predicate<std::iter_value_t<Iter>, std::iter_value_t<Iter>> Pred>
void SelectionSort( Iter first, Sent last, Pred pred )
{
    for ( ; first != last; ++first ) {
        auto min = std::min_element( first, last, pred );
        std::iter_swap( first, min );
    }
}

template<std::forward_iterator Iter, std::sentinel_for<Iter> Sent>
void SelectionSort( Iter first, Sent last )
{
    SelectionSort( first, last, std::less{} );
}

template<std::bidirectional_iterator Iter, std::sentinel_for<Iter> Sent,
         std::predicate<std::iter_value_t<Iter>, std::iter_value_t<Iter>> Pred>
void InsertionSort( Iter first, Sent last, Pred pred )
{
    for ( auto next = first; next != last; ++next )
        for ( auto walker = next; walker != first; ) {
            const auto r = walker;
            const auto l = --walker;
            if ( !pred( *r, *l ) )
                break;
            std::iter_swap( l, r );
        }
}

template<std::bidirectional_iterator Iter, std::sentinel_for<Iter> Sent>
void InsertionSort( Iter first, Sent last )
{
    InsertionSort( first, last, std::less{} );
}

template<
    std::input_iterator Iter1, std::input_iterator Iter2,
    std::sentinel_for<Iter1> Sent1, std::sentinel_for<Iter2> Sent2,
    std::predicate<std::iter_value_t<Iter1>, std::iter_value_t<Iter2>> Pred,
    std::output_iterator<std::iter_value_t<Iter1>> DestIter>
void Merge( Iter1 first_l, Sent1 last_l, Iter2 first_r, Sent2 last_r,
            DestIter dest, Pred pred )
{
    while ( first_l != last_l && first_r != last_r )
        *dest++ = pred( *first_r, *first_l ) ? *first_r++ : *first_l++;

    while ( first_l != last_l )
        *dest++ = *first_l++;

    while ( first_r != last_r )
        *dest++ = *first_r++;
}
template<std::input_iterator Iter1, std::input_iterator Iter2,
         std::sentinel_for<Iter1> Sent1, std::sentinel_for<Iter2> Sent2,
         std::output_iterator<std::iter_value_t<Iter1>> DestIter>
    requires std::mergeable<Iter1, Iter2, DestIter, std::less<>>
void Merge( Iter1 first_l, Sent1 last_l, Iter2 first_r, Sent2 last_r,
            DestIter dest )
{
    Merge( first_l, last_l, first_r, last_r, dest, std::less{} );
}

template<std::random_access_iterator Iter, std::sentinel_for<Iter> Sent,
         std::predicate<std::iter_value_t<Iter>, std::iter_value_t<Iter>> Pred>
void MergeSort( Iter first, Sent last, Pred pred )
{
    if ( last <= first )
        return;

    auto size = std::distance( first, last );
    if ( size < 2 )
        return;

    auto first_l = first;
    auto last_l = first + size / 2;
    auto first_r = last_l;
    auto last_r = last;

    MergeSort( first_l, last_l, pred );
    MergeSort( first_r, last_r, pred );

    std::vector<std::iter_value_t<Iter>> temp;
    temp.reserve( size );

    Merge( first_l, last_l, first_r, last_r, std::back_inserter( temp ), pred );

    std::move( temp.begin(), temp.end(), first );
}

template<std::random_access_iterator Iter, std::sentinel_for<Iter> Sent>
void MergeSort( Iter first, Sent last )
{
    MergeSort( first, last, std::less{} );
}

template<std::bidirectional_iterator Iter, std::sentinel_for<Iter> Sent,
         std::predicate<std::iter_value_t<Iter>, std::iter_value_t<Iter>> Pred>
Iter PartitionWithPivot( Iter first, Sent last, Pred pred )
{
    // no need to range-check check because
    // QuickSort will call it on a non-empty range

    // used for bounds checking, this is not moved around
    const auto begin = first;
    auto pivot( std::move( *first ) );

    for ( ++first; first != last; ++first ) {
        // skip in-place elements at the beginning
        for ( ; first != last && pred( *first, pivot ); ++first ) {}

        if ( first == last )
            break;

        // skip in-place elements at the end
        do {
            --last;
        } while ( first != last && !pred( *last, pivot ) );

        if ( first == last )
            break;
        std::iter_swap( first, last ); // swap out-of-place elements
    }

    auto pivot_pos = std::prev( first );
    if ( begin != pivot_pos ) {
        *begin = std::move( *pivot_pos );
    }
    *pivot_pos = std::move( pivot );
    return first;
}

template<std::bidirectional_iterator Iter, std::sentinel_for<Iter> Sent,
         std::predicate<std::iter_value_t<Iter>, std::iter_value_t<Iter>> Pred>
void QuickSort( Iter first, Sent last, Pred pred )
{
    if ( first == last )
        return;

    auto partition_point = PartitionWithPivot( first, last, pred );

    QuickSort( first, std::prev( partition_point ), pred );
    QuickSort( partition_point, last, pred );
}

template<std::bidirectional_iterator Iter, std::sentinel_for<Iter> Sent>
void QuickSort( Iter first, Sent last )
{
    QuickSort( first, last, std::less{} );
}

template<std::random_access_iterator Iter, std::sentinel_for<Iter> Sent,
         std::predicate<std::iter_value_t<Iter>, std::iter_value_t<Iter>> Pred>
void Heapify( Iter first, Sent last, Iter start, Pred pred )
{
    const auto size = last - first;
    auto i = start - first;

    while ( i < size ) {
        auto parent = first + i;
        auto left_child = first + 2 * i + 1;
        auto right_child = first + 2 * i + 2;

        auto largest = parent;
        if ( left_child < last && pred( *largest, *left_child ) )
            largest = left_child;
        if ( right_child < last && pred( *largest, *right_child ) )
            largest = right_child;

        if ( largest == parent )
            return;
        std::iter_swap( parent, largest );
        i = std::distance( first, largest );
    }
}

template<std::random_access_iterator Iter, std::sentinel_for<Iter> Sent>
void Heapify( Iter first, Sent last, Iter start )
{
    Heapify( first, last, start, std::less{} );
}

template<std::random_access_iterator Iter, std::sentinel_for<Iter> Sent,
         std::predicate<std::iter_value_t<Iter>, std::iter_value_t<Iter>> Pred>
void PushHeap( Iter first, Sent last, Pred pred )
{
    const auto size = std::distance( first, last );

    if ( size > 1 )
        for ( auto i = size; i > 0 && pred( first + i / 2, first + i ); i /= 2 )
            std::swap( first + i / 2, first + i );
}

template<std::random_access_iterator Iter, std::sentinel_for<Iter> Sent>
void PushHeap( Iter first, Sent last )
{
    PushHeap( first, last, std::less{} );
}

template<std::random_access_iterator Iter, std::sentinel_for<Iter> Sent,
         std::predicate<std::iter_value_t<Iter>, std::iter_value_t<Iter>> Pred>
void PopHeap( Iter first, Sent last, Pred pred )
{
    std::iter_swap( first, --last );
    Heapify( first, last, first, pred );
}

template<std::random_access_iterator Iter, std::sentinel_for<Iter> Sent>
void PopHeap( Iter first, Sent last )
{
    PopHeap( first, last, std::less{} );
}

template<std::random_access_iterator Iter, std::sentinel_for<Iter> Sent,
         std::predicate<std::iter_value_t<Iter>, std::iter_value_t<Iter>> Pred>
void MakeHeap( Iter first, Sent last, Pred pred )
{
    if ( first == last )
        return;

    for ( auto walker = first + ( last - first ) / 2; walker-- != first; )
        Heapify( first, last, walker, pred );
}

template<std::random_access_iterator Iter, std::sentinel_for<Iter> Sent>
void MakeHeap( Iter first, Sent last )
{
    MakeHeap( first, last, std::less{} );
}

template<std::random_access_iterator Iter, std::sentinel_for<Iter> Sent,
         std::predicate<std::iter_value_t<Iter>, std::iter_value_t<Iter>> Pred>
void HeapSort( Iter first, Sent last, Pred pred )
{
    MakeHeap( first, last, pred );

    for ( auto size = last - first; size > 1; --size, --last )
        PopHeap( first, last, pred );
}

template<std::random_access_iterator Iter, std::sentinel_for<Iter> Sent>
void HeapSort( Iter first, Sent last )
{
    HeapSort( first, last, std::less{} );
}

} // namespace kts

#endif // KTS_SORTINGALGORITHMS_H_
