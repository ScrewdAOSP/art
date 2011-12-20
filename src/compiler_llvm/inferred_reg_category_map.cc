/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "inferred_reg_category_map.h"

#include "backend_types.h"
#include "stl_util.h"

#include <stdint.h>
#include <vector>

namespace art {
namespace compiler_llvm {


InferredRegCategoryMap::InferredRegCategoryMap(uint32_t insns_size,
                                               uint8_t regs_size)
: registers_size_(regs_size), lines_(insns_size, NULL) {
}

InferredRegCategoryMap::~InferredRegCategoryMap() {
  STLDeleteElements(&lines_);
}

RegCategory InferredRegCategoryMap::GetRegCategory(uint32_t dex_pc,
                                                   uint16_t reg_idx) const {
  CHECK_NE(lines_[dex_pc], static_cast<RegCategoryLine*>(NULL));
  return static_cast<RegCategory>((*lines_[dex_pc])[reg_idx]);
}

void InferredRegCategoryMap::SetRegCategory(uint32_t dex_pc,
                                            uint16_t reg_idx,
                                            RegCategory cat) {
  if (lines_[dex_pc] == NULL) {
    lines_[dex_pc] = new RegCategoryLine(registers_size_, kRegUnknown);
  }

  (*lines_[dex_pc])[reg_idx] = cat;
}

bool InferredRegCategoryMap::
operator==(InferredRegCategoryMap const& rhs) const {

  if (registers_size_ != rhs.registers_size_) {
    return false;
  }

  if (lines_.size() != rhs.lines_.size()) {
    return false;
  }

  for (size_t i = 0; i < lines_.size(); ++i) {
    if (lines_[i] == NULL && rhs.lines_[i] == NULL) {
      continue;
    }

    if ((lines_[i] == NULL && rhs.lines_[i] != NULL) ||
        (lines_[i] != NULL && rhs.lines_[i] == NULL)) {
      return false;
    }

    if (*lines_[i] != *rhs.lines_[i]) {
      return false;
    }
  }

  return true;
}

bool InferredRegCategoryMap::
operator!=(InferredRegCategoryMap const& rhs) const {

  return !(*this == rhs);
}


} // namespace compiler_llvm
} // namespace art
