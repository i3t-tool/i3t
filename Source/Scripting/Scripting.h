/**
 * \file Scripting.h
 * \author Daniel Gruncl
 * \brief Interpreter of c scripts.
 * \date 11.3.2021
 */
#pragma once

#include <memory>
#include <vector>

#include "picoc.h"

#include "GUI/Elements/Nodes/WorkspaceElementsWithCoreData.h"

#define PICOC_STACK_SIZE (32 * 1024) /* space for the the stack */

bool loadWorkspace(const char* filename);
bool saveWorkspace(const char* filename, std::vector<Ptr<WorkspaceNodeWithCoreData>>* _workspace);
// std::vector<WorkspaceMatrix4x4>* loadWorkspace();
int picocRunInteractive();
int picocRunFile(const char* filename);
int picocRunSource(const char* source);
void scriptingHelp(int);

/**
 * \class Scripting
 * Contains instance of picoc interpreter. Context of interpreter is kept whole
 * during whole lifetime of Scripting instance, which allows to proces multiple
 * batches of commands, unlike picocRun* functions, which can run only one batch
 * of commands.
 */
class Scripting
{
public:
	Scripting();
	~Scripting();
	void runCommand(std::string);

private:
	Picoc m_picoc;
	bool m_init = false;
};
