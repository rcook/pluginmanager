#include "catch.hpp"
#include "utillib/Trie.h"

using namespace std;

TEST_CASE("Trie", "Trie")
{
    SECTION("basics")
    {
        Trie<char, string> trie;

        REQUIRE(!trie.find(""));
        REQUIRE(trie.insert(""));
        REQUIRE(!trie.insert(""));
        REQUIRE(trie.find(""));

        REQUIRE(trie.insert("hello"));
        REQUIRE(!trie.insert("hello"));
        REQUIRE(trie.insert("balloon"));
        REQUIRE(!trie.insert("balloon"));
        REQUIRE(trie.insert("ballooniness"));
        REQUIRE(!trie.insert("ballooniness"));
        REQUIRE(trie.insert("ball"));
        REQUIRE(!trie.insert("ball"));

        REQUIRE(!trie.find("hell"));
        REQUIRE(trie.find("hello"));
        REQUIRE(!trie.find("helloo"));
        REQUIRE(trie.find("balloon"));
        REQUIRE(trie.find("ballooniness"));
        REQUIRE(trie.find("ball"));

        REQUIRE(trie.findByPrefix("hell")->compare("hello") == 0);
        REQUIRE(trie.findByPrefix("hello")->compare("hello") == 0);
        REQUIRE(!trie.findByPrefix("helloo"));
        REQUIRE(!trie.findByPrefix("bal"));
        REQUIRE(trie.findByPrefix("ball")->compare("ball") == 0);
        REQUIRE(!trie.findByPrefix("ballo"));
        REQUIRE(trie.findByPrefix("balloon")->compare("balloon") == 0);
        REQUIRE(trie.findByPrefix("ballooni")->compare("ballooniness") == 0);
        REQUIRE(trie.findByPrefix("ballooniness")->compare("ballooniness") == 0);
        REQUIRE(!trie.findByPrefix("ballooninessy"));
    }
}
