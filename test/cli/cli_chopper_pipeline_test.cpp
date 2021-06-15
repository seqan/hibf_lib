#include <fstream>
#include <seqan3/std/ranges>     // range comparisons
#include <string>                // strings
#include <vector>                // vectors

#include <cereal/archives/binary.hpp>

#include <seqan3/search/dream_index/interleaved_bloom_filter.hpp>
#include <seqan3/test/tmp_filename.hpp>
#include <seqan3/utility/views/join_with.hpp>
#include <seqan3/utility/views/to.hpp>

#include <hibf/detail_bin_prefixes.hpp>

#include "cli_test.hpp"

// check if each chopper submodule can work with the output of the other
TEST_F(cli_test, chopper_pipeline)
{
    // CHOPPER COUNT
    // =========================================================================
    std::string seq_filename = data("small.fa");
    seqan3::test::tmp_filename const taxa_filename{"data.tsv"};

    // we need to have tax ids from the user
    {
        std::ofstream fout{taxa_filename.get_path()};
        fout << seq_filename << '\t' << "TAX1\n"
             << seq_filename << '\t' << "TAX2\n"
             << seq_filename << '\t' << "TAX2\n"
             << seq_filename << '\t' << "TAX3\n";
    }

    cli_test_result count_result = execute_app("chopper", "count",
                                               "-k", "15",
                                               "-w", "25",
                                               "-t", "2",
                                               "-c", "2",
                                               "-f", taxa_filename.get_path().c_str());

    std::vector<std::string> expected_components
    {
        seq_filename + "\t95\tTAX3",
        seq_filename + ";" + seq_filename + "\t95\tTAX2",
        seq_filename + "\t95\tTAX1"
    };

    size_t line_count{};
    for (auto && line : count_result.out | std::views::split('\n') | seqan3::views::to<std::vector<std::string>>)
    {
        EXPECT_TRUE(std::ranges::find(expected_components, line) != expected_components.end());
        ++line_count;
    }

    EXPECT_EQ(expected_components.size(), line_count);

    // Write count_result to output file (user would pipe the output)
    seqan3::test::tmp_filename const count_filename{"data.tsv"};

    {
        std::ofstream fout{count_filename.get_path()};
        fout << (expected_components | seqan3::views::join_with(std::string{'\n'}) | seqan3::views::to<std::string>);
    }
}
