#pragma once

#define DB_CALLBACK(name) \
    auto name = [](void* p, int count, char** values, char** names) -> int

#define DB_CALLBACK_PARAM(type) \
    auto param = static_cast<type*>(p);