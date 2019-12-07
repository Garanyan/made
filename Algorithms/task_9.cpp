//
// Created by ovanes on 30.11.2019.
//

#include "Huffman.h"
#include <queue>
#include <stack>

#include <list>
#include <iostream>
#include <vector>
#include <cassert>

class BitsWriter
{
public:
    void WriteBit(bool bit);

    void WriteByte(unsigned char byte);

    std::vector<unsigned char> GetResult();

private:
    std::vector<unsigned char> buffer_;
    unsigned char accumulator_ = 0;
    int bits_count_ = 0;
};

void BitsWriter::WriteBit(bool bit)
{
    // Ставим бит в аккумулятор на нужное место
    accumulator_ |= static_cast<unsigned char>(bit) << bits_count_;
    ++bits_count_;
    if (bits_count_ == 8)
    {
        bits_count_ = 0;
        buffer_.push_back(accumulator_);
        accumulator_ = 0;
    }
}

void BitsWriter::WriteByte(unsigned char byte)
{
    if (bits_count_ == 0)
    {
        buffer_.push_back(byte);
    } else
    {
        accumulator_ |= byte << bits_count_;
        buffer_.push_back(accumulator_);
        accumulator_ = byte >> (8 - bits_count_);
    }
}

std::vector<unsigned char> BitsWriter::GetResult()
{
    if (bits_count_ != 0)
    {
        // Добавляем в буфер аккумулятор, если в нем что-то есть.
        buffer_.push_back(accumulator_);
    }
    buffer_.push_back(static_cast<unsigned char>(bits_count_));
    return std::move(buffer_);
}

class BitsReader
{
public:
    bool ReadBit(bool &val);

    bool ReadByte(unsigned char &val);

    void SetData(std::vector<unsigned char> data_);

    void cleanAccumulator();

    size_t getSize();

private:
//    std::vector<unsigned char> buffer_;
//    std::list<unsigned char> buffer_;
    std::deque<unsigned char> buffer_;
    unsigned char accumulator_ = 0;
    int bits_count_ = 8;
};


struct Node
{
    byte value;
    Node *leftChild = nullptr;
    size_t weight = 0;
    Node *rightChild = nullptr;
    Node *parent = nullptr;

    explicit Node(byte val_, size_t weight_) : value(val_), weight(
            weight_)
    {}

    Node(byte val, size_t w_, Node *lc, Node *rc, Node *parent_ = nullptr)
    {
        value = val;
        leftChild = lc;
        rightChild = rc;
        parent = parent_;
        weight = w_;
    }

    inline bool operator<(const Node &rhs)
    { return this->weight < rhs.weight; };

    inline bool operator>(const Node &rhs)
    { return this->weight > rhs.weight; }

    inline bool operator<=(const Node &rhs)
    { return !(operator>(rhs)); }

    inline bool operator>=(const Node &rhs)
    { return !(operator<(rhs)); }

    bool isLeaf()
    {
        return leftChild == nullptr;
    }

};

void EncodeNode(Node *node, BitsWriter &bits_writer)
{
    if (node->isLeaf())
    {
        bits_writer.WriteBit(1);
        bits_writer.WriteByte(node->value);
    } else
    {
        bits_writer.WriteBit(0);
        EncodeNode(node->leftChild, bits_writer);
        EncodeNode(node->rightChild, bits_writer);
    }
}


void BitsReader::SetData(std::vector<unsigned char> data_)
{
    std::deque<unsigned char> words3(data_.begin(), data_.end());
    buffer_ = words3;
}

bool BitsReader::ReadBit(bool &val)
{
    if (bits_count_ == 8 && !buffer_.empty())
    {
        accumulator_ = buffer_[0];
        buffer_.pop_front();
        bits_count_ = 0;
    } else if ((bits_count_ == 0 || bits_count_ == 8) && buffer_.empty())
    {
        return false;
    }

    bool result = ((accumulator_ >> bits_count_) & 1);
    bits_count_++;
    val = result;
    return true;
}

bool BitsReader::ReadByte(unsigned char &val)
{
    unsigned char res = 0;
    for (size_t i = 0; i < 8; ++i)
    {
        bool bit;
        if (!ReadBit(bit))
            return false;
        res |= bit << i;
    }
    val = res;
    return true;
}

void BitsReader::cleanAccumulator()
{
    if (bits_count_ != 0 && bits_count_ != 8)
    {
        accumulator_ = buffer_.front();
        buffer_.erase(buffer_.begin());
        bits_count_ = 0;
    }
}

size_t BitsReader::getSize()
{
    return buffer_.size();
}

class HuffmanTree
{
public:
    HuffmanTree(std::size_t val)
    {
        tree = std::vector<Node *>(val, nullptr);
    };

    HuffmanTree()
    {};

    void build(const std::vector<int> &freqs);

    void encode(byte symbol, BitsWriter &output) const;

    void EncodeTree(BitsWriter &bits_writer);

    void DecodeTree(BitsReader &bits_reader);

    bool getLeaf(std::vector<bool> &path, byte &result);

    void ReadNode(BitsReader &bits_reader, Node *parent);

    ~HuffmanTree();

private:

    std::vector<Node *> tree;

    Node *root = nullptr;
};

class NodePtrComp
{
public:
    bool operator()(Node *&lhs, Node *&rhs) const
    {
        return *lhs > *rhs;
    }
};

void HuffmanTree::build(const std::vector<int> &freqs)
{
    std::priority_queue<Node *, std::vector<Node *>, NodePtrComp> pq;

    for (int i = 0; i < 256; i++)
    {
        if (freqs[i] > 0)
        {
            Node *nodep = new Node(i, freqs[i]);
            tree[i] = nodep;
            pq.push(tree[i]);
        }
    }


    while (pq.size() > 1)
    {
        Node *p1 = pq.top();
        pq.pop();

        Node *p2 = pq.top();
        pq.pop();

        size_t total = p1->weight + p2->weight;
        Node *parent = new Node(0, total, p1, p2);
        p1->parent = parent;
        p2->parent = parent;
        pq.push(parent);
    }
    if (pq.size() == 1)
    {
        root = pq.top();
    }

}

HuffmanTree::~HuffmanTree()
{
    for (auto node: tree)
        delete node;
}


void HuffmanTree::ReadNode(BitsReader &bits_reader, Node *parent)
{
    bool read;
    if (bits_reader.ReadBit(read))
        if (read == 1)
        {
            byte val;
            bits_reader.ReadByte(val);
            tree.emplace_back(new Node(val, 0, nullptr, nullptr, parent));
        } else
        {
            auto res = new Node(0, 0, nullptr, nullptr, parent);
            ReadNode(bits_reader, res);
            Node *leftChild = tree.back();
            ReadNode(bits_reader, res);
            Node *rightChild = tree.back();

            res->leftChild = leftChild;
            res->rightChild = rightChild;
            tree.emplace_back(res);
        }
}


void HuffmanTree::encode(byte symbol, BitsWriter &output) const
{
    Node *curr = tree[symbol];
    std::stack<bool> treeHolder;
    while (curr != root)
    {
        if (curr == curr->parent->leftChild)
        {
            treeHolder.push(0);
        } else
        {
            treeHolder.push(1);
        }
        curr = curr->parent;
    }

    while (treeHolder.size() != 0)
    {
        output.WriteBit(treeHolder.top());
        treeHolder.pop();
    }
}

void HuffmanTree::EncodeTree(BitsWriter &bits_writer)
{
    EncodeNode(root, bits_writer);
}

void HuffmanTree::DecodeTree(BitsReader &bits_reader)
{

    ReadNode(bits_reader, nullptr);
    root = tree.back();
}

bool HuffmanTree::getLeaf(std::vector<bool> &path, byte &result)
{
    Node *current = root;
    for (size_t t = 0; t < path.size(); ++t)
    {
        if (!path[t])
        {
            current = current->leftChild;
        } else
        {
            current = current->rightChild;
        }
    }
    if (current->isLeaf())
    {
        result = current->value;
        return true;
    }

    return false;
}

void Encode(IInputStream &original, IOutputStream &compressed)
{
    BitsWriter input;
    std::vector<int> freqs(256, 0);
    byte value;
    int total_count = 0;
    while (original.Read(value))
    {
        input.WriteByte(value);
        freqs[value]++;
        ++total_count;
    }
    HuffmanTree encodeHuffmanTree(256);

    encodeHuffmanTree.build(freqs);

    BitsWriter output;
    BitsWriter tmpTree;

    encodeHuffmanTree.EncodeTree(tmpTree);
    auto output_result = input.GetResult();
    for (size_t i = 0; i < output_result.size() - 1; ++i)
    {
        encodeHuffmanTree.encode(output_result[i], output);
    }

    auto res = output.GetResult();

    auto tree_res = tmpTree.GetResult();
    res.insert(res.begin(), tree_res.begin(), tree_res.end() - 1);
    for (size_t i = 0; i < res.size(); ++i)
    {
        compressed.Write(res[i]);
    }

}

void Decode(IInputStream &compressed, IOutputStream &original)
{

    std::vector<unsigned char> data;
    byte value;
    while (compressed.Read(value))
    {
        data.push_back(value);
    }
    int bits_count_last = (int) data.back();
    data.pop_back();
    BitsReader bits_reader;
    bits_reader.SetData(data);

    HuffmanTree decodeHuffmanTree;
    decodeHuffmanTree.DecodeTree(bits_reader);
    bits_reader.cleanAccumulator();

    int full_data_size = bits_reader.getSize() + 1;

    bool val;
    int counter = 0;
    std::vector<bool> path;
    while (bits_reader.ReadBit(val))
    {
        if (bits_count_last != 0 && counter > 8 * (full_data_size - 1) + bits_count_last)
        {

            break;
        }
        path.push_back(val);
        byte decoded;
        if (decodeHuffmanTree.getLeaf(path, decoded))
        {
            original.Write(decoded);
            path.clear();
        }
        counter++;
    }

}

int main()
{
    // Получаем данные, которые нужно закодировать
    vector<vector<byte> > input;

    fillInputs(input);

    // Сжимаем данные
    vector<vector<byte> > compressed;
    compressed.resize(input.size());
    for (unsigned int i = 0; i < input.size(); i++)
    {
        CInputStream iStream(input[i]);
        COutputStream oStream(compressed[i]);
        Encode(iStream, oStream);
    }

    // Распаковываем сжатые данные и проверяем, что они совпадают с оригиналом
    for (unsigned int i = 0; i < input.size(); i++)
    {
        vector<byte> output;
        CInputStream iStream(compressed[i]);
        COutputStream oStream(output);
        Decode(iStream, oStream);

        if (!isEqual(input[i], output))
        {
            cout << -1;
            return 0;
        }
    }

    // Вычисляем степень сжатия
    cout << (100. * calculateSize(compressed) / calculateSize(input));

    return 0;
}
