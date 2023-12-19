#include <functional>

#include "Arguments.h"
#include "ArgumentParser.h"

namespace html_parser {
    Arguments parseArguments(int argc, char* argv[])
    {
        Arguments arguments;
        ArgumentParser(argc, argv)
            .setProgramDescription("Parse HTML file to DOM tree.")
            .addPositional("file", "The HTML file to parse, '-' for stdin.",
                ArgumentParser::stringParser(arguments.inputFile),
                true, "-")
            .parse();
        return arguments;
    }
}
