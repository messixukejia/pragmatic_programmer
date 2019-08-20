#ifndef DATA_STRUCT_SKIPLIST_H_
#error Attempt to include "skiplist.h" exclusively is forbidden!
#endif

////////////////////////////// SkipList ////////////////////////////// 
template <typename Key, class Comparator>
typename SkipList<Key, Comparator>::SkipNode* SkipList<Key, Comparator>::find_equal_or_greater_node(const Key& key, SkipNode** prev) {
	SkipNode* cur_node = _head;
	uint32_t level = _max_height - 1;

	while(true) {
		SkipNode* next_node = cur_node->next(level);
		// 如果后续节点为空，则向下一层。
		if (NULL != next_node && _compare(key, next_node->_key) > 0) {
			cur_node = next_node;
		} else {
			if (NULL != prev) {
				prev[level] = cur_node;
			}
			if (0 == level) {
				return next_node;
			} else {
				level--;
			}
		}
	}
}

template <typename Key, class Comparator>
typename SkipList<Key, Comparator>::SkipNode* SkipList<Key, Comparator>::find_less_node(const Key& key) {
	SkipNode* cur_node = _head;
	uint32_t level = _max_height - 1;

	while (true) {
		SkipNode* next_node = cur_node->next(level);
		if (NULL == next_node || _compare(next_node->_key, key) >= 0) {
			if (0 == level) {
				return cur_node;
			} else {
				level--;
			}
		} else {
			cur_node = next_node;
		}
	}
}

template <typename Key, class Comparator>
typename SkipList<Key, Comparator>::SkipNode* SkipList<Key, Comparator>::find_last_node() {
	SkipNode* cur_node = _head;
	uint32_t level = _max_height - 1;
	while(true) {
		SkipNode* next_node = cur_node->next(level);
		if (NULL == next_node) {
			if (0 == level) {
				return cur_node;
			} else {
				level--;
			}
		} else {
			cur_node = next_node;
		}
	}
}

template <typename Key, class Comparator>
Retcode SkipList<Key, Comparator>::insert(const Key& key) {
	// newnode: 待插入节点
	// prev: 各层对应的前序节点指针
	// greater_node: 后续节点。

	SkipNode* prev[MAX_HEIGHT];
	SkipNode* greater_node;
	greater_node = find_equal_or_greater_node(key, prev);

	uint32_t height = _random_engine() % MAX_HEIGHT + 1;
	if (height > _max_height) {
		height = _max_height + 1;
	}
	SkipNode* new_node = new_skip_node(key, height);
	if (NULL == new_node) {
		return RET_FAIL;
	}

	// 如果新节点高度大于之前最大高度，则补齐前序节点。
	if (height > _max_height) {
		for (uint32_t h = _max_height; h < height; h++) {
			prev[h] = _head;
		}

		std::cout << "max_height change from " << _max_height << " to  " << height << std::endl;
		_max_height = height;
	}	

	// 插入节点
	for (uint32_t h = 0; h < height; h++) {
		SkipNode* x = prev[h]->next(h);
		new_node->set_next(h, x);
		prev[h]->set_next(h, new_node);
	}

	return RET_SUCC;
}

template <typename Key, class Comparator>
bool SkipList<Key, Comparator>::has_key(const Key& key) {
	SkipNode* greater_node = find_equal_or_greater_node(key, NULL);	
	if (NULL != greater_node && 0 == _compare(key, greater_node->_key)) {
		return true;
	}
	return false;
}

template <typename Key, class Comparator>
typename SkipList<Key, Comparator>::SkipNode* SkipList<Key, Comparator>::new_skip_node(const Key& key, uint32_t height) {
	SkipNode* node = (SkipNode*)(new char[sizeof(SkipNode) + sizeof(std::atomic<SkipNode*>)*(height - 1)]);
	if (NULL == node) {
		std::cout << "malloc node failed. height is " << height << std::endl;
		return NULL;
	}
 
	node->_key = key;

	for (uint32_t h = 0; h < height; h++) {
		node->set_next(h, NULL);
	}

	return node;
}

////////////////////////////// Iterator ////////////////////////////// 
template <typename Key, class Comparator>
bool Iterator<Key, Comparator>::valid() const {
	return (NULL != _node) && (_node != _list->head());
}

template <typename Key, class Comparator>
const Key& Iterator<Key, Comparator>::key() const {
	return _node->_key;
}

template <typename Key, class Comparator>
void Iterator<Key, Comparator>::next() {
	_node = _node->next(0);
}

template <typename Key, class Comparator>
void Iterator<Key, Comparator>::prev() {
	_node = _list->find_less_node(_node->_key);
}

template <typename Key, class Comparator>
void Iterator<Key, Comparator>::seek(const Key& target) {
	_node = _list->find_equal_or_greater_node(target, NULL);	
}

template <typename Key, class Comparator>
void Iterator<Key, Comparator>::seek_to_first() {
	_node = _list->head()->next(0);
}

template <typename Key, class Comparator>
void Iterator<Key, Comparator>::seek_to_last() {
	_node = _list->find_last_node();	
}


