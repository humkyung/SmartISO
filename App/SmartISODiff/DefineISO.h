#pragma once

#define	ISO_MAIN_PIPE		1100
#define ISO_SUB_PIPE		1110
#define ISO_DIM_EXT_LINE	1200
#define ISO_DIM_LEADER		1210
#define ISO_DIM_TEXT		1220
#define ISO_GEN_LEADER		1300
#define ISO_GEN_TEXT		1310

const UInt32 MainPipeLevel = 37;
const UInt32 MainPipeColor = 3;
const UInt32 SubPipeLevel = 27;
const UInt32 SubPipeColor = 3;	

const UInt32 DimExtLineLevel = 47;
const UInt32 DimExtLineColor = 0;

const UInt32 DimLeaderLevel = 46;
const UInt32 DimLeaderLevel_1 = 47;
const UInt32 DimLeaderColor = 0;

const UInt32 DimTextLevel = 46;
const UInt32 DimTextColor = 0;

const UInt32 GenLeaderLevel = 48;
const UInt32 GenLeaderColor = 0;

const UInt32 GenTextLevel = 48;
const UInt32 GenTextColor = 0;

//ISO_Area
const double ISO_Area_Low_X = 2.4130; 
const double ISO_Area_Low_Y = 4.8803;
const double ISO_Area_Low_Z = 0.0;

const double ISO_Area_High_X = 31.5927;
const double ISO_Area_High_Y = 33.0433;
const double ISO_Area_High_Z = 0.0;