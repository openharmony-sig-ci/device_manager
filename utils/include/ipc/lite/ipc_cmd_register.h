/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OHOS_DEVICE_MANAGER_IPC_CMD_PARSER_H
#define OHOS_DEVICE_MANAGER_IPC_CMD_PARSER_H

#include <cstdint>
#include <unordered_map>

#include "liteipc_adapter.h"
#include "single_instance.h"

#include "ipc_req.h"
#include "ipc_rsp.h"

namespace OHOS {
namespace DistributedHardware {
#define ON_IPC_SET_REQUEST(cmdCode, paraA, paraB, paraC, paraD) \
    static int32_t IpcSetRequest##cmdCode(paraA, paraB, paraC, paraD); \
    struct IpcRegisterSetRequestFunc##cmdCode { \
        IpcRegisterSetRequestFunc##cmdCode() \
        { \
            IpcCmdRegister::GetInstance().RegisterSetRequestFunc(cmdCode, IpcSetRequest##cmdCode); \
        } \
    }; \
    IpcRegisterSetRequestFunc##cmdCode g_IpcRegisterSetRequestFunc##cmdCode; \
    static int32_t IpcSetRequest##cmdCode(paraA, paraB, paraC, paraD) \

#define ON_IPC_READ_RESPONSE(cmdCode, paraA, paraB) \
    static int32_t IpcReadResponse##cmdCode(paraA, paraB); \
    struct IpcRegisterReadResponseFunc##cmdCode { \
        IpcRegisterReadResponseFunc##cmdCode() \
        { \
            IpcCmdRegister::GetInstance().RegisterReadResponseFunc(cmdCode, IpcReadResponse##cmdCode); \
        } \
    }; \
    IpcRegisterReadResponseFunc##cmdCode g_IpcRegisterReadResponseFunc##cmdCode; \
    static int32_t IpcReadResponse##cmdCode(paraA, paraB) \

#define ON_IPC_CMD(cmdCode, paraA, paraB, paraC) \
    static void IpcCmdProcess##cmdCode(paraA, paraB, paraC); \
    struct IpcRegisterCmdProcessFunc##cmdCode { \
        IpcRegisterCmdProcessFunc##cmdCode() \
        { \
            IpcCmdRegister::GetInstance().RegisterCmdProcessFunc(cmdCode, IpcCmdProcess##cmdCode); \
        } \
    }; \
    IpcRegisterCmdProcessFunc##cmdCode g_IpcRegisterCmdProcessFunc##cmdCode; \
    static void IpcCmdProcess##cmdCode(paraA, paraB, paraC) \

#define ON_IPC_SERVER_CMD(cmdCode, paraA, paraB) \
    static void IpcServerCmdProcess##cmdCode(paraA, paraB); \
    class IpcRegisterServerCmdProcessFunc##cmdCode { \
    public: \
        IpcRegisterServerCmdProcessFunc##cmdCode() \
        { \
            IpcCmdRegister::GetInstance().RegisterServerCmdProcessFunc(cmdCode, IpcServerCmdProcess##cmdCode); \
        } \
    }; \
    IpcRegisterServerCmdProcessFunc##cmdCode g_IpcRegisterServerCmdProcessFunc##cmdCode; \
    static void IpcServerCmdProcess##cmdCode(paraA, paraB)

using SetIpcRequestFunc = int32_t (*)(std::shared_ptr<IpcReq> pBaseReq, IpcIo &request,
    uint8_t *buffer, size_t bufferLen);
using ReadResponseFunc = int32_t (*)(IpcIo &reply, std::shared_ptr<IpcRsp> pBaseRsp);
using OnIpcCmdFunc = void (*)(IpcIo &reply, const IpcContext &ctx, void *ipcMsg);
using OnIpcServerCmdFunc = void (*)(IpcIo &req, IpcIo &reply);

class IpcCmdRegister {
DECLARE_SINGLE_INSTANCE(IpcCmdRegister);
public:
    void RegisterSetRequestFunc(int32_t cmdCode, SetIpcRequestFunc setIpcRequestFunc)
    {
        setIpcRequestFuncMap_.emplace(cmdCode, setIpcRequestFunc);
    };
    void RegisterReadResponseFunc(int32_t cmdCode, ReadResponseFunc readResponseFunc)
    {
        readResponseFuncMap_.emplace(cmdCode, readResponseFunc);
    };
    void RegisterCmdProcessFunc(int32_t cmdCode, OnIpcCmdFunc onIpcCmdFunc)
    {
        onIpcCmdFuncMap_.emplace(cmdCode, onIpcCmdFunc);
    };
    void RegisterServerCmdProcessFunc(int32_t cmdCode, OnIpcServerCmdFunc onIpcServerCmdFunc)
    {
        onIpcServerCmdFuncMap_.emplace(cmdCode, onIpcServerCmdFunc);
    };
    int32_t SetRequest(int32_t cmdCode, std::shared_ptr<IpcReq> pBaseReq, IpcIo &request,
        uint8_t *buffer, size_t buffLen);
    int32_t ReadResponse(int32_t cmdCode, IpcIo &reply, std::shared_ptr<IpcRsp> pBaseRsp);
    int32_t OnIpcCmd(int32_t cmdCode, IpcIo &reply, const IpcContext &ctx, void *ipcMsg);
    int32_t OnIpcServerCmd(int32_t cmdCode, IpcIo &req, IpcIo &reply);
private:
    std::unordered_map<int32_t, SetIpcRequestFunc> setIpcRequestFuncMap_;
    std::unordered_map<int32_t, ReadResponseFunc> readResponseFuncMap_;
    std::unordered_map<int32_t, OnIpcCmdFunc> onIpcCmdFuncMap_;
    std::unordered_map<int32_t, OnIpcServerCmdFunc> onIpcServerCmdFuncMap_;
};
} // namespace DistributedHardware
} // namespace OHOS
#endif // OHOS_DEVICE_MANAGER_IPC_CMD_PARSER_H