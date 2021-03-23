#pragma once
//----------------
/**
 * \file Scripting.h
 * \author Daniel Gruncl
 * \brief Interpreter of c scripts.
 */
 //---------------
#include "Dependencies/picoc/picoc.h"

#include "GUI/Elements/Nodes/WorkspaceElements.h"
//#include "GUI/Elements/Nodes/WorkspaceMatrix4x4.h"
#include <vector>
#include <memory>

#define PICOC_STACK_SIZE (32*1024)       /* space for the the stack */

bool LoadWorkspace(const char*filename, std::vector<std::unique_ptr<WorkspaceNode>>*_workspace);
bool SaveWorkspace(const char*filename, std::vector<std::unique_ptr<WorkspaceNode>>*_workspace);
//std::vector<WorkspaceMatrix4x4>* LoadWorkspace();
int PicocRunInteractive();
int PicocRunFile(const char* filename);
int PicocRunSource(const char* source);

//struct Picoc;

class Scripting {
public:
	Scripting();
	~Scripting();
	void runCommand(std::string);
private:
	Picoc m_picoc;
	bool m_picocExitPoint;

};