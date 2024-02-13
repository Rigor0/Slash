#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, true);
#define DRAW_SPHERE_SingleFrame(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, -1.f);
#define DRAW_LINE(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, true);
#define DRAW_LINE_SingleFrame(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, -1.f, 1.f);
#define DRAW_POINT(Position) if(GetWorld()) DrawDebugPoint(GetWorld(), Position, 10, FColor::Green, true);
#define DRAW_POINT_SingleFrame(Position) if(GetWorld()) DrawDebugPoint(GetWorld(), Position, 10, FColor::Green, false, -1.f);
#define DRAW_VECTOR(StartLocation, EndLocation) if(GetWorld()) \
    { \
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, true); \
        DrawDebugPoint(GetWorld(), StartLocation, 10, FColor::Green, true); \
    }
#define DRAW_VECTOR_SingleFrame(StartLocation, EndLocation) if(GetWorld()) \
    { \
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, -1.f, 0, 1.f); \
        DrawDebugPoint(GetWorld(), StartLocation, 10, FColor::Green, false, -1.f); \
    }
