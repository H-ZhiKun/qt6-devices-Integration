#pragma once
#include "BaseProduct.h"
#include <AppFramework.h>

namespace AppFrame
{

class AppPlcExtension : public NonCopyable
{
  public:
    static void afterPLCStatesChange(const uint8_t status)
    {
        const auto &product = appFramework().getPlugin<BaseProduct>();
        // 请完善代码逻辑 修改product 队列中符合需求的瓶状态, 调用product->setRemove();
        if (status == 2 || status == 3)
        {
            product->setAllRemove();
        }
    }
};
} // namespace AppFrame