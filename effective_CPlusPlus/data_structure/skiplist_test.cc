#include <iostream>
#include <vector>
#include <algorithm>
#include "gtest/gtest.h"
#include "skiplist.h"

#define MAX_LOOP 1000

typedef uint64_t Key;
struct Comparator {
  int operator()(const Key& a, const Key& b) const {
    if (a < b) {
      return -1;
    } else if (a > b) {
      return +1;
    } else {
      return 0;
    }
  }
};

// The fixture for testing class Project1. From google test primer.
class SkiplistTest : public ::testing::Test {
protected:
	SkiplistTest() {
		Key k = 0;
		for (k = 0; k < MAX_LOOP; k++) {
			_key_list[k] = k * 10;
		}
	
		std::random_shuffle(_key_list, _key_list + MAX_LOOP - 1);
	}

	virtual ~SkiplistTest() {
	}

	virtual void SetUp() {
	}

	virtual void TearDown() {
	}

	Key _key_list[MAX_LOOP];
};

TEST_F(SkiplistTest, insert_forward) {
	Comparator cmp;
	SkipList<Key, Comparator> skiplist(cmp);
	Key k = 0;
	for (k = 0; k < MAX_LOOP; k++) {
		skiplist.insert(k * 10);
	}
	
	Iterator<Key, Comparator> iter(&skiplist);
	
	for (k = 0; k < MAX_LOOP; k++) {
		EXPECT_EQ(iter.valid(), true);
		EXPECT_EQ(iter.key(), k * 10);
		iter.next();
	}
	EXPECT_EQ(iter.valid(), false);
}

TEST_F(SkiplistTest, insert_backward) {
	Comparator cmp;
	SkipList<Key, Comparator> skiplist(cmp);
	Key k = 0;
	for (k = MAX_LOOP; k > 0; k--) {
		skiplist.insert(k * 10);
	}
	
	Iterator<Key, Comparator> iter(&skiplist);
	
	for (k = 1; k <= MAX_LOOP; k++) {
		EXPECT_EQ(iter.valid(), true);
		EXPECT_EQ(iter.key(), k * 10);
		iter.next();
	}
	EXPECT_EQ(iter.valid(), false);
}

TEST_F(SkiplistTest, insert_random) {
	Comparator cmp;
	SkipList<Key, Comparator> skiplist(cmp);
	
	Key k = 0;
	for (k = 0; k < MAX_LOOP; k++) {
		skiplist.insert(_key_list[k]);
	}
	
	Iterator<Key, Comparator> iter(&skiplist);
	for (k = 0; k < MAX_LOOP; k++) {
		EXPECT_EQ(iter.valid(), true);
		EXPECT_EQ(iter.key(), k * 10);
		iter.next();
	}
	EXPECT_EQ(iter.valid(), false);
}

TEST_F(SkiplistTest, insert_seek) {
	Comparator cmp;
	SkipList<Key, Comparator> skiplist(cmp);
	Key k = 0;
	for (k = 0; k < MAX_LOOP; k++) {
		skiplist.insert(_key_list[k]);
	}

	EXPECT_EQ(skiplist.has_key(0), true);
	EXPECT_EQ(skiplist.has_key(5), false);
	EXPECT_EQ(skiplist.has_key(MAX_LOOP / 2 * 10), true);
	EXPECT_EQ(skiplist.has_key(MAX_LOOP / 3 * 10), true);
	EXPECT_EQ(skiplist.has_key(MAX_LOOP / 3 * 10 + 1), false);
}

TEST_F(SkiplistTest, insert_iter_seek) {
	Comparator cmp;
	SkipList<Key, Comparator> skiplist(cmp);
	
	Key k = 0;
	for (k = 0; k < MAX_LOOP; k++) {
		skiplist.insert(_key_list[k]);
	}
	
	Iterator<Key, Comparator> iter(&skiplist);
	iter.seek(0);
	EXPECT_EQ(iter.key(), 0);

	iter.seek(5);
	EXPECT_EQ(iter.key(), 10);

	iter.seek(MAX_LOOP / 2 * 10);
	EXPECT_EQ(iter.key(), MAX_LOOP / 2 * 10);

	iter.seek(MAX_LOOP / 2 * 10 + 1);
	EXPECT_EQ(iter.key(), (MAX_LOOP / 2 + 1 )* 10);
}

TEST_F(SkiplistTest, insert_iter_backward) {
	Comparator cmp;
	SkipList<Key, Comparator> skiplist(cmp);
	
	Key k = 0;
	for (k = 0; k < MAX_LOOP; k++) {
		skiplist.insert(_key_list[k]);
	}

	Iterator<Key, Comparator> iter(&skiplist);
	iter.seek_to_last();
	EXPECT_EQ(iter.key(), (MAX_LOOP - 1) * 10);

	for (k = MAX_LOOP - 2; k > 0; k--) {
		iter.prev();
		EXPECT_EQ(iter.key(), k * 10);
	}	
	iter.prev();
	EXPECT_EQ(iter.key(), 0 * 10);
	iter.prev();
	EXPECT_EQ(iter.valid(), false);
}


