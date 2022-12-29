#pragma once

#include "ledgrid.h"

#pragma once
class Program {
public:
    virtual void setup() {};
    virtual void tick(Ledgrid&) = 0;

    //virtual void setConfig(JsonObject data) { }
    //virtual void getConfig(JsonObject data) { }
};
