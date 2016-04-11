#pragma once

struct Coordinate
{
    Coordinate(const int argX, const int argY)
        : x(argX)
        , y(argY)
    {
    }

    int x;
    int y;
};