#include "LRUCache.h"

LRUCache::LRUCache(int capacity) {
    cap = capacity;
    head = new node(0xff, 0xff);
    tail = new node(0xff, 0xff);
    head -> next = tail;
    tail -> prev = head;
}

LRUCache::~LRUCache() {
    while (head->next != tail) {
        node *cache_entry = head->next;
        head->next = cache_entry->next;
        delete cache_entry;
    }
    delete tail;
    delete head;
}
 
void LRUCache::addnode(node * newnode) {
    node * temp = head -> next;
    newnode -> next = temp;
    newnode -> prev = head;
    head -> next = newnode;
    temp -> prev = newnode;
}

void LRUCache::deletenode(node * delnode) {
    node * delprev = delnode -> prev;
    node * delnext = delnode -> next;
    delprev -> next = delnext;
    delnext -> prev = delprev;
}
    
LRUInfo LRUCache::get(int offset) {
    LRUInfo response = {0};
    if (map.find(offset) != map.end()) {
        node * resnode = map[offset];
        // Cache Hit
        response.offset = resnode -> offset;
        response.data = resnode -> data;
        response.isCacheMiss = false;
        map.erase(offset);
        deletenode(resnode);
        addnode(resnode);
        map[offset] = head -> next;
        return response;
    }
    response.isCacheMiss = true;
    return response;
}
    
LRUInfo LRUCache::put(int offset, uint8_t data) {
    LRUInfo response = {0};
    bool reuse = false;
    node * existingnode = nullptr;
    response.isCacheMiss = true; // No need to update
    if (map.find(offset) != map.end()) {
        // Cache Hit
        // Update Value of existing cache_entry
        existingnode = map[offset];
        map.erase(offset);
        deletenode(existingnode);
        reuse = true;
        response.isCacheMiss = false; // No need to update
    }
    if (map.size() == cap) {
        // Data not found, need to replace LRU entry
        existingnode = tail -> prev;
        map.erase(existingnode -> offset);
        if (existingnode != head) {
            reuse = true;
        }
        deletenode(tail -> prev);

        // Store data to update, Write-Back situation
        response.cacheReplaced = true;
        response.isCacheMiss = true;
        response.offset = existingnode -> offset;
        response.data = existingnode -> data;
    }
    if (reuse) {
        existingnode -> data = data;
        existingnode -> offset = offset;
        addnode(existingnode);
    } else {
        addnode(new node(offset, data));
    }
    map[offset] = head -> next;
    return response;
}