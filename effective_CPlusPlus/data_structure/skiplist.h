#ifndef DATA_STRUCT_SKIPLIST_H_
#define DATA_STRUCT_SKIPLIST_H_

#include <iostream> 
#include <atomic>
#include <random>

#include "common.h"

static const uint32_t MAX_HEIGHT = 20;

//Comparator: Key的比较函数。 less < 0, equal = 0, greater > 0.
template <typename Key, class Comparator>
class SkipList {
public:
	// 节点定义
	struct SkipNode {
		SkipNode* next(uint32_t level) {
			return _next[level].load(std::memory_order_acquire);
		}
		void set_next(uint32_t level, SkipNode* node) {
			_next[level].store(node, std::memory_order_release);
		}

		Key _key;
		std::atomic<SkipNode*> _next[1];	
	};	

	SkipList(Comparator cmp) :
		_max_height(1), _head(new_skip_node(0, MAX_HEIGHT)), _compare(cmp) {

	}
		
	// 寻找大于或等于查询key的节点。如果查不到，返回null。
	SkipNode* find_equal_or_greater_node(const Key& key, SkipNode** prev);

	// 查找小余查询key的节点。
	SkipNode* find_less_node(const Key& key);

	SkipNode* find_last_node();	

	Retcode insert(const Key& key);

	bool has_key(const Key& key);	

	SkipNode* head() {
		return _head;
	}
	
private:
	SkipNode* new_skip_node(const Key& key, uint32_t height);
	
	uint32_t _max_height;
	std::default_random_engine _random_engine; 
	SkipNode* _head;
	Comparator const _compare;
};

template <typename Key, class Comparator>
class Iterator {
public:
	Iterator(SkipList<Key, Comparator>* list) : 
		_list(list), 
		_node(_list->head()->next(0)) {}

	bool valid() const;

    const Key& key() const;

    void next();

    void prev();

    void seek(const Key& target);

    void seek_to_first();

    void seek_to_last();

private:
	SkipList<Key, Comparator>* _list;
	typename SkipList<Key, Comparator>::SkipNode* _node;
};

#include "skiplist.hpp"

#endif // DATA_STRUCT_SKIPLIST_H_
