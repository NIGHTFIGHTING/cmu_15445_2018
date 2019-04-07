/**
 * index_iterator.h
 * For range scan of b+ tree
 */
#pragma once
#include "page/b_plus_tree_leaf_page.h"

namespace cmudb {

#define INDEXITERATOR_TYPE                                                     \
  IndexIterator<KeyType, ValueType, KeyComparator>

INDEX_TEMPLATE_ARGUMENTS
class IndexIterator {
public:
  // you may define your own constructor based on your member variables
  IndexIterator(B_PLUS_TREE_LEAF_PAGE_TYPE *leaf, int index, BufferPoolManager *bpm);
  ~IndexIterator();

  bool isEnd() {
    return (leaf_ == nullptr) || (index_ >= leaf_->GetSize());
  };

  // isEnd()Ϊtrue������µ��ã�δ����
  const MappingType &operator*() {
    return leaf_->GetItem(index_);
  };

  // isEnd()Ϊtrue������µ��ã�δ����
  IndexIterator &operator++() {
    if (++index_ >= leaf_->GetSize()) {
        page_id_t next_page_id = leaf_->GetNextPageId();
        if (next_page_id == INVALID_PAGE_ID) {
            leaf_ = nullptr;
        } else {
            //����leafָ��next_page_id��Ӧ��Ҷ�ӽڵ�
            bmp_->UnpinPage(leaf_->GetPageId(), false);
            Page *page = bmp_->FetchPage(next_page_id);
            leaf_ = reinterpret_cast<B_PLUS_TREE_LEAF_PAGE_TYPE *>(page->GetData());
            index_ = 0;
        }
    }
    return *this;
  };

private:
  // add your own private member variables here
  B_PLUS_TREE_LEAF_PAGE_TYPE *leaf_;
  int index_;
  BufferPoolManager *bmp_;
};

} // namespace cmudb
