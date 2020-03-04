#pragma once
#include "ResourceHolder.h"
#include <unistd.h>

using DescriptorHolder = ResourceHolder<int, decltype(close), close>;
