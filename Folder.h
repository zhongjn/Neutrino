#pragma once
#include "Property.h"
#include "Nullable.h"
#include <string>

class Folder {
    PROPERTY_READ(int, Id)
    PROPERTY_READ(string, Name)
public:
    Folder(int id, string name) : m_Id(id), m_Name(move(name)) {};
};