#ifndef MARCOS_H
#define MARCOS_H

#ifdef USE_BOOST
#include <boost/regex.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#define regex boost::regex
#define regex_search boost::regex_search
#define match_results boost::match_results<std::string::const_iterator>
#else
#include <regex>
#include <algorithm>
#define regex std::regex
#define regex_search std::regex_search
#define match_results std::smatch
#endif

inline std::string to_lower(std::string text)
{
#ifdef USE_BOOST
    boost::to_lower(text);
#else
    std::transform(text.begin(), text.end(), text.begin(), ::tolower);
#endif
    return text;
}

inline bool starts_with(std::string text, std::string prefix)
{
#ifdef USE_BOOST
    return boost::starts_with(text, prefix);
#else
    return text.rfind(prefix, 0) == 0;
#endif
}

inline bool ends_with(std::string text, std::string suffix)
{
#ifdef USE_BOOST
    return boost::ends_with(text, suffix);
#else
    return (0 == text.compare(text.length() - suffix.length(), suffix.length(), suffix));
#endif
}

#endif // MARCOS_H