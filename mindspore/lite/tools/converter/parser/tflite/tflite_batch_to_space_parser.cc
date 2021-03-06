/**
 * Copyright 2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * distributed under the License is distributed on an AS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "tools/converter/parser/tflite/tflite_batch_to_space_parser.h"
#include <vector>
#include <memory>
#include <string>
#include <map>

namespace mindspore::lite {
PrimitiveC *TfliteBatchToSpaceParser::ParseLitePrimitive(const std::unique_ptr<tflite::OperatorT> &tflite_op,
                                                         const std::unique_ptr<tflite::ModelT> &tflite_model) {
  const auto &tflite_subgraph = tflite_model->subgraphs.front();
  auto primitive = std::make_unique<schema::PrimitiveT>();
  std::unique_ptr<schema::BatchToSpaceT> attr = std::make_unique<schema::BatchToSpaceT>();
  if (attr == nullptr) {
    MS_LOG(ERROR) << "new op failed";
    return nullptr;
  }

  if (GetTfliteData(tflite_op->inputs[1], tflite_subgraph->tensors, tflite_model->buffers, attr->blockShape)) {
    MS_LOG(ERROR) << "get batchToSpace -> blockShape failed";
    return nullptr;
  }
  if (GetTfliteData(tflite_op->inputs[2], tflite_subgraph->tensors, tflite_model->buffers, attr->crops)) {
    MS_LOG(ERROR) << "get batchToSpace -> crops failed";
    return nullptr;
  }

  primitive->value.type = schema::PrimitiveType_BatchToSpace;
  primitive->value.value = attr.release();
  return PrimitiveC::Create(primitive.release());
}

TfliteNodeRegister g_tfliteBatchToSpaceNDParser(tflite::BuiltinOperator_BATCH_TO_SPACE_ND,
                                                new TfliteBatchToSpaceParser());
}  // namespace mindspore::lite
