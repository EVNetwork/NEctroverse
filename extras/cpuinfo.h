/* *****************************************************************************
 *
 * Copyright (c) 2007-2013 Alexis Naveros.
 * Portions developed under contract to the SURVICE Engineering Company.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this file; see the file named COPYING for more
 * information.
 *
 * *****************************************************************************
 */

#ifndef CPUINFO_H
#define CPUINFO_H


typedef struct
{
  uint32_t endianness;
  uint32_t intellevel;
  uint32_t amdlevel;

  uint32_t arch;
  char vendorstring[12+1];
  char identifier[48+1];

  uint32_t vendor;
  uint32_t class;
  uint32_t family;
  uint32_t model;
  uint32_t cacheline;
  uint32_t clflushsize;

  int32_t cachelineL1code, cachesizeL1code, cacheassociativityL1code, cachesharedL1code;
  int32_t cachelineL1data, cachesizeL1data, cacheassociativityL1data, cachesharedL1data;
  int32_t cacheunifiedL1;
  int32_t cachelineL2, cachesizeL2, cacheassociativityL2, cachesharedL2;
  int32_t cachelineL3, cachesizeL3, cacheassociativityL3, cachesharedL3;

  int64_t sysmemory;
  uint32_t socketphysicalcores;
  uint32_t socketlogicalcores;
  uint32_t socketcount;
  uint32_t totalcorecount;
  uint32_t wordsize;
  uint32_t gpregs;
  uint32_t fpregs;


  /* CPU capabilities */
  int capcmov:1;
  int capclflush:1;
  int captsc:1;
  int capmmx:1;
  int capmmxext:1;
  int cap3dnow:1;
  int cap3dnowext:1;
  int capsse:1;
  int capsse2:1;
  int capsse3:1;
  int capssse3:1;
  int capsse4p1:1;
  int capsse4p2:1;
  int capsse4a:1;
  int capavx:1;
  int capavx2:1;
  int capxop:1;
  int capfma3:1;
  int capfma4:1;
  int capmisalignsse:1;
  int capaes:1;
  int cappclmul;
  int caprdrnd:1;
  int capcmpxchg16b:1;
  int cappopcnt:1;
  int caplzcnt:1;
  int capmovbe:1;
  int caprdtscp:1;
  int capconstanttsc:1;
  int capf16c:1;
  int capbmi:1;
  int capbmi2:1;
  int captbm:1;
  int caphyperthreading:1;
  int capmwait:1;
  int caplongmode:1;
  int capthermalsensor:1;
  int capclockmodulation:1;
  /* CPU capabilities */

} cpuInfo;


void cpuGetInfo( cpuInfo *cpu );

void cpuGetInfoEnv( cpuInfo *cpu );


////


enum
{
  CPUINFO_LITTLE_ENDIAN,
  CPUINFO_BIG_ENDIAN,
  CPUINFO_MIXED_ENDIAN
};


enum
{
  CPUINFO_ARCH_AMD64,
  CPUINFO_ARCH_IA32,
  CPUINFO_ARCH_UNKNOWN,
  CPUINFO_ARCH_COUNT //Addition by Necro
};

extern char *cpuArchName[CPUINFO_ARCH_COUNT]; //Addition by Necro

enum
{
  CPUINFO_VENDOR_AMD,
  CPUINFO_VENDOR_INTEL,
  CPUINFO_VENDOR_UNKNOWN,
  CPUINFO_VENDOR_COUNT //Addition by Necro
};

extern char *cpuVendorName[CPUINFO_VENDOR_COUNT]; //Addition by Necro

enum
{
  CPUINFO_CLASS_STEAMROLLER,
  CPUINFO_CLASS_JAGUAR,
  CPUINFO_CLASS_PILEDRIVER,
  CPUINFO_CLASS_BULLDOZER,
  CPUINFO_CLASS_BOBCAT,
  CPUINFO_CLASS_BARCELONA,
  CPUINFO_CLASS_ATHLON64SSE3,
  CPUINFO_CLASS_ATHLON64,
  CPUINFO_CLASS_ATHLON,
  CPUINFO_CLASS_K6_2,
  CPUINFO_CLASS_K6,

  CPUINFO_CLASS_COREI7AVX2,
  CPUINFO_CLASS_COREI7AVX,
  CPUINFO_CLASS_COREI7,
  CPUINFO_CLASS_CORE2,
  CPUINFO_CLASS_NOCONA,
  CPUINFO_CLASS_PRESCOTT,
  CPUINFO_CLASS_PENTIUM4,
  CPUINFO_CLASS_PENTIUM3,
  CPUINFO_CLASS_PENTIUM2,

  CPUINFO_CLASS_I686,
  CPUINFO_CLASS_I586,

  CPUINFO_CLASS_AMD64GENERIC,
  CPUINFO_CLASS_IA32GENERIC,
  CPUINFO_CLASS_UNKNOWN,

  CPUINFO_CLASS_COUNT
};

extern char *cpuClassName[CPUINFO_CLASS_COUNT]; //Addition by Necro

#endif /* CPUINFO_H */


