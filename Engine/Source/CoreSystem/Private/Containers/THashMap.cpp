//
// Created by Egor Orachyov on 03.07.2019.
//

#include <Containers/THashMap.h>

namespace Berserk
{

    template <typename K, typename V>
    const float32 THashMap<K,V>::LOAD_FACTOR_TO_EXPAND = 1.7;

    template <typename K, typename V>
    const float32 THashMap<K,V>::EXPAND_FACTOR = 4.0;

} // namespace Berserk
