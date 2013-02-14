#ifndef UTIL_H_SNIHUCWC
#define UTIL_H_SNIHUCWC

#include <memory>
#include <vector>
#include <deque>
#include <string>
#include <chrono>
#include <thread>
#include <boost/algorithm/string.hpp>

#define ENTIRE(blah) blah.begin(), blah.end()

namespace util
{
    constexpr uint32_t bit(uint32_t x) {
        return 1<<x;
    }
    constexpr uint32_t mask(uint32_t x) {
        return (1<<x) - 1;
    }

    template<class Container, class Object>
    bool has(const Container& c, const Object& o)
    {
        return std::find(ENTIRE(c), o) != c.end();
    }

    template<class Container, class Pred>
    void remove_if(Container& c, Pred& p) {
        c.erase(std::remove_if(ENTIRE(c), p), c.end());
    }

    template<class Container, class Object>
    void remove(Container& c, const Object& o)
    {
        c.erase(std::remove(ENTIRE(c), o), c.end());
    }

    template<class T, class... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template <class T>
    std::unique_ptr<T> clone_unique(T const* t)
    {
        return std::shared_ptr<T>(t->clone());
    }

    template <class T>
    std::shared_ptr<T> clone_shared(T const* t)
    {
        return std::shared_ptr<T>(t->clone());
    }

    inline std::vector<std::string> lines(const std::string& s)
    {
        std::vector<std::string> v;
        return boost::algorithm::split(v, s, boost::is_any_of("\r\n"));
    }
    //std::string lines(const std::vector<std::string>& s, std::string sep="")
    //{
    //    return boost::algorithm::join(s, "\n");
    //}


    class null_ptr_exception:
        public std::runtime_error
    {
        public:
            null_ptr_exception():
                std::runtime_error("null pointer exception")
            {}
    };

    template<class T>
    T safe_ptr(T ptr)
    {
        if(ptr == nullptr)
            throw null_ptr_exception();
        return ptr;
    }

    template <class T>
    class scoped_dtor {
        private:
            T* m_object;
        public:
            scoped_dtor(T* o):
                m_object(o)
            {}

            ~scoped_dtor(){
                call();
            }

            void call() {
                if(m_object)
                    m_object->~T();
            }

            void resolve() {
                m_object = nullptr;
            }

            void now() {
                call();
                resolve();
            }
    };

    template <class Time>
    bool retry(int count, Time delay, std::function<bool()> func)
    {
        for(int i=0; i<count; i==-1 || ++i)
        {
            if(func())
                return true;
            std::this_thread::sleep_for(delay);
        }
        return false;
    }

    // only thread-safe in c++11
    template<class T>
    class singleton
    {
        public:
            static T& get()
            {
                static T instance;
                return instance;
            }
    };
}


#endif

