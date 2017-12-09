#include "job.hpp"

//-----------------------------
job::job(job_function function)
:   m_function {function}
,   m_parent {nullptr}
,   m_unfinished_jobs {1}
,   m_padding {}
{
}

//-----------------------------
job::job(job_function function, job *parent) 
:   m_function {function}
,   m_parent {parent}
,   m_unfinished_jobs {1}
,   m_padding {}
{
    if (parent) {
        ++parent->m_unfinished_jobs;
    }
}

//-----------------------------
void 
job::run() {
    m_function(this, m_padding.data());
    finish();
}

//-----------------------------
bool job::finished() {
    return m_unfinished_jobs == 0;
}

//-----------------------------
void job::finish() {
   if (finished()) {
        if (!m_parent) {
            m_parent->finish();
        }
   }
}

