#include "rules.hpp"

int healthPercent(int health) {
    return health * 100 / maxHealth();
}
