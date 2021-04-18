#pragma once

class Module
{
	friend class Application;

public:
	Module(){};
	virtual ~Module(){};
	virtual void init(){};
	virtual void beginFrame(){};
	virtual void endFrame(){};
};
