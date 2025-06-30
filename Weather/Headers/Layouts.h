#pragma once

class Layouts
{
public:
    Layouts() = default;
protected:
    virtual constexpr void createLayouts() = 0;
protected:
    virtual ~Layouts() {}
};
