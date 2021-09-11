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

#ifndef OHOS_DEVICE_MANAGER_IPC_SERVER_LISTENER_MGR_H
#define OHOS_DEVICE_MANAGER_IPC_SERVER_LISTENER_MGR_H

#include <cstdint>
#include <string>
#include <map>
#include <mutex>

#include "liteipc_adapter.h"
#include "single_instance.h"

namespace OHOS {
namespace DistributedHardware {
typedef struct CommonSvcId {
    unsigned int handle;
    unsigned int token;
    unsigned int cookie;
    IpcContext *ipcCtx;
    unsigned int cbId;
}CommonSvcId;

class IpcServerListenermgr {
DECLARE_SINGLE_INSTANCE(IpcServerListenermgr);
public:
    int32_t RegisterListener(std::string &pkgName, const CommonSvcId *svcId);
    int32_t GetListenerByPkgName(std::string &pkgName, CommonSvcId *svcId);
    int32_t UnregisterListener(std::string &pkgName);
    const std::map<std::string, CommonSvcId> &GetAllListeners();
private:
    std::map<std::string, CommonSvcId> dmListenerMap_;
    std::mutex lock_;
};
} // namespace DistributedHardware
} // namespace OHOS
#endif // OHOS_DEVICE_MANAGER_IPC_SERVER_LISTENER_MGR_H
