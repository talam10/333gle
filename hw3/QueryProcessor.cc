/*
 * Copyright ©2023 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include "./QueryProcessor.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <string>
#include <vector>

extern "C" {
  #include "./libhw1/CSE333.h"
}

using std::list;
using std::sort;
using std::string;
using std::vector;

namespace hw3 {

QueryProcessor::QueryProcessor(const list<string>& index_list, bool validate) {
  // Stash away a copy of the index list.
  index_list_ = index_list;
  array_len_ = index_list_.size();
  Verify333(array_len_ > 0);

  // Create the arrays of DocTableReader*'s. and IndexTableReader*'s.
  dtr_array_ = new DocTableReader* [array_len_];
  itr_array_ = new IndexTableReader* [array_len_];

  // Populate the arrays with heap-allocated DocTableReader and
  // IndexTableReader object instances.
  list<string>::const_iterator idx_iterator = index_list_.begin();
  for (int i = 0; i < array_len_; i++) {
    FileIndexReader fir(*idx_iterator, validate);
    dtr_array_[i] = fir.NewDocTableReader();
    itr_array_[i] = fir.NewIndexTableReader();
    idx_iterator++;
  }
}

QueryProcessor::~QueryProcessor() {
  // Delete the heap-allocated DocTableReader and IndexTableReader
  // object instances.
  Verify333(dtr_array_ != nullptr);
  Verify333(itr_array_ != nullptr);
  for (int i = 0; i < array_len_; i++) {
    delete dtr_array_[i];
    delete itr_array_[i];
  }

  // Delete the arrays of DocTableReader*'s and IndexTableReader*'s.
  delete[] dtr_array_;
  delete[] itr_array_;
  dtr_array_ = nullptr;
  itr_array_ = nullptr;
}

// This structure is used to store a index-file-specific query result.
typedef struct {
  DocID_t doc_id;  // The document ID within the index file.
  int     rank;    // The rank of the result so far.
} IdxQueryResult;

vector<QueryProcessor::QueryResult>
QueryProcessor::ProcessQuery(const vector<string>& query) const {
  Verify333(query.size() > 0);

  // STEP 1.
  // (the only step in this file)

  vector<QueryProcessor::QueryResult> final_result;

  // go through each index file
  for (int i = 0; i < array_len_; i++) {
    vector<IdxQueryResult> index_result;

    // check first word
    DocIDTableReader* ditr = itr_array_[i]->LookupWord(query[0]);
    // no record for the first word, so continue to next index file
    if (ditr == nullptr) {
      delete ditr;
      continue;
    }

    // get list of doc_ids for the query word and add them to index_result list
    list<DocIDElementHeader> id_list = ditr->GetDocIDList();
    delete ditr;
    for (DocIDElementHeader& header : id_list) {
      IdxQueryResult doc_res = {header.doc_id, header.num_positions};
      index_result.push_back(doc_res);
    }

    // go through each subsequent word
    for (auto word = query.begin()+1; word != query.end(); word++) {
      DocIDTableReader* ditr = itr_array_[i]->LookupWord(*word);
      // no record for the query word, so remove the current results
      if (ditr == nullptr) {
        index_result.clear();
        delete ditr;
        break;
      }
      // get position offsets of the current word for each doc_id result
      list<DocPositionOffset_t> pos_list;
      for (auto res = index_result.begin(); res != index_result.end(); res++) {
        pos_list.clear();
        ditr->LookupDocID(res->doc_id, &pos_list);
        // if no position offsets exist then remove document from index_result
        if (pos_list.size() == 0) {
          index_result.erase(res);
          res--;
          continue;
        }
        // else add the number of position offsets to the doc rank
        res->rank += pos_list.size();
      }
      delete ditr;
    }
    // add the results from the current index to the final results vector
    for (auto& res : index_result) {
      string doc_name;
      dtr_array_[i]->LookupDocID(res.doc_id, &doc_name);
      // generate QueryResult with doc_name from IdxQueryResult with doc_id
      QueryResult doc_result = {doc_name, res.rank};
      final_result.push_back(doc_result);
    }
  }

  // Sort the final results.
  sort(final_result.begin(), final_result.end());
  return final_result;
}

}  // namespace hw3
