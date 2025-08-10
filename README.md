# KtsAlgorithms
A collection algorithms

## Getting Started
### Requirements
- C++20 or newer
- CMake 3.30 or newer

### Install and Build
In your project's *CMakeLists.txt* file include following:
``` CMake
FetchContent_Declare(
    KtsAlgorithms
    GIT_REPOSITORY https://github.com/Kitso1999/KtsSortingAlgorithms.git
    GIT_TAG <version>
)

FetchContent_MakeAvailable(GoogleTest)
```

Then for your target add:
``` CMake
add_target(<target> PRIVATE Kts::Algorithms)
```

### Usage Example

``` C++
#include <vector>
#include <cassert>

#include <kts/algorithms/sort.hpp>

int main()
{
    std::vector v{ 3,4,5,1,9 };
    kts::BubbleSort( v.begin(), v.end() );
    assert( std::is_sorted( v.begin(), v.end() ) );
    kts::HeapSort( v.begin(), v.end(), std::greater<>{} );
    assert( std::is_sorted( v.rbegin(), v.rend() ) );    
}
```
