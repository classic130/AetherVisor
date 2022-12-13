#include "logging.h"
#include "paging_utils.h"
#include "disassembly.h"
#include "portable_executable.h"
#include "vmexit.h"
#include "utils.h"
#include "npt_hook.h"

namespace BranchTracer
{
	void Init(VcpuData* vcpu_data, uintptr_t start_addr, uintptr_t log_buffer, int log_buffer_size);

	void Start(VcpuData* vcpu_data);
	void Stop(VcpuData* vcpu_data);
	void Start();

	void Init(VcpuData* vcpu_data, uintptr_t start_addr, uintptr_t out_buffer);

	extern bool active;	
	extern bool initialized;

	extern uintptr_t start_address;
	extern HANDLE thread_id;
	extern NPTHooks::NptHook* capture_thread_bp;	/*	stealth breakpoint to capture the thread executing a function	*/

	struct BranchLog
	{
		int capacity;
		int buffer_idx;

		uintptr_t* buffer;

		void Log(uintptr_t entry)
		{
			if (buffer_idx < (capacity / sizeof(uintptr_t)))
			{
				buffer[buffer_idx] = entry;
				buffer_idx += 1;
			}
			else
			{
				/*	overwrite starting from the beginning...	*/

				buffer_idx = 0;
			}
		}
	};

	extern BranchLog* log_buffer;
};
