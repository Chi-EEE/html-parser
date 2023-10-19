#ifndef MARCOS_H
#define MARCOS_H

#ifdef USE_BOOST
#include <boost/regex.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/trim.hpp>
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

inline std::string to_lower(const std::string &text)
{
    std::string clonedText(text);
#ifdef USE_BOOST
    boost::to_lower(clonedText);
#else
    std::transform(clonedText.begin(), clonedText.end(), clonedText.begin(), ::tolower);
#endif
    return clonedText;
}

inline bool starts_with(const std::string &text, const std::string &prefix)
{
#ifdef USE_BOOST
    return boost::starts_with(text, prefix);
#else
    return text.rfind(prefix, 0) == 0;
#endif
}

inline bool ends_with(const std::string &text, const std::string &suffix)
{
#ifdef USE_BOOST
    return boost::ends_with(text, suffix);
#else
    return (0 == text.compare(text.length() - suffix.length(), suffix.length(), suffix));
#endif
}

static inline std::string trim(const std::string &text)
{
    std::string clonedText(text);
#ifdef USE_BOOST
    boost::trim_left(clonedText);
#else
    clonedText.erase(clonedText.begin(), std::find_if(clonedText.begin(), clonedText.end(), [](unsigned char ch)
                                                      { return !std::isspace(ch); }));
#endif
    return clonedText;
}

#endif // MARCOS_H