//
// Created by Garanyan Ovanes (Ovanes-x) on 21.12.2019.
//

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <set>
#include <queue>

class TrieNode
{
public:

    //typedef btree::btree_map <char, TrieNode *> LinksMap;
    //typedef std::unordered_map <char, TrieNode *> LinksMap;
    typedef std::map<char, TrieNode *> LinksMap;
    LinksMap links;

    TrieNode *fail;  // Предыдущее состояние для функции отката. Только для root равно NULL.
    TrieNode *term; // Ближайшее терминальное состояние. Если отстутствует - NULL
    int out;

    explicit TrieNode(TrieNode *fail_node = nullptr)
    {
        fail = fail_node;
        term = nullptr;
        out = -1;
    }

    TrieNode *getLink(char c) const
    {
        LinksMap::const_iterator iter = links.find(c);
        if (iter != links.end())
        {
            //auto pos = visitedLinks.find(iter->second);
            //if (!pos->second)
            return iter->second;
        }

        return nullptr;
    }

    bool isTerminal() const
    {
        return (out >= 0);
    }
};


class AhoCorasick
{
public:

    struct WordStruct
    {
        int len;
        int align;
    };

    std::map<std::string::iterator, std::set<TrieNode *>> visitedStateIterators;

    ///
    AhoCorasick();

    ///
    ~AhoCorasick();

    ///
    void clean();

    ///
    void addString(const std::string &pattern, size_t align);

    ///
    void init();

    ///
    void search(std::string &text, std::map<size_t, size_t> &voting, std::map<std::string, size_t> &pattern_to_shift,
                std::string::iterator beginIt, TrieNode *state);


private:

    //
    bool step(char c);

    //
    void
    printTermsForCurrentState(std::map<size_t, size_t> &voting, int pos);

    //
    static void addEntity(const AhoCorasick::WordStruct &wordStruct, std::map<size_t, size_t> &voting, int pos);

    std::shared_ptr<TrieNode> _root;
    std::vector<WordStruct> _words;
    TrieNode *_current_state;
    std::shared_ptr<std::set<TrieNode *>> _nodesToDelete;

};


AhoCorasick::AhoCorasick()
{
    _nodesToDelete = std::shared_ptr<std::set<TrieNode *>>(new std::set<TrieNode *>());
    _root = std::shared_ptr<TrieNode>(new TrieNode());
}

void AhoCorasick::clean()
{
    for (auto it : *_nodesToDelete)
    {
        delete it;
    }
}

AhoCorasick::~AhoCorasick()
{
    clean();
}

bool AhoCorasick::step(char c)
{
    while (_current_state)
    {
        TrieNode *candidate = _current_state->getLink(c);

        if (candidate)
        {
            _current_state = candidate;
            return true;
        }
        _current_state = _current_state->fail;
    }
    _current_state = _root.get();
    return false;
}

void AhoCorasick::addEntity(const AhoCorasick::WordStruct &wordStruct, std::map<size_t, size_t> &voting, int pos)
{
    size_t found_pos = pos + 1 - wordStruct.len;
    if (found_pos >= wordStruct.align)
    {
        if (voting.find(found_pos - wordStruct.align) == voting.end())
            voting.emplace(found_pos - wordStruct.align, 1);
        else
            ++voting[found_pos - wordStruct.align];
    }
}

void AhoCorasick::addString(const std::string &pattern, size_t align)
{
    TrieNode *current_node = _root.get();
    for (auto it = pattern.begin(); it != pattern.end(); ++it)
    {
        TrieNode *child_node = current_node->getLink(*it);
        if (!child_node)
        {
            child_node = new TrieNode(_root.get());
            current_node->links[*it] = child_node;
            _nodesToDelete->insert(child_node);
        }
        current_node = child_node;
    }
    current_node->out = _words.size();

    WordStruct newWordStruct;
    newWordStruct.align = align;
    newWordStruct.len = pattern.size();

    _words.push_back(newWordStruct);
}

void AhoCorasick::init()
{
    std::queue<TrieNode *> q;
    q.push(_root.get());
    while (!q.empty())
    {
        TrieNode *current_node = q.front();
        q.pop();
        for (TrieNode::LinksMap::const_iterator iter = current_node->links.begin();
             iter != current_node->links.end(); ++iter)
        {
            char bitStructure = iter->first;
            TrieNode *child = iter->second;


            TrieNode *temp_node = current_node->fail;
            while (temp_node)
            {
                TrieNode *fail_candidate = temp_node->getLink(bitStructure);
                if (fail_candidate)
                {
                    child->fail = fail_candidate;
                    break;
                }
                temp_node = temp_node->fail;
            }


            if (child->fail->isTerminal())
            {
                child->term = child->fail;
            } else
            {
                child->term = child->fail->term;
            }
            q.push(child);
        }
    }
}

void AhoCorasick::printTermsForCurrentState(std::map<size_t, size_t> &voting, int pos)
{
    if (_current_state->isTerminal())
    {
        addEntity(_words[_current_state->out], voting, pos);
    }
    TrieNode *temp_node = _current_state->term;
    while (temp_node)
    {
        addEntity(_words[temp_node->out], voting, pos);
        temp_node = temp_node->term;
    }

}

void AhoCorasick::search(std::string &text, std::map<size_t, size_t> &voting,
                         std::map<std::string, size_t> &pattern_to_shift, std::string::iterator beginIt,
                         TrieNode *state)
{
    if (state == nullptr)
        _current_state = _root.get();
    else
        _current_state = state;

    for (auto it = beginIt; it != text.end(); it++)
    {

        if (_current_state == nullptr)
            _current_state = _root.get();

        auto pos = visitedStateIterators.find(it);

        if ((pos != visitedStateIterators.end()) && (pos->second.find(_current_state) != pos->second.end()))
        {
            _current_state = _root.get();
            continue;
        }

        step(*it);

        auto posVisited = visitedStateIterators.find(it);
        if ((posVisited == visitedStateIterators.end()) ||
            (posVisited->second.find(_current_state) == posVisited->second.end()))
        {
            printTermsForCurrentState(voting, it - beginIt);
        } else
        {
            _current_state = _root.get();
        }
        visitedStateIterators[it].insert(_current_state);

    }
    //
    _current_state = state;

}

void splitPatterns(const std::string &patterns, std::map<std::string, size_t> &pattern_to_shift)
{
    size_t prev_punct = 0;
    size_t found = patterns.find_first_of('?');
    while (found != std::string::npos)
    {
        pattern_to_shift[patterns.substr(prev_punct, found - prev_punct)] = prev_punct;
        prev_punct = found + 1;
        found = patterns.find_first_of('?', prev_punct);

    }

    pattern_to_shift[patterns.substr(prev_punct, found - prev_punct)] = prev_punct;
}

void printSearchSubstr(const std::string &patterns, std::string &text)
{
    std::map<size_t, size_t> patterns_voting;
    std::map<std::string, size_t> shift_to_pattern;
    splitPatterns(patterns, shift_to_pattern);

    AhoCorasick ahoCorasick;
    size_t patterns_num = 0;
    for (const auto &pattern: shift_to_pattern)
    {
        if (!pattern.first.empty())
        {
            ahoCorasick.addString(pattern.first, pattern.second);
            ++patterns_num;
        }
    }

    ahoCorasick.init();

    ahoCorasick.search(text, patterns_voting, shift_to_pattern, text.begin(), nullptr);
    for (auto vote :patterns_voting)
    {
        if (vote.second == patterns_num &&
            vote.first + patterns.size() <= text.size())
        {
            std::cout << vote.first << std::endl;
        }
    }

}

//ab??aba
//ababacaba

//??a??
//qwertypouaaj
int main()
{
    std::string patterns, text;
    std::cin >> patterns >> text;

    printSearchSubstr(patterns, text);
    return 0;
}