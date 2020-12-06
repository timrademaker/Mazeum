// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGrid.h"

MapGrid::MapGrid(const uint32 MaximumWidth)
    : ShiftOffset(MaxWidth - MaximumWidth)
{
    check(MaximumWidth <= MaxWidth && "Maximum maze width cannot exceed 32");
}

MapGrid::~MapGrid()
{
}

void MapGrid::Set(unsigned int X, unsigned int Y, bool IsOccupied)
{
    uint32 mask = (1 << X);

    if (IsOccupied)
    {
        Grid[Y] |= mask;
    }
    else
    {
        Grid[Y] &= ~mask;
    }
}

void MapGrid::SetRow(unsigned int Y, const uint32 Row)
{
    Grid[Y] = Row;
}

bool MapGrid::IsEmpty(unsigned int X, unsigned int Y)
{
    return ((Grid[Y] >> (MaxWidth - ShiftOffset - (X + 1))) & 1) == 0;
}
