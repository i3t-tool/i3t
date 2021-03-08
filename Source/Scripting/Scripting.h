#pragma once
//----------------
/**
 * \file Scripting.h
 * \author Daniel Gruncl
 * \brief Interpreter of c scripts.
 */
 //---------------
#include "GUI/Elements/Nodes/WorkspaceElements.h"
#include "GUI/Elements/Nodes/WorkspaceMatrix4x4.h"
#include <vector>
#include <memory>

void LoadWorkspace(const char*filename, std::vector<std::unique_ptr<WorkspaceNode>>*_workspace);
void SaveWorkspace(const char*filename, std::vector<std::unique_ptr<WorkspaceNode>>*_workspace);
//std::vector<WorkspaceMatrix4x4>* LoadWorkspace();
int PicocRunInteractive();
int PicocRunFile(const char* filename);
int PicocRunSource(const char* source);