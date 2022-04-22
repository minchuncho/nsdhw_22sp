#include <cstdlib>
#include <new>
#include <memory>
#include <limits>
#include <vector>

#include "byte_counter.hpp"

template<class T>
class Allocator {
public:
	typedef T value_type;

    Allocator() = default;
    ~Allocator() = default;

	T* allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max()/sizeof(T)){
            throw std::bad_alloc();
        }
        
        const std::size_t bytes = n*sizeof(T);

        T* p = static_cast<T*>(std::malloc(bytes));
        if(p){
            counter_.inc_bytes(bytes);
            return p;
        }
        else{
            throw std::bad_alloc();
        }
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        std::free(p);

        const std::size_t bytes = n*sizeof(T);
        counter_.dec_bytes(bytes);
    }

    ByteCounter counter_;
};