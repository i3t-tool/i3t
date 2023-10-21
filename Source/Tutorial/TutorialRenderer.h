/**
 * \file
 * \brief An abstract class defining methods for rendering parts in tutorial window
 * \author Miroslav Müller
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

// forward declarations from Tutorial.h to avoid cyclic dependency
struct Explanation;
struct Headline;
struct Task;
struct ChoiceTask;
struct MultiChoiceTask;
struct InputTask;
struct Hint;

class ITutorialRenderer
{
public:
	virtual ~ITutorialRenderer() = default;

	virtual void renderExplanation(Explanation* explanation) = 0;
	virtual void renderHeadline(Headline* headline) = 0;
	virtual void renderTask(Task* task) = 0;
	virtual void renderHint(Hint* hint) = 0;
	virtual void renderChoiceTask(ChoiceTask* choiceTask) = 0;
	virtual void renderMultiChoiceTask(MultiChoiceTask* multiChoiceTask) = 0;
	virtual void renderInputTask(InputTask* inputTask) = 0;
};
