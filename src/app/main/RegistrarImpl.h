#pragma once

#include "Op.h"
#include "interfacelib/Registrar.h"
#include "utillib/Trie.h"
#include <unordered_map>

class RegistrarImpl : public Registrar
{
public:
    RegistrarImpl() = default;

public:
    void registerOp(const std::string& name, OpFunc func) override;
    const std::unordered_map<std::string, Op>& ops() const { return m_ops; }
    int invoke(const std::string& command, int x, int y) const;

private:
    Trie<char, std::string> m_names;
    std::unordered_map<std::string, Op> m_ops;
};
