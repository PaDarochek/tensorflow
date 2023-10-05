# Copyright 2023 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================

from typing import Any

class InterpreterWrapper:
    def __init__(self, *args, **kwargs) -> None: ...
    def AllocateTensors(self, subgraph_index: int = ...) -> object: ...
    def GetSignatureDefs(self) -> object: ...
    def GetSubgraphIndexFromSignature(self, arg0: str) -> object: ...
    def GetTensor(self, tensor_index: int, subgraph_index: int = ...) -> object: ...
    def InputIndices(self) -> object: ...
    def Invoke(self, subgraph_index: int = ...) -> object: ...
    def ModifyGraphWithDelegate(self, arg0: int) -> object: ...
    def NodeInputs(self, arg0: int) -> object: ...
    def NodeName(self, arg0: int) -> str: ...
    def NodeOutputs(self, arg0: int) -> object: ...
    def NumNodes(self) -> int: ...
    def NumTensors(self, arg0: int) -> int: ...
    def OutputIndices(self) -> object: ...
    def ResetVariableTensors(self) -> object: ...
    def ResizeInputTensor(self, i: int, value, strict: bool, subgraph_index: int = ...) -> object: ...
    def SetNumThreads(self, arg0: int) -> object: ...
    def SetTensor(self, i: int, value, subgraph_index: int = ...) -> object: ...
    def TensorName(self, arg0: int, arg1: int) -> str: ...
    def TensorQuantization(self, arg0: int, arg1: int) -> object: ...
    def TensorQuantizationParameters(self, arg0: int, arg1: int) -> object: ...
    def TensorSize(self, arg0: int, arg1: int) -> object: ...
    def TensorSizeSignature(self, arg0: int, arg1: int) -> object: ...
    def TensorSparsityParameters(self, arg0: int, arg1: int) -> object: ...
    def TensorType(self, arg0: int, arg1: int) -> object: ...
    def interpreter(self) -> int: ...
    def tensor(self, base_object, tensor_index: int, subgraph_index: int = ...) -> object: ...

def CreateWrapperFromBuffer(*args, **kwargs) -> Any: ...
def CreateWrapperFromFile(*args, **kwargs) -> Any: ...
