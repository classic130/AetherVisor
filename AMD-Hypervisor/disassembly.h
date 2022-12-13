#pragma once
#include "includes.h"
#include "hypervisor.h"

namespace Disasm
{
	static ZydisDecoder zydis_decoder;

	ZydisDecodedInstruction Disassemble(
		uint8_t* instruction, 
		ZydisDecodedOperand* operands
	);

	/*	Gets total instructions length closest to byte_length	*/
	int	LengthOfInstructions(
		void* address, 
		int byte_length
	);

	ZyanU64 GetMemoryAccessTarget(
		ZydisDecodedInstruction& instruction,
		ZydisDecodedOperand* operands,
		ZyanU64 runtime_address,
		ZydisRegisterContext* registers
	);

	void MyRegContextToZydisRegContext(
		VcpuData* vcpu_data, 
		GuestRegisters* guest_regs, 
		ZydisRegisterContext* context
	);


	uint8_t* ForEachInstruction(uint8_t* start, uint8_t* end, bool(*Callback)(uint8_t* insn_addr, ZydisDecodedInstruction instruction));

	int Init();
};