// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/** Class representing a 2D map where each space is either filled or empty */
class UNREALSFAS_API FMapGrid
{
    /** The maximum possible width of the grid */
    static constexpr unsigned int MaxGridWidth = 32;
    /** The maximum possible depth of the grid */
    static constexpr unsigned int MaxGridDepth = 32;

public:
    FMapGrid(const uint32 MaximumWidth = MaxGridWidth, const uint32 MaximumDepth = MaxGridDepth);
	~FMapGrid();

    /**
     * Set the state of a tile
     * @param X The X-coordinate of the grid to set the value for
     * @param Y The Y-coordinate of the grid to set the value for
     * @param IsOccupied Whether the specified coordinate is taken or not
     */
    void Set(unsigned int X, unsigned int Y, bool IsOccupied);
    /**
     * Set the state of a row
     * @param Y The Y-coordinate of the grid to set the value for
     * @param Row The new value of the row
     */
    void SetRow(unsigned int Y, const uint32 Row);

    /**
     * Get the state of a tile
     * @param X The X-coordinate of the grid to get the value for
     * @param Y The Y-coordinate of the grid to get the value for
     * @return True if the tile at the specified coordinate is empty
     */
    bool IsEmpty(unsigned int X, unsigned int Y) const;

    /**
     * Set all grid values to 0
     */
    void Clear();

    /**
     * Get the width of this grid
     * @return The width of this grid
     */
    FORCEINLINE uint32 GetWidth() const { return Width; }
    /**
     * Get the depth of this grid
     * @return The depth of this grid
     */
    FORCEINLINE uint32 GetDepth() const { return Depth; }

protected:
    /** An array representing the grid */
    uint32 Grid[MaxGridDepth] = { 0 };
    /** The user-defined maximum width of the grid */
    uint32 Width = MaxGridWidth;
    /** The user-defined maximum depth of the grid */
    uint32 Depth = MaxGridDepth;
};
