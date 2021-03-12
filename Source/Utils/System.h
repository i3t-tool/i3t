/**
 * \file Utils/System.h
 * \author Martin Herich hericmar@fel.cvut.cz
 * \date 4.11.2020
 *
 * Useful operating system related utilities.
 */
#pragma once

/**
 * \brief Run a shell command - used for url and mailto in AbInfoTab of AboutForm

 * \param sexe The command to run in the shell
 */
void runShell(const char* sexe);
bool openFileDialog(char*filename,int bufsize);