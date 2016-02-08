#ifndef INITIALIZE
#define INITIALIZE

#include "tracker.h"

void InitTrackInfo(TrackInfo* trackInfo, int track_length, int init_gap);

DescMat* InitDescMat(int height, int width, int nBins);

void ReleDescMat(DescMat* descMat);

void InitDescInfo(DescInfo* descInfo, int nBins, bool isHof, int size, int nxy_cell, int nt_cell);

void InitSeqInfo(SeqInfo* seqInfo, char* video);

void usage();

bool arg_parse(int argc, char** argv);

#endif // INITIALIZE

