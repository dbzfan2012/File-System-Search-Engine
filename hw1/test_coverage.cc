/*
 * Copyright ©2024 Hannah C. Tang.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Spring Quarter 2024 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

extern "C" {
  #include "./HashTable.h"
  #include "./HashTable_priv.h"
  #include "./LinkedList.h"
  #include "./LinkedList_priv.h"
}

#include "gtest/gtest.h"

#include "./test_suite.h"

namespace hw1 {

int TestLLPayloadComparatorNew(LLPayload_t p1, LLPayload_t p2) {
  // A comparator used to test sort.
  if (p1 > p2)
    return 1;
  if (p1 < p2)
    return -1;
  return 0;
}

// Our payload structure
typedef struct payload_st {
  int magic_num;
  int payload_num;
} Payload;

class Test_Coverage : public ::testing::Test {
 protected:
  static const int kMagicNum = 0xDEADBEEF;

  // Code here will be called before each test executes (ie, before
  // each TEST_F).
  virtual void SetUp() {
    freeInvocations_ = 0;
  }

  // A version of free() that verifies the payload before freeing it.
  static void VerifiedFree(HTValue_t payload) {
    ASSERT_EQ(kMagicNum, (static_cast<Payload *>(payload))->magic_num);
    free(payload);
  }

  // A version of VerifiedFree() which counts how many times it's been
  // invoked; this allows us to make assertions.  Note that the counter
  //  is reset in SetUp().
  static int freeInvocations_;
  static void InstrumentedFree(HTValue_t payload) {
    freeInvocations_++;
    VerifiedFree(payload);
  }

  static void StubbedFree(LLPayload_t payload) {
    // Do nothing but verify the payload is non-NULL and
    // increment the free count.
    ASSERT_TRUE(payload != NULL);
    freeInvocations_++;
  }
};  // class Test_Coverage

// statics:
int Test_Coverage::freeInvocations_;
const int Test_Coverage::kMagicNum;


TEST_F(Test_Coverage, CoverageTests) {
  HashTable *ht = HashTable_Allocate(3);
  ASSERT_EQ(0, ht->num_elements);
  ASSERT_EQ(3, ht->num_buckets);

  ASSERT_TRUE(ht->buckets != NULL);
  ASSERT_EQ(0, LinkedList_NumElements(ht->buckets[0]));
  ASSERT_EQ(0, LinkedList_NumElements(ht->buckets[1]));
  ASSERT_EQ(0, LinkedList_NumElements(ht->buckets[2]));
  HashTable_Free(ht, &Test_Coverage::VerifiedFree);


  // Bonus: hashed_key created with FNVHash64
  HashTable *table = HashTable_Allocate(10);
  HTKey_t hashed_key = static_cast<HTKey_t>(12);
  unsigned char buffer[] = {(unsigned char) (hashed_key), (unsigned char) (hashed_key + 1), (unsigned char) (hashed_key + 4), (unsigned char) (hashed_key + 9), (unsigned char) (hashed_key + 16)};
  hashed_key = FNVHash64(buffer, 5);
  // Insert two elements with the key
  HTKeyValue_t newkv1, oldkv1;
  newkv1.key = hashed_key;
  newkv1.value = static_cast<HTValue_t>(&newkv1);
  ASSERT_FALSE(HashTable_Insert(table, newkv1, &oldkv1));
  HTKeyValue_t newkv2, oldkv2;
  newkv2.key = hashed_key;
  newkv2.value = static_cast<HTValue_t>(&newkv2);
  ASSERT_TRUE(HashTable_Insert(table, newkv2, &oldkv2));
  // Remove element with the key
  // ASSERT_TRUE(HashTable_Remove(table, hashed_key, &oldkv1));

  // Bonus: Free iterator before it moves forward, then allocate again
  HTIterator *it = HTIterator_Allocate(table);
  HTIterator_Free(it);
  it = HTIterator_Allocate(table);
  ASSERT_TRUE(HTIterator_IsValid(it));

  // Bonus: Remove returns false when table has 0 elements
  HTKeyValue_t newkv_test;
  ASSERT_TRUE(HTIterator_Remove(it, &newkv_test));
  ASSERT_FALSE(HTIterator_Remove(it, &newkv_test));

  // Bonus: Test sort doesn't run with < 2 elements
  LinkedList *llp = LinkedList_Allocate();
  LinkedList_Push(llp, (LLPayload_t) 32);
  LinkedList before = *llp;
  LinkedList_Sort(llp, true, TestLLPayloadComparatorNew);
  ASSERT_EQ((*llp).head, before.head);

  HashTable_Free(table, &Test_Coverage::VerifiedFree);
  HTIterator_Free(it);
  LinkedList_Free(llp, &Test_Coverage::StubbedFree);
}

}  // namespace hw1
