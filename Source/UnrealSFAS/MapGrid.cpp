// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGrid.h"

FMapGrid::FMapGrid(const uint32 MaximumWidth)
    : ShiftOffset(MaxWidth - MaximumWidth)
{
    check(MaximumWidth <= MaxWidth && "Maximum maze width cannot exceed 32");
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

bool FMapGrid::IsEmpty(unsigned int X, unsigned int Y)
{
    return ((Grid[Y] >> (MaxWidth - ShiftOffset - (X + 1))) & 1) == 0;
}
