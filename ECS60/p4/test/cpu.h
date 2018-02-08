// Author: Sean Davis
#ifndef cpuH
#define cpuH

#include "CPURunner.h"

class CPU
{
public:
  CPU();
  void operation(OpCode &opCode, Instruction &instruction, int &buffer);
  void add(OpCode &opCode, Instruction &instruction, int &buffer);
  void track(OpCode &opCode, Instruction &instruction, int &buffer);
  void simplifyList(OpCode &opCode, Instruction &instruction, int &buffer);
  void processEnd(OpCode &opCode, Instruction &instruction, int &buffer);
  void processPurge(OpCode &opCode, Instruction &instruction, int &buffer);
};
#endif
