#include "cfgDef.h"

#include <QKeyEvent>

keyHash_t CCfgHelper::getKeyHash(int key, int modifiler)
{
    return (static_cast<keyHash_t>(modifiler) << 32) | (key & 0xFFFFFFFF);
}

keyHash_t CCfgHelper::getKeyHash(const QKeyEvent* ev)
{
    if (nullptr == ev)
        return 0;
    
    int mod = NO_MODIFILER;
    auto modFlags = ev->modifiers();
    if (modFlags.testFlag(Qt::ControlModifier))
        mod |= MOD_CTRL;
    if (modFlags.testFlag(Qt::AltModifier))
        mod |= MOD_ALT;
    if (modFlags.testFlag(Qt::ShiftModifier))
        mod |= MOD_SHIFT;

    return getKeyHash(ev->key(), mod);
}

bool CCfgHelper::isUsageType(Usage usage, Usage usageType)
{
    return static_cast<int>(usageType) == (static_cast<int>(usage) & static_cast<int>(usageType));
}
