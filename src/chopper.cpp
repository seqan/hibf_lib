#include <seqan3/argument_parser/all.hpp>
#include <seqan3/core/debug_stream.hpp>

#include <hibf/build/chopper_build.hpp>
#include <hibf/count/chopper_count.hpp>
#include <hibf/search/chopper_search.hpp>

int main(int argc, const char *argv [])
{
    seqan3::argument_parser top_level_parser{"chopper", argc, argv, seqan3::update_notifications::off,
                                             {"count", "build", "search"}};
    top_level_parser.info.version = "1.0.0";

    try
    {
        top_level_parser.parse();
    }
    catch (seqan3::argument_parser_error const & ext) // the user did something wrong
    {
        std::cerr << "[CHOPPER ERROR] " << ext.what() << '\n'; // customize your error message
        return -1;
    }

    seqan3::argument_parser & sub_parser = top_level_parser.get_sub_parser(); // hold a reference to the sub_parser

    if (sub_parser.info.app_name == std::string_view{"chopper-count"})
        return chopper_count(sub_parser);
    else if (sub_parser.info.app_name == std::string_view{"chopper-build"})
        return chopper_build(sub_parser);
    else if (sub_parser.info.app_name == std::string_view{"chopper-search"})
        return chopper_search(sub_parser);

    return 0;
}
