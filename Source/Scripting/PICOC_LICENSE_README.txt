Extending picoc with custom libraries
-----
Create function
libraryxxx(Picoc *pc);
and add call to it in
PicocInitialise(Picoc *pc, int StackSize) (file platform.c)

How to create custom data types and functions:
Here are some snippets from libraryI3T.cpp:

    void parentGameObject(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs){//custom function
        void *a = Param[0]->Val->Pointer; //read parameter 0 as int
        void *b = Param[1]->Val->Pointer; //read parameter 1 as int
        ReturnValue->Val->Integer=FALSE; //set return value as int (==bool)
    }
    struct LibraryFunction PlatformLibrary1[] =
    {
	    { parentGameObject, "bool parentGameObject(GameObject*,GameObject*);" },
        { NULL,         NULL }
    };
    const char defs[]="typedef void GameObject;typedef int bool;typedef void node;";
    int mat4scale=1;
    void PlatformLibraryInitI3T(Picoc *pc){
        IncludeRegister(pc, "I3T.h", NULL, &PlatformLibrary1[0], defs);// add custom functions and type definitions
    
        VariableDefinePlatformVar(pc, NULL, "mat4scale", &pc->IntType, (union AnyValue *)&mat4scale, FALSE);// add readonly variable exposed to script
    }

picoc
-----

PicoC is a very small C interpreter for scripting. It was originally written 
as a script language for a UAV's on-board flight system. It's also very 
suitable for other robotic, embedded and non-embedded applications.

The core C source code is around 3500 lines of code. It's not intended to be 
a complete implementation of ISO C but it has all the essentials. When 
compiled it only takes a few k of code space and is also very sparing of 
data space. This means it can work well in small embedded devices. It's also 
a fun example of how to create a very small language implementation while 
still keeping the code readable.

picoc is now feature frozen. Since it's important that it remain small it's 
intended that no more major features will be added from now on. It's been 
tested on x86-32, x86-64, powerpc, arm, ultrasparc, HP-PA and blackfin 
processors and is easy to port to new targets. 


Copyright
---------

picoc is published under the "New BSD License".
http://www.opensource.org/licenses/bsd-license.php


Copyright (c) 2009-2011, Zik Saleeba
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are 
met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.
      
    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the 
      distribution.
      
    * Neither the name of the Zik Saleeba nor the names of its 
      contributors may be used to endorse or promote products derived 
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
