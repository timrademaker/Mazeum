#include "MuseumLayouts.h"

TArray<FMapGrid> FMuseumLayouts::MuseumLayoutArray;


const TArray<FMapGrid>& FMuseumLayouts::GetMuseumLayouts()
{
    if (MuseumLayoutArray.Num() == 0)
    {
        PopulateMuseumLayoutArray();
    }

    return MuseumLayoutArray;
}

void FMuseumLayouts::PopulateMuseumLayoutArray()
{
    MuseumLayoutArray.Empty();

    {
        FMapGrid& layout = MuseumLayoutArray.Add_GetRef(FMapGrid(15, 15));

        layout.SetRow(0, 0b111110000000000);
        layout.SetRow(1, 0b111110000000000);
        layout.SetRow(2, 0b000110000000000);
        layout.SetRow(3, 0b000110000000000);
        layout.SetRow(4, 0b000110000000000);
        layout.SetRow(5, 0b000111111111000);
        layout.SetRow(6, 0b000100000011000);
        layout.SetRow(7, 0b000100000011000);
        layout.SetRow(8, 0b000100000011000);
        layout.SetRow(9, 0b000111111111000);
    }

    {
        FMapGrid& layout = MuseumLayoutArray.Add_GetRef(FMapGrid(15, 14));

        layout.SetRow(0 , 0b111111100000000);
        layout.SetRow(1 , 0b111111100000000);
        layout.SetRow(2 , 0b000011100000000);
        layout.SetRow(3 , 0b000011111111111);
        layout.SetRow(4 , 0b000011111111111);
        layout.SetRow(5 , 0b000011100000000);
        layout.SetRow(6 , 0b000011100000000);
        layout.SetRow(7 , 0b000011100000000);
        layout.SetRow(8 , 0b111111111111111);
        layout.SetRow(9 , 0b111111111111111);
        layout.SetRow(10, 0b111111111111111);
        layout.SetRow(11, 0b000000000000000);
        layout.SetRow(12, 0b000000000000000);
        layout.SetRow(13, 0b000000000000000);
    }
    
    {
        FMapGrid& layout = MuseumLayoutArray.Add_GetRef(FMapGrid(18, 18));

        layout.SetRow(0 , 0b111111111111111111);
        layout.SetRow(1 , 0b111111111111111111);
        layout.SetRow(2 , 0b110000000000000011);
        layout.SetRow(3 , 0b110000000000000011);
        layout.SetRow(4 , 0b110000000000000011);
        layout.SetRow(5 , 0b110000000000000011);
        layout.SetRow(6 , 0b110000000000000011);
        layout.SetRow(7 , 0b110000000000000011);
        layout.SetRow(8 , 0b111111111111111111);
        layout.SetRow(9 , 0b111111111111111111);
        layout.SetRow(10, 0b110000000000000011);
        layout.SetRow(11, 0b110000000000000011);
        layout.SetRow(12, 0b110000000000000011);
        layout.SetRow(13, 0b110000000000000011);
        layout.SetRow(14, 0b110000000000000011);
        layout.SetRow(15, 0b110000000000000011);
        layout.SetRow(16, 0b111111111111111111);
        layout.SetRow(17, 0b111111111111111111);
    }
    
    {
        FMapGrid& layout = MuseumLayoutArray.Add_GetRef(FMapGrid(18, 22));

        layout.SetRow(0 , 0b111111111111111111);
        layout.SetRow(1 , 0b111111111111111111);
        layout.SetRow(2 , 0b110000000000000011);
        layout.SetRow(3 , 0b110000000000000011);
        layout.SetRow(4 , 0b110000000000000011);
        layout.SetRow(5 , 0b110000000000000011);
        layout.SetRow(6 , 0b110000000000000011);
        layout.SetRow(7 , 0b110000000000000011);
        layout.SetRow(8 , 0b111111111111111111);
        layout.SetRow(9 , 0b111111111111111111);
        layout.SetRow(10, 0b000000001100000000);
        layout.SetRow(11, 0b000000001100000000);
        layout.SetRow(12, 0b000000001100000000);
        layout.SetRow(13, 0b000000001100000000);
        layout.SetRow(14, 0b000000001100000000);
        layout.SetRow(15, 0b000000001100000000);
        layout.SetRow(16, 0b111111111111111111);
        layout.SetRow(17, 0b111111111111111111);
        layout.SetRow(18, 0b000000000000000000);
        layout.SetRow(19, 0b000000000000000000);
        layout.SetRow(20, 0b000000000000000000);
        layout.SetRow(21, 0b000000000000000000);
    }
}
