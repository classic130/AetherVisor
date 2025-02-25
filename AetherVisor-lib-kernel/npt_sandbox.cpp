
#include "aethervisor_kernel.h"
#include "utils.h"

namespace Aether
{
	namespace Sandbox
	{
        extern "C"
        {
            void DenyPageAccess(void* page_addr, bool allow_reads)
            {
                svm_vmmcall(VMMCALL_ID::deny_sandbox_reads, PAGE_ALIGN(page_addr));
            }

            void DenyRegionAccess(void* base, size_t range, bool allow_reads)
            {
                auto aligned_range = (uintptr_t)PAGE_ALIGN(range + 0x1000);

                for (auto offset = (uint8_t*)base; offset < (uint8_t*)base + aligned_range; offset += PAGE_SIZE)
                {
                    svm_vmmcall(VMMCALL_ID::deny_sandbox_reads, base, offset);
                }
            }

            int SandboxPage(uintptr_t address, uintptr_t tag, bool global_page = false)
            {
                if (global_page)
                {
                    Util::TriggerCOW((uint8_t*)address);
                }

                svm_vmmcall(VMMCALL_ID::sandbox_page, address, tag);

                return 0;
            }


            int UnboxPage(uintptr_t address, uintptr_t tag, bool global_page = false)
            {
                if (global_page)
                {
                    Util::TriggerCOW((uint8_t*)address);
                }

                svm_vmmcall(VMMCALL_ID::unbox_page, address, tag);

                return 0;
            }

            void SandboxRegion(uintptr_t base, uintptr_t size, bool COW)
            {
                for (auto offset = base; offset < base + size; offset += PAGE_SIZE)
                {
                    LARGE_INTEGER interval;
                    interval.QuadPart = -1 * 1000 * 1000; /* 1/10 second */
                    KeDelayExecutionThread(KernelMode, FALSE, &interval);

                    SandboxPage((uintptr_t)offset, NULL, COW);
                }
            }

            void UnboxRegion(uintptr_t base, uintptr_t size, bool global_page)
            {
                for (auto offset = base; offset < base + size; offset += PAGE_SIZE)
                {
                    UnboxPage((uintptr_t)offset, NULL, global_page);
                }
            }}
	}
}
