#ifndef JOB_HPP
#define JOB_HPP

#include <atomic>
#include <cstdint>
#include <array>
#include <type_traits> // std::is_pod<>, std::enable_if
#include <cstring>

class job final {
public:

    using job_function = void (*)(job *parent, void const *data);
    
    job() = delete;
    
    job(job_function function);
    job(job_function function, job *parent);
   
    template<typename Data>
    job(job_function function, Data const &data)
    :   m_function{function}
    ,   m_parent{nullptr}
    ,   m_unfinished_jobs{1}
    ,   m_padding{} {
        
        set_data(data);
    }

    template<typename Data>
    job(job_function function, job *parent, Data const &data) 
    :   m_function {function}
    ,   m_parent {parent}
    ,   m_unfinished_jobs {1}
    ,   m_padding {} {
        if (parent) {
            parent->m_unfinished_jobs++;
        }
        
        set_data(data);
    }
    
    template<typename Data,
             typename = std::enable_if_t<std::is_pod<Data>::value && sizeof(Data) <= 44>>
    void set_data(Data const &data) {
        std::memcpy(m_padding.data(), &data, sizeof(Data));
    }

    template<typename Data>
    Data const & get_data() const {
        return reinterpret_cast<Data const*>(m_padding.data());
    }

    void run(); 
    bool finished();

private:
    void finish();
    
private: 
    job_function m_function;
    job *m_parent;
    std::atomic<std::uint32_t> m_unfinished_jobs;
    
    // 64 bits it the cache line size here 
    // making jobs the size of a cache line to avoid
    // false sharing
                                        
    std::array<std::uint8_t, 44> m_padding; 
};

#endif // JOB_HPP
