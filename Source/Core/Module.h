#pragma once

class Module
{
	friend class Application;

public:
	Module() = default;
	virtual ~Module() = default;
	virtual void init() {}
	virtual void beginFrame() {}
	virtual void endFrame() {}
	virtual void onClose() {}
};
