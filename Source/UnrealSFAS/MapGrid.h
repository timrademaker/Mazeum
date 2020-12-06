// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * @brief Class representing a 2D map where each space is either filled or empty
 */
class UNREALSFAS_API FMapGrid
{
public:
    static constexpr unsigned int MaxWidth = 32;
    static constexpr unsigned int MaxDepth = 32;

public:
    FMapGrid(const uint32 MaximumWidth = MaxWidth);
	~FMapGrid();

    /**
    * @brief Set the state of a tile
    * @params X The X-coordinate of the grid to set the value for
    * @params Y The Y-coordinate of the grid to set the value for
    * @params IsOccupied Whether the specified coordinate is taken or not
    */
    void Set(unsigned int X, unsigned int Y, bool IsOccupied);
    /**
    * @brief Set the state of a row
    * @params Y The Y-coordinate of the grid to set the value for
    * @params Row The new value of the row
    */
    void SetRow(unsigned int Y, const uint32 Row);

    /**
    * @brief Get the state of a tile
    * @params X The X-coordinate of the grid to get the value for
    * @params Y The Y-coordinate of the grid to get the value for
    * @returns True if the tile at the specified coordinate is empty
    */
    bool IsEmpty(unsigned int X, unsigned int Y);

protected:
    uint32 Grid[MaxDepth] = { 0 };
    /// If the desired maximum width is not MaxWidth, IsEmpty needs this to make sure that it gets the correct value
    uint32 ShiftOffset = 0;
};
