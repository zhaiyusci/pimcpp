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

#include "ObservableBase.h"
#include "time.h"


// void
// ObservableVar::Flush()
// {
//   if (Comm.MyProc() == 0)
//     Out.FlushFile();
// }


void 
ObservableClass::WriteInfo()
{
  if (PathData.Path.Communicator.MyProc()==0)
    IOSection.WriteVar("Description",Description);
}

void 
ObservableClass::Read(IOSectionClass &in)
{
  in.ReadVar("Prefactor", Prefactor);
  assert(in.ReadVar("Frequency",Frequency));
  assert(in.ReadVar("Name",Name));
  if(!(in.ReadVar("Description",Description))){
    Description="No description available";
  }
}


void 
ObservableClass::DoEvent()
{
  TimesCalled++;
  if ((TimesCalled % Frequency) == 0)
    Accumulate();
}
