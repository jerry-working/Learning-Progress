#pragma once

extern "C" __declspec(dllexport) int Z3DemoInit();
extern "C" __declspec(dllexport) int Z3DemoExit();

int FileCreate();
int FuncB();
int FileCreateTemplate();
int FileHist();

void rectorInit();
void rectorUnload();

