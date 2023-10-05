/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "xla/translate/hlo_to_mhlo/hlo_utils.h"

#include <cstdint>
#include <cstring>
#include <vector>

#include "mlir/IR/Builders.h"  // from @llvm-project
#include "mlir/IR/BuiltinTypes.h"  // from @llvm-project
#include "mlir/IR/MLIRContext.h"  // from @llvm-project
#include "mlir/Support/DebugStringHelper.h"  // from @llvm-project
#include "xla/literal.h"
#include "xla/literal_util.h"
#include "xla/shape_util.h"
#include "xla/test.h"
#include "xla/types.h"
#include "tsl/lib/core/status_test_util.h"

namespace xla {
namespace {

TEST(ConvertTensorShapeToType, Simple) {
  mlir::MLIRContext context;
  context.loadDialect<mlir::mhlo::MhloDialect>();
  mlir::Builder builder(&context);

  // Static shape.
  {
    auto shape = ShapeUtil::MakeShape(PrimitiveType::S32, {8, 128});
    TF_ASSERT_OK_AND_ASSIGN(
        auto type,
        ConvertTensorShapeToType<mlir::RankedTensorType>(shape, builder));

    auto expected = mlir::RankedTensorType::get({8, 128}, builder.getI32Type());
    EXPECT_TRUE(type == expected)
        << " Expected: " << mlir::debugString(expected)
        << " Computed: " << mlir::debugString(type);
  }

  // Dynamic shape.
  {
    auto shape =
        ShapeUtil::MakeShape(PrimitiveType::S32, {8, 128}, {true, false});
    TF_ASSERT_OK_AND_ASSIGN(
        auto type,
        ConvertTensorShapeToType<mlir::RankedTensorType>(shape, builder));

    int64_t bounds[] = {8, mlir::ShapedType::kDynamic};
    auto extensions = mlir::mhlo::TypeExtensionsAttr::get(&context, bounds);
    auto expected = mlir::RankedTensorType::get(
        {mlir::ShapedType::kDynamic, 128}, builder.getI32Type(), extensions);
    EXPECT_TRUE(type == expected)
        << " Expected: " << mlir::debugString(expected)
        << " Computed: " << mlir::debugString(type);
  }
}

TEST(LiteralToAttrToXlaFormat, Simple) {
  mlir::MLIRContext context;
  context.loadDialect<mlir::mhlo::MhloDialect>();
  mlir::Builder builder(&context);

  // int16
  {
    Literal x = LiteralUtil::CreateR2<int16_t>({{0, 1, 2}, {3, 4, 5}});
    TF_ASSERT_OK_AND_ASSIGN(mlir::DenseElementsAttr attr,
                            CreateDenseElementsAttrFromLiteral(x, builder));

    std::vector<uint8_t> data;
    TF_ASSERT_OK(CopyDenseElementsDataToXlaFormat(attr, &data));
    for (int i = 0; i < 6; i++) {
      int16_t x;
      memcpy(&x, &data[i * 2], 2);
      EXPECT_EQ(x, i);
    }
  }

  // int4
  {
    Literal x = LiteralUtil::CreateR2<s4>(
        {{s4(0), s4(1), s4(2)}, {s4(3), s4(4), s4(5)}});
    TF_ASSERT_OK_AND_ASSIGN(mlir::DenseElementsAttr attr,
                            CreateDenseElementsAttrFromLiteral(x, builder));

    std::vector<uint8_t> data;
    TF_ASSERT_OK(CopyDenseElementsDataToXlaFormat(attr, &data));
    EXPECT_EQ(data, std::vector<uint8_t>({0x01, 0x23, 0x45}));
  }
}

}  // namespace
}  // namespace xla
