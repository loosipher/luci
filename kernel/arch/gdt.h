#pragma once

struct GdtEntry;
struct GdtPointer;

extern void load_segments(void);
void lgdt(void);
