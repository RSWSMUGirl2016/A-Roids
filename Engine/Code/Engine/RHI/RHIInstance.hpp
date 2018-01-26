#pragma once
#include "Engine/RHI/RHIOutput.hpp"

typedef bool(*WindowMessageHandler)(HWND, UINT, WPARAM, LPARAM);

class RHIInstance
{
public:
	~RHIInstance();

	RHIOutput* CreateOutput(WindowMessageHandler windowMessageHandler);

private:	
	RHIInstance();

public:
	static RHIInstance *m_rhiInstance;
	static RHIInstance* GetInstance();
};
