#ifndef _MENCI_HTML_PARSER_ARGUMENTPARSER_H
#define _MENCI_HTML_PARSER_ARGUMENTPARSER_H

#include <cassert>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <functional>
#include <optional>

#include "macros.h"

namespace html_parser {
	class ArgumentParser
	{
	public:
		using ParserFunction = std::function<std::optional<std::string>(const std::string&)>;

		template <typename T>
		using ValidatorFunction = std::function<std::optional<std::string>(const T&)>;

	private:
		struct Argument
		{
			std::string name, alias, valueName;
			std::string description;
			bool optional, specfied;
			std::string defaultValue;
			ParserFunction parserFunction;
		};

		size_t argc;
		char** argv;

		std::unordered_map<std::string, std::shared_ptr<Argument>> mapNameArgument;
		std::unordered_map<std::string, std::shared_ptr<Argument>> mapAliasArgument;
		std::vector<std::shared_ptr<Argument>> options;
		std::vector<Argument> positionalArguments;

		std::string programDescription;

	public:
		ArgumentParser(int argc, char** argv) : argc((size_t)argc), argv(argv) {}

		template <typename IntType = size_t>
		static ParserFunction integerParser(IntType& var,
			ValidatorFunction<IntType> validator = nullptr,
			std::function<IntType(const std::string&, size_t*, int)> convertFunction = (IntType(*)(const std::string&, size_t*, int))std::stoul)
		{
			return [=, &var](const std::string& str) -> std::optional<std::string>
				{
					try
					{
						IntType val = convertFunction(str, nullptr, 10);
						if (validator)
						{
							std::optional<std::string> errorMessage = validator(val);
							if (errorMessage)
								return errorMessage;
						}

						var = val;
						return std::nullopt;
					}
					catch (const std::exception& e)
					{
						return "Not a valid integer: " + str;
					}
				};
		}

		template <typename StringType = std::string>
		static ParserFunction stringParser(StringType& var,
			std::function<std::optional<std::string>(const std::string&)> validator = nullptr)
		{
			return [=, &var](const std::string& str) -> std::optional<std::string>
				{
					if (validator)
					{
						std::optional<std::string> errorMessage = validator(str);
						if (errorMessage)
							return errorMessage;
					}

					var = str;
					return std::nullopt;
				};
		}

		ArgumentParser& setProgramDescription(const std::string& programDescription);
		ArgumentParser& addOption(const std::string& name,
			const std::string& alias,
			const std::string& valueName,
			const std::string& description,
			ParserFunction parserFunction = nullptr,
			bool optional = false,
			const std::string& defaultValue = "");
		ArgumentParser& addPositional(const std::string& valueName,
			const std::string& description,
			ParserFunction parserFunction = nullptr,
			bool optional = false,
			const std::string& defaultValue = "");
		ArgumentParser& showHelp();
		ArgumentParser& parse();
	};
}

#endif // _MENCI_HTML_PARSER_ARGUMENTPARSER_H
