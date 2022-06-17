/**
 * \file	Tutorial/TutorialRenderer.h
 *
 * An abstract class defining methods for rendering parts in tutorial window
 */
#pragma once

// #include "Tutorial.h"
// forward declarations from Tutorial.h to avoid cyclic dependency
struct Explanation;
struct Task;
struct ChoiceTask;
struct MultiChoiceTask;
struct InputTask;
struct Hint;


class ITutorialRenderer
{
public:
	virtual ~ITutorialRenderer() = default;

	virtual void renderExplanation(Explanation* explanation)             = 0;
	virtual void renderTask(Task* task)                                  = 0;
	virtual void renderHint(Hint* hint)                                  = 0;
	virtual void renderChoiceTask(ChoiceTask* choiceTask)                = 0;
	virtual void renderMultiChoiceTask(MultiChoiceTask* multiChoiceTask) = 0;
	virtual void renderInputTask(InputTask* inputTask)                   = 0;
};
