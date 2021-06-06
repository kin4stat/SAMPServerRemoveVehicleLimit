#include "main.h"
#include "urmem.hpp"

const DWORD PATCH_ADDR = 0x814CC84;//0x814CB24; // 0x814CC84 - R3
const char *PATCH_SIG = "\x2D\x90\x01\x00\x00\x3D\xD3\x00\x00\x00\x77\x2D";

logprintf_t logprintf;
extern void *pAMXFunctions;

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
    pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	urmem::sig_scanner scanner;
    urmem::address_t patch_model_address{};
	if (scanner.init(reinterpret_cast<urmem::address_t>(ppData[0]))) {
		if (
            !scanner.find(PATCH_SIG, "xxxxxxxxxxxx", patch_model_address) ||
            !patch_model_address
        ) {
            logprintf("  [Disable vehicle limit]: Plugin failed to load - Signature not found");
			return false;
        }
		else {
			UnProtect(patch_model_address + 10, 2);
			*reinterpret_cast<unsigned short*>(patch_model_address + 10) = 0x9090;
			
		}
		logprintf("  [Disable vehicle limit]: Plugin loaded");
	}
	else {
		logprintf("  [Disable vehicle limit]: Plugin failed to load - Scanner not inited!");
		return false;
	}
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	logprintf("  [Disable vehicle limit]: Plugin was unloaded");
}

void UnProtect(DWORD addr, size_t size)
{
#ifdef WIN32
	DWORD oldProtect;
	VirtualProtect((LPVOID)addr, size, PAGE_EXECUTE_READWRITE, &oldProtect);
#else
	mprotect((void*)(((int)addr / 4096) * 4096), size, PROT_WRITE | PROT_READ | PROT_EXEC);
#endif
}
