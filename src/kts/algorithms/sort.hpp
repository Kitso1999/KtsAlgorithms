#include <algorithm>
#include <concepts>
#include <iterator>
#include <vector>

namespace kts
{
template<std::random_access_iterator Iter, std::sentinel_for<Iter> Sent,
         std::predicate<std::iter_value_t<Iter>, std::iter_value_t<Iter>> Pred>
void Heapify( Iter first, Sent last, Iter start, Pred pred )
{
    const auto size = last - first;
    auto i = start - first;

    while ( i < size ) {
        const auto l = 2 * i + 1;
        const auto r = 2 * i + 2;

        auto largest = i;
        if ( l < size && pred( *(first + largest), *(first + l) ) )
            largest = l;
        if ( r < size && pred( *(first + largest), *(first + r) ) )
            largest = r;

        if ( largest == i )
            return;
        std::iter_swap( first + i, first + largest );
        i = largest;
    }
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

template<std::random_access_iterator Iter, std::sentinel_for<Iter> Sent,
         std::predicate<std::iter_value_t<Iter>, std::iter_value_t<Iter>> Pred>
void PopHeap( Iter first, Sent last, Pred pred )
{
    std::iter_swap( first, --last );
    Heapify( first, last, first, pred );
}

template<std::random_access_iterator Iter, std::sentinel_for<Iter> Sent,
         std::predicate<std::iter_value_t<Iter>, std::iter_value_t<Iter>> Pred>
void MakeHeap( Iter first, Sent last, Pred pred )
{
    if ( first == last )
        return;

    for ( auto walker = first + ( last - first ) / 2; walker != first; )
        Heapify( first, last, --walker, pred );
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

}

// vvv export when C++20 modules will be widely supported and adapted
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

    Merge( std::make_move_iterator( first_l ), std::make_move_iterator( last_l ), std::make_move_iterator( first_r ),
           std::make_move_iterator( last_r ), std::back_inserter( temp ), std::move( pred ) );

    std::move( temp.begin(), temp.end(), std::move( first ) );
}

template<std::random_access_iterator Iter, std::sentinel_for<Iter> Sent>
void MergeSort( Iter first, Sent last )
{
    MergeSort( first, last, std::less{} );
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
