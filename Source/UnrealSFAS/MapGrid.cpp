// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGrid.h"

FMapGrid::FMapGrid(const uint32 MaximumWidth, const uint32 MaximumDepth)
    : ShiftOffset(MaxGridWidth - MaximumWidth)
    , Width(MaximumWidth)
    , Depth(MaximumDepth)
{
    check(MaximumWidth <= MaxGridWidth && "Maximum maze width cannot exceed 32");
    check(MaximumDepth <= MaxGridDepth && "Maximum maze depth cannot exceed 32");
}

FMapGrid::~FMapGrid()
{
}

void FMapGrid::Set(unsigned int X, unsigned int Y, bool IsOccupied)
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

void FMapGrid::SetRow(unsigned int Y, const uint32 Row)
{
    Grid[Y] = Row;
}

bool FMapGrid::IsEmpty(unsigned int X, unsigned int Y) const
{
    return ((Grid[Y] >> (MaxGridWidth - ShiftOffset - (X + 1))) & 1) == 0;
}

void FMapGrid::Clear()
{
    for (int row = 0; row < MaxGridDepth; ++row)
    {
        SetRow(row, 0);
    }
}

uint32 FMapGrid::GetWidth() const
{
    return Width;
}

uint32 FMapGrid::GetDepth() const
{
    return Depth;
}
