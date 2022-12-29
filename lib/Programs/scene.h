#pragma once

#include "program.h"

struct Vector {
    int x, y;
    Vector(int x, int y)
        : x(x), y(y)
    {
    }
    Vector& operator+=(const Vector& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    Vector& operator-=(const Vector& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
};

class Params {
    std::map<std::string, std::map<std::string, int>> _p;

public:
    int get(std::string objname, std::string paramname)
    {
        return _p[objname][paramname];
    }

    std::map<std::string, int>& getdictfor(std::string objname)
    {
        return _p[objname];
    }
};
class Actor {
public:
    virtual void getParams(std::map<std::string, int>& p) = 0;
    virtual void tick(Ledgrid& matrix, Params& p) = 0;
};

class Dot : public Actor {

    Vector dir;
    Vector pos;
public:
    Dot()
        : dir(1, 1)
        , pos(5, 5)
    {

    }

    void getParams(std::map<std::string, int>& p) override
    {
        p["x"] = pos.x;
        p["y"] = pos.y;
    }


    void tick(Ledgrid& matrix, Params& p) override
    {
        EVERY_N_MILLISECONDS(10) {
            pos += dir;

            dir.x = random8(3)-1;
            dir.y = random8(3)-1;
        }

        matrix.drawPixel(pos.x, pos.y, CRGB::Blue);
    }
};

class Line : public Actor {
    int x;

    int step;
public:
    Line()
        : x(10), step(0)
    {
    }

    void getParams(std::map<std::string, int>& p) override
    {
        
    }

    void tick(Ledgrid& matrix, Params& p) override
    {
        EVERY_N_MILLISECONDS(100) {
                Vector dot(p.get("dot", "x"), p.get("dot", "y"));
                if (dot.x > x)
                    x++;
                else if (dot.x < x)
                    x--;
        }

        matrix.drawLine(x, 0, x, matrix.height(), CHSV(millis()/200, 255, 255));
    }
};
class Scene : public Program {
    Dot _dot;
    Line _line;
public:
    void setup() override
    {
    }
    void tick(Ledgrid &matrix) override
    {
        Params p;
        _dot.getParams(p.getdictfor("dot"));
        _line.getParams(p.getdictfor("line"));

        EVERY_N_MILLISECONDS(500) {
            matrix.fillScreen(CRGB::Black);
        }
        _dot.tick(matrix, p);
        _line.tick(matrix, p);
    }
};
