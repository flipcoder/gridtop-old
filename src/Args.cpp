#include "Args.h"

void Args :: analyze()
{
    for(const auto& arg: m_Args)
    {
        if(boost::starts_with(arg, "--"))
        {
            if(arg.length()==2)
                continue; // -- as separator
            
            size_t idx = arg.find('=');
            if(idx != std::string::npos)
            {
                if(idx != arg.rfind('='))
                    throw std::exception(); // more than one '='
                std::string key = arg.substr(2, idx-2);
                if(m_Values.find(key) != m_Values.end())
                    throw std::exception(); // duplicate
                std::string value = arg.substr(idx+1);
                if(!value.empty())
                    if(!m_Values.insert({key,value}).second)
                        throw std::exception(); // failed to insert
            }
        }else if(boost::starts_with(arg, "-")){
            // TODO: expand to -- equivalent?
        }else{
            m_Other.push_back(arg);
        }
    }
}

