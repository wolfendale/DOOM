// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//
//-----------------------------------------------------------------------------

static const char rcsid[] = "$Id: m_bbox.c,v 1.1 1997/02/03 22:45:10 b1 Exp $";

#ifdef _MSC_VER
#else
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#endif 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <stdarg.h>

#include "doomdef.h"
#include "i_sound.h"
#include "i_video.h"
#include "m_misc.h"

#include "d_net.h"
#include "g_game.h"

#ifdef __GNUG__
#pragma implementation "i_system.h"
#endif
#include "i_system.h"

int mb_used = 6;

#ifdef _MSC_VER
typedef struct timeval {
  uint32_t tv_sec;
  uint32_t tv_usec;
  uint32_t tv_nsec;
} timeval;
#define timespec timeval
void gettimeofday(timeval* time, timeval* time2) {
  time->tv_sec = 0;
  time->tv_usec = 0;
}
void nanosleep(timespec *remaining, timespec *request) {
}
#endif


void I_Tactile(int on, int off, int total) {
  // UNUSED.
  on = off = total = 0;
}

ticcmd_t emptycmd;
ticcmd_t *I_BaseTiccmd(void) { return &emptycmd; }

int I_GetHeapSize(void) { return mb_used * 1024 * 1024; }

byte *I_ZoneBase(int *size) {
  *size = mb_used * 1024 * 1024;
  return (byte *)malloc(*size);
}

//
// I_GetTime
// returns time in 1/70th second tics
//
int I_GetTime(void) {
  struct timeval tp;
  int newtics;
  static int basetime = 0;

  gettimeofday(&tp, NULL);
  if (!basetime)
    basetime = tp.tv_sec;
  newtics = (tp.tv_sec - basetime) * TICRATE + tp.tv_usec * TICRATE / 1000000;
  return newtics;
}

//
// I_Init
//
void I_Init(void) {
  I_InitSound();
  //  I_InitGraphics();
}

//
// I_Quit
//
void I_Quit(void) {
  D_QuitNetGame();
  I_ShutdownSound();
  I_ShutdownMusic();
  M_SaveDefaults();
  I_ShutdownGraphics();
  exit(0);
}

void I_WaitVBL(int count) {
  struct timespec remaining, request = {
    .tv_sec = 0,
    .tv_nsec = count * (1000000000 / 70)
  };
  nanosleep(&remaining, &request);
}

void I_BeginRead(void) {}

void I_EndRead(void) {}

byte *I_AllocLow(int length) {
  byte *mem;

  mem = (byte *)malloc(length);
  memset(mem, 0, length);
  return mem;
}

//
// I_Error
//
extern boolean demorecording;

void I_Error(char *error, ...) {
  va_list argptr;

  // Message first.
  va_start(argptr, error);
  fprintf(stderr, "Error: ");
  vfprintf(stderr, error, argptr);
  fprintf(stderr, "\n");
  va_end(argptr);

  fflush(stderr);

  // Shutdown. Here might be other errors.
  if (demorecording)
    G_CheckDemoStatus();

  D_QuitNetGame();
  I_ShutdownGraphics();

  #ifdef _MSC_VER
  __debugbreak();
  #endif

  exit(-1);
}
