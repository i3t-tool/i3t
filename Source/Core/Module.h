#pragma once

class Module
{
	friend class Application;

public:
	Module() = default;
	virtual ~Module() = default;
	virtual void onInit() {}
	virtual void onBeginFrame() {}
	virtual void onEndFrame() {}
	virtual void onUpdate(double delta) {}
	virtual void onClose() {}
};
