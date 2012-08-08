/////////////////////////////////////////////////////////////
// Copyright (C) 2003-2006 Bryan Clark and Kenneth Esler   //
//                                                         //
// This program is free software; you can redistribute it  //
// and/or modify it under the terms of the GNU General     //
// Public License as published by the Free Software        //
// Foundation; either version 2 of the License, or         //
// (at your option) any later version.  This program is    //
// distributed in the hope that it will be useful, but     //
// WITHOUT ANY WARRANTY; without even the implied warranty //
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. //  
// See the GNU General Public License for more details.    //
// For more information, please see the PIMC++ Home Page:  //
//           http://pathintegrals.info                     //
/////////////////////////////////////////////////////////////

#include "PermutationCount.h"


void PermutationCountClass::WriteBlock()
{
  //  Array<double,1> CorSum(Correlation.size());
  //  Path.Communicator.Sum(Correlation, CorSum);

  double norm = 1.0 / ((double) NumSamples);
  for (int i = 0; i < SectorCount.size(); i++)
    SectorCount(i) = Prefactor * PathData.Path.Communicator.Sum(SectorCount(i)) * norm;
  SectorCountVar.Write(SectorCount);
  SectorCountVar.Flush();
  SectorCount = 0.0;
  for (int i = 0; i < CycleCount.size(); i++)
    CycleCount(i) = Prefactor * PathData.Path.Communicator.Sum(CycleCount(i)) * norm / PathData.Path.NumParticles();
  CycleCountVar.Write(CycleCount);
  CycleCountVar.Flush();
  CycleCount = 0.0;
  NumSamples = 0;
}


void PermutationCountClass::Read(IOSectionClass &in)
{
  ObservableClass::Read(in);

  // Maximum number of permutation sectors tracked
  int MaxNSectors;
  if(!in.ReadVar("MaxNSectors", MaxNSectors))
    MaxNSectors = 0; // 0 -> Track all sectors

  // Setup Permutation Sectors
  SetupPermSectors(PathData.Path.NumParticles(),MaxNSectors);

 // string speciesName;
 // Species=-1;
////   assert(in.ReadVar("Species1",speciesName));
////   for (int spec=0;spec<PathData.NumSpecies();spec++){ //???what is Species ?
////     if (PathData.Species(spec).Name==speciesName){
////       Species=spec;
////     }
////   }
  if (PathData.Path.Communicator.MyProc()==0){
    IOSection.WriteVar("Type","PermutationCount");
    IOSection.WriteVar("Cumulative", false);
  }

  /// Now write the one-time output variables
  if (PathData.Path.Communicator.MyProc()==0)
    WriteInfo();
  SectorCount.resize(PossPerms.size());
  SectorCount = 0.0;
  CycleCount.resize(PathData.Path.NumParticles());
  CycleCount = 0.0;
  NumSamples = 0;
}

void PermutationCountClass::Accumulate()
{
  TimesCalled++;
  //Move the join to the end so we don't have to worry about permutations
  PathData.MoveJoin(PathData.NumTimeSlices() - 1);
  NumSamples++;

  int PermSector, PermNumber;
  vector<int> ThisPerm;
  GetPermInfo(ThisPerm,PermSector,PermNumber);
  SectorCount(PermSector) += 1;
  for (vector<int>::size_type j=0; j != ThisPerm.size(); j++) {
    CycleCount(ThisPerm[j]-1)++;
  }
}


