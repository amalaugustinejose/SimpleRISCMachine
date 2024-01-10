#include <unordered_map>
#include <cstdint>

struct LRUInfo {
    int offset;
    uint8_t data;
    bool cacheReplaced = false;
    bool isCacheMiss = false;   
};

class LRUCache {
    class node {
        public:
        int offset;
        uint8_t data;
        node * next;
        node * prev;
        node(int _offset, int _data) {
            offset = _offset;
            data = _data;
        }
    };

    node * head;
    node * tail;

    int cap;
    std::unordered_map < int, node * > map;
    
    void addnode(node * newnode);
    void deletenode(node * delnode);   
public:

    LRUCache(int capacity);
    ~LRUCache();

    LRUInfo get(int offset);    
    LRUInfo put(int offset, uint8_t data);
};