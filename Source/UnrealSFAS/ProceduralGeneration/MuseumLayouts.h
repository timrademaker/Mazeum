#pragma once

#include "CoreMinimal.h"

#include "../MapGrid.h"

class UNREALSFAS_API FMuseumLayouts
{
public:
    /**
     * Get all defined museum layouts
     * @return An array of layouts
     */
    static const TArray<FMapGrid>& GetMuseumLayouts();

private:
    /** Add museum layouts to the array */
    static void PopulateMuseumLayoutArray();

private:
    /** Array containing all museum layouts */
    static TArray<FMapGrid> MuseumLayoutArray;
};