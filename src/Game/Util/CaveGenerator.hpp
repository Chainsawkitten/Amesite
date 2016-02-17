#pragma once

///TODO: COMMENT THESE
bool** GenerateCaveMap(const int& rowCount, const int& columnCount, const int& iterations);

void PrintMapToLog(bool** map, const int& rowCount, const int& columnCount);

void ProcessCaveMap(bool** map, const int& rowCount, const int& columnCount, const int& iterations);