#ifndef _ARGS_H_59ROLBDK
#define _ARGS_H_59ROLBDK

#include <vector>
#include <map>
#include <string>
#include "Util.h"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <cassert>

class Args
{
    public:
        Args(const std::vector<std::string>& args):
            m_Args(args)
        {
            analyze();
        }

        Args(std::vector<std::string>&& args):
            m_Args(args)
        {
            analyze();
        }

        Args(const std::string& lines):
            m_Args(util::lines(lines))
        {
            analyze();
        }

        Args(int argc, const char** argv):
            m_Args(argv+1, argv+argc)
        {
            analyze();
        }

        std::vector<std::string>& get() { return m_Args; }
        const std::vector<std::string>& get() const { return m_Args; };

        size_t size() const { return m_Args.size(); }

        bool empty() const { return m_Args.empty(); }

        bool has(const std::string& s) const {
            return util::has(m_Args, s);
        }

        std::vector<std::string> get_matches(boost::regex expr) const {
            std::vector<std::string> matches;
            for(const auto& s: m_Args)
                if(boost::regex_match(s, expr))
                    matches.push_back(s);
            return matches;
        }

        //std::vector<std::tuple<std::string, boost::smatch>> get_match_groups(boost::regex expr) const {
        //    std::vector<std::tuple<std::string, boost::smatch>> matches;
        //    boost::smatch results;
        //    for(const auto& s: m_Args)
        //        if(boost::regex_match(s, results, expr))
        //            matches.push_back(std::tuple<std::string, boost::smatch>(s, results));
        //    return matches;
        //}


        size_t num_matches(boost::regex expr) const {
            size_t count = 0;
            for(const auto& s: m_Args) {
                if(boost::regex_match(s, expr))
                    ++count;
            }
            return count;
        }

        std::string value(std::string key) const {
            try{
                return m_Values.at(key);
            }catch(const std::out_of_range&){
                return std::string();
            }
        }

        std::string data() const {
            return boost::algorithm::join(m_Args, "\n");
        }

        // TODO: tokenize based on arg separators
        // std::vector<Args> separate() const {
        // }

        std::string get(unsigned int pos) {}
        std::string after(unsigned int pos, int offset) {}
        std::string before(unsigned int pos, int offset) {}
        std::string after(std::string s, int offset) {}
        std::string before(std::string s, int offset) {}
        unsigned int find(std::string s) {}

    private:

        void analyze();

        std::vector<std::string> m_Args;
        //std::vector<std::pair<std::string, std::string>> m_Values;
        std::map<std::string, std::string> m_Values;
};

#endif

