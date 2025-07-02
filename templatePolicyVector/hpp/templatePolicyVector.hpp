#pragma once

#include <cstddef>
#include <cstdlib>
#include <mutex>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vcruntime_new.h>

namespace policy_vector {

    //New/Delete memory allocation policy
    // Uses global new/delete operators for memory management
    template<typename T>
    struct NewDelAllocate{
        // Function to deallocate memory
        T* allocate(std::size_t n){
            return static_cast<T*>(::operator new(n * sizeof(T)));
        }

        // Function to deallocate memory
        void deallocate(T* p, std::size_t){
            ::operator delete(p);
        }

    };

    // Malloc memory allocation policy
    // Uses malloc/free for memory management
    template<typename T>
    struct MallocFreeAllocate{
        // Function to allocate memory
        T* allocate(std::size_t n){
            return static_cast<T*>(std::malloc(n * sizeof(T)));
        }
        // Function to deallocate memory
        void deallocate(T* p, std::size_t){
            free(p);
        }
    };

    // NoLock policy
    // Does not provide any locking mechanism
    template<typename T>
    struct NoLock{
        void lock() noexcept {};
        void unlock() noexcept {};
    };

    // MutexLock policy
    // Uses std::mutex for locking
    template<typename T>
    struct MutexLock{
        private:
        std::mutex mtx_;
        public:
        void lock() noexcept{
            mtx_.lock();
        }
        void unlock() noexcept{
            mtx_.unlock();
        }
    };

    // ErrorPolicy that does nothing
    // Does not handle errors
    template<typename T>
    struct NoError{
        void handle_error(const char* msg) const noexcept {}
    };

    // ErrorPolicy that throws std::out_of_range
    // Throws an exception when an error occurs
    template<typename T>
    struct ErrorThrow{
        [[noreturn]] void handle_error(const char* msg) const {
            throw std::out_of_range(msg);
        }
    };

    // RAII for safety LockPolicy for many objects
    template<typename ...Lps>
    class LockGuard{
        private:
        std::tuple<Lps&...> locks_;
        // Pack_helper
        template<std::size_t ...Ind>
        void unlockAll(std::index_sequence<Ind...>){
            (std::get<Ind>(locks_).unlock(), ...);
        }
        public:
        explicit LockGuard(Lps&... politics) : locks_(politics...){
            (politics.lock(), ...);
        }

        ~LockGuard(){
            unlockAll(std::make_index_sequence<sizeof...(Lps)
                /*or std::tuple_size_v(decltype(locks_))*/>());
        }

        LockGuard(const LockGuard&) = delete;
        LockGuard& operator=(const LockGuard&) = delete;
        LockGuard(LockGuard&&) = delete;
        LockGuard& operator=(LockGuard&&) = delete;

    };
    /* Usage example
    auto& lk1 = static_cast<LockPolicy<T>&>(*this);
    auto& lk2 = static_cast<LockPolicy2<T>&>(other);
    LockGuard guard(lk1, lk2);
    */

    // Template for policy-based container-vector
    template<
    typename T,
    template<class> class AllocatePolicy,
    template<class> class LockPolicy,
    template<class> class ErrorPolicy
    >
    class PolicyVector :
        private AllocatePolicy<T>,
        private LockPolicy<T>,
        private ErrorPolicy<T> 
    {
        public:
        // Default constuctor
        PolicyVector() = default;

        // Destructor
        ~PolicyVector(){
            for(std::size_t i = 0; i < size_; i++){
                data_[i].~T();
            }
            AllocatePolicy<T>::deallocate(data_, capacity_);
        }

        // Move-Constructor
        PolicyVector(PolicyVector&& source) : size_(source.size_),
            capacity_(source.capacity_), data_(source.data_){
                auto& lk = static_cast<LockPolicy<T>&>(source);
                LockGuard<LockPolicy<T>> guard(lk);
                source.data_ = nullptr;
                source.capacity_ = source.size_ = 0;
            }

        // Move-operator
        PolicyVector& operator=(PolicyVector&& source){
            if (this != &source){
                source.LockPolicy<T>::lock();
                this->lock();

                // Free old memory
                for (std::size_t i = 0; i < size_; i++){
                    data_[i].~T();
                }
                AllocatePolicy<T>::deallocate(data_, capacity_);

                data_ = source.data_;
                capacity_ = source.capacity_;
                size_ = source.size_;
                source.data_ = nullptr;
                source.capacity_ = source.size_ = 0;
                this->unlock();
                source.LockPolicy<T>::unlock();
            }
            return *this;
        }

        // Push_back functon
        // Add value to the and of vector
        void push_back(const T& value){
            auto& lk = static_cast<LockPolicy<T>&>(*this);
            LockGuard<LockPolicy<T>> guard(lk);
            if (size_ >= capacity_){
                // Standart geometric growth for amortize complexity
                std::size_t newCap = capacity_ ? capacity_ * 2 : 1;
                T* newData = static_cast<T*>(AllocatePolicy<T>::allocate(newCap));
                for (std::size_t i = 0; i < size_; i++){
                    new(&newData[i]) T(std::move(data_[i]));
                    data_[i].~T();
                }
                // Free old memory
                AllocatePolicy<T>::deallocate(data_, capacity_);
                data_ = newData;
                capacity_ = newCap;
            }
            new(&data_[size_]) T(value);
            size_++;
        }
        
        // Pop_back function
        // Delete the end element of vecctor
        void pop_back(){
            auto& lk = static_cast<LockPolicy<T>&>(*this);
            LockGuard<LockPolicy<T>> guard(lk);
            if (size_ == 0){
                ErrorPolicy<T>::handle_error("pop_back the null-size vector");
            }
            data_[size_-1].~T();
            size_--;
        }

        // At function
        //Safety access the element of vector by its vector
        T& at(std::size_t idx){
            auto& lk = static_cast<LockPolicy<T>&>(*this);
            LockGuard<LockPolicy<T>> guard(lk);
            if (idx >= size_){
                ErrorPolicy<T>::handle_error("index out of range");
            }
            return data_[idx];
        }

        // Const At function
        const T& at(std::size_t idx) const{
            auto& lk = static_cast<const LockPolicy<T>&>(*this);
            LockGuard<LockPolicy<T>> guard(lk);
            if (idx >= size_){
                static_cast<const ErrorPolicy<T>&>(*this).hedle_error("index out of range");
            }
            return static_cast<const T&>(data_[idx]);
        }

        std::size_t size() noexcept { return size_; }
        std::size_t capacity() noexcept { return capacity_; }

        private:
        T* data_ = nullptr;
        std::size_t size_ = 0;
        std::size_t capacity_ = 0;
    };
}