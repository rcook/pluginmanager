#pragma once

#include <cassert>
#include <vector>

template<typename T, typename TSequence>
class Trie
{
public:
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;

public:
    ~Trie() = default;

    Trie()
        : m_value('\0')
        , m_hasWord(false)
    {
    }

    Trie(Trie&& other)
        : m_value(std::move(other.m_value))
        , m_hasWord(std::move(other.m_hasWord))
        , m_sequence(std::move(other.m_sequence))
        , m_nodes(std::move(other.m_nodes))
    {
    }

    bool insert(const TSequence& sequence)
    {
        return insertHelper(sequence, sequence.data(), sequence.size());
    }

    bool find(const TSequence& sequence) const
    {
        const auto* node = findHelper(sequence.data(), sequence.size());
        return node && node->m_hasWord;
    }

    const TSequence* findByPrefix(const TSequence& prefix) const
    {
        const auto& node = findByPrefixHelper(prefix.data(), prefix.size());
        return node ? &node->m_sequence : nullptr;
    }

private:
    Trie(const TSequence& sequence, const T* stem, size_t stemSize)
        : m_value(stem[0])
        , m_hasWord(stemSize == 1)
        , m_sequence(stemSize == 1 ? sequence : TSequence())
    {
        if (stemSize > 1)
        {
            m_nodes.emplace_back(Trie(sequence, stem + 1, stemSize - 1));
        }
    }

    bool insertHelper(const TSequence& sequence, const T* stem, size_t stemSize)
    {
        if (stemSize == 0)
        {
            if (m_hasWord)
            {
                return false;
            }

            m_hasWord = true;
            m_sequence = sequence;
            return true;
        }

        auto value = stem[0];
        for (auto& node : m_nodes)
        {
            if (node.m_value == value)
            {
                return node.insertHelper(sequence, stem + 1, stemSize - 1);
            }
        }

        m_nodes.emplace_back(Trie(sequence, stem, stemSize));
        return true;
    }

    const Trie* findHelper(const T* stem, size_t stemSize) const
    {
        if (stemSize == 0)
        {
            return this;
        }

        auto value = stem[0];
        for (const auto& node : m_nodes)
        {
            if (node.m_value == value)
            {
                return node.findHelper(stem + 1, stemSize - 1);
            }
        }

        return nullptr;
    }

    const Trie* findByPrefixHelper(const T* stem, size_t stemSize) const
    {
        if (stemSize == 0)
        {
            if (m_hasWord)
            {
                return this;
            }

            const auto* node = this;
            for (; ;)
            {
                auto nodeCount = node->m_nodes.size();

                if (nodeCount == 0)
                {
                    assert(node->m_hasWord);
                    return node;
                }

                if (nodeCount != 1 || node->m_hasWord)
                {
                    return nullptr;
                }

                node = &node->m_nodes.front();
            }
        }

        auto value = stem[0];
        for (const auto& node : m_nodes)
        {
            if (node.m_value == value)
            {
                return node.findByPrefixHelper(stem + 1, stemSize - 1);
            }
        }

        return nullptr;
    }

private:
    const T m_value;
    bool m_hasWord;
    TSequence m_sequence;
    std::vector<Trie> m_nodes;
};
