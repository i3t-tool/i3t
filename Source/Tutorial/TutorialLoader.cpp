/**
 * \file
 * \brief
 * \authors Miroslav Müller, Adam Loucký
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "TutorialLoader.h"

#include <filesystem>
#include <fstream>

#include "stb_image.h"
#include "yaml-cpp/yaml.h"

#include "Logger/Logger.h"
#include "Tutorial/Tutorial.h"
#include "Utils/FilesystemUtils.h"

std::shared_ptr<TutorialHeader> TutorialLoader::loadTutorialHeader(std::string& path)
{
	// PARSE GENERAL INFO (YAML)
	YAML::Node tutorial_yaml;
	try
	{
		tutorial_yaml = YAML::LoadFile(path);
	}
	catch (...)
	{
		LOG_FATAL("Tutorial file '" + path + "' not found or YAML header unparsable");
		return nullptr; // return nothing
	}
	// std::cout << tutorial_yaml << std::endl;

	// title
	std::string title = "undefined";
	if (tutorial_yaml["title"])
	{
		title = tutorial_yaml["title"].as<std::string>();
		LOG_DEBUG(title);
	}
	else
	{
		LOG_FATAL("Tutorial title not specified");
	}
	// description
	std::string description = "undefined";
	if (tutorial_yaml["description"])
	{
		description = tutorial_yaml["description"].as<std::string>();
		LOG_DEBUG(description);
	}
	else
	{
		LOG_FATAL("Tutorial description not specified");
	}
	// scene
	std::string scene = "undefined";
	if (tutorial_yaml["scene"])
	{
		scene = "Data/Tutorials/" + tutorial_yaml["scene"].as<std::string>();
		LOG_DEBUG(scene);
	}
	else
	{
		LOG_DEBUG("Tutorial scene not specified");
	}

	// thumbnail
	std::shared_ptr<GUIImage> thumbnail = nullptr; // dummy image here? - NOPE rather later when rendering and
	                                               // encountering a nullptr (safer in case of loader errors)
	if (tutorial_yaml["thumbnail"])
	{
		thumbnail = loadImage(getDirectory(path) + tutorial_yaml["thumbnail"].as<std::string>());
	}
	else
	{
		LOG_FATAL("Thumbnail not specified");
	}
	// language
	Language lang = Language::English;
	if (tutorial_yaml["language"])
	{
		std::string langStr = tutorial_yaml["language"].as<std::string>();
		LOG_DEBUG(langStr);
		// switch (langStr) {
		//	todo
		// }
	}
	else
	{
		LOG_INFO("Language not specified - setting english");
		// todo
	}

	// we create our tutorial header object on heap, we are using shared ptr, so
	// that when there aren't any references, we can eg properly free the loaded
	// image and destroy it
	return std::make_shared<TutorialHeader>(std::move(path), std::move(title), std::move(description), std::move(scene),
	                                        std::move(thumbnail));
}

std::shared_ptr<Tutorial> TutorialLoader::loadTutorial(std::shared_ptr<TutorialHeader> header)
{
	// CHECK
	std::ifstream tutorialStream(header->m_filename);
	if (!tutorialStream.good())
	{
		LOG_FATAL("Tutorial file '{}' was not found", header->m_filename.string());
		return nullptr; // return nothing
	}
	// FOR TUT CREATION DEBUG
	int lineNumber = 0;

	// SKIP YAML (MOVE STREAM POINTER PAST IT)
	int yamlSeparatorCount = 0;
	while (yamlSeparatorCount < 2)
	{
		lineNumber++;
		// GET FIRST WORD OF THE LINE AND MOVE TO NEXT ONE
		std::string firstWord;
		tutorialStream >> firstWord;
		if (firstWord == "---")
		{
			yamlSeparatorCount++; // if its a --- then note it
		}
		tutorialStream.ignore(std::numeric_limits<std::streamsize>::max(),
		                      tutorialStream.widen('\n')); // skip to the next line
		// CHECK
		if (!tutorialStream.good())
		{
			if (tutorialStream.eof())
			{
				LOG_FATAL("Tutorial file '{}'"
				          "' missing 2 '---' YAML marks at the beginning of file or "
				          "no further content behind them",
				          header->m_filename.string());
			}
			else
			{
				LOG_FATAL("Tutorial file '{}' I/O error", header->m_filename.string());
			}
			return nullptr;
		}
	}

	// PREPARE PARSING MACHINE
	std::vector<TutorialStep> steps; // we will be filling this vector and then creating
	                                 // a tutorial with it
	steps.emplace_back();            // add the first step
	int currentStep = 0;
	// int currentBlockIndent = -1;  // deprecated
	auto currentBlock = EBlockType::NOT_BLOCK;
	std::string line;
	// temporaries for accumulating multiple line content:
	std::string textStore;
	std::vector<std::string> vectorOfTextsStore;
	int numberStore = -1;
	std::vector<int> vectorOfNumsStore;

	// -------------------------------------------------------

	// [FUNCS] DETERMINING ELEMENT TYPES
	auto getBlockType = [](const std::string& string) {
		using enum EBlockType;

		// is keyword
		static const std::unordered_map<std::string, EBlockType> stringToBlockType = {
		    {"task:", TASK},   {"hint:", HINT},     {"choice:", CHOICE},     {"multichoice:", MULTICHOICE},
		    {"input:", INPUT}, {"script:", SCRIPT}, {"headline:", HEADLINE}, {"scriptFile:", SCRIPT_FILE}};
		if (const auto it{stringToBlockType.find(string)}; it != std::end(stringToBlockType))
		{
			return it->second;
		}
		// is anything else
		return NOT_BLOCK;
	};
	auto getSingleLineType = [](const std::string& string) {
		using enum ESingleLineType;

		// is keyword
		static const std::unordered_map<std::string, ESingleLineType> stringToSingleLineType = {
		    {"task:", TASK_SINGLE},         {"hint:", HINT_SINGLE},      {"x:", CORRECT_ANSWER},
		    {"o:", WRONG_ANSWER},           {"answers:", ANSWER_LIST},   {"script:", SCRIPT_SINGLE},
		    {"headline:", HEADLINE_SINGLE}, {"scriptFile:", SCRIPT_FILE}};
		if (const auto it{stringToSingleLineType.find(string)}; it != std::end(stringToSingleLineType))
		{
			return it->second;
		}
		// is anything else
		return NOT_SINGLE_LINE;
	};

	// do i need this funtion? i think i dont even need a switch, i just add to
	// the stores auto addToCurrentBlock = [&](const std::string& string) -> void
	// {
	//  switch (currentBlock) {
	//  case EXPLANATION:
	//    textStore
	//  }
	//};

	// [FUNC] FILLING THE STEP CLASS WITH COMPLETED BLOCK ELEMENTS
	auto endCurrentBlock = [&]() -> void {
		using enum EBlockType;

		switch (currentBlock)
		{
		case EXPLANATION:
			createExplanation(steps[currentStep], textStore);
			break;
		case TASK:
			createTask(steps[currentStep], textStore);
			break;
		case HINT:
			createHint(steps[currentStep], textStore);
			break;
		case SCRIPT:
			addScript(steps[currentStep], textStore);
			break;
		case SCRIPT_FILE:
		{
			const auto scriptPath = getDirectory(header->m_filename) + textStore;
			auto readFileResult = FilesystemUtils::readFile(scriptPath);
			if (readFileResult)
			{
				auto scriptSource = readFileResult.value();
				addScript(steps[currentStep], scriptSource);
			}
			else
			{
				LOG_FATAL("Cannot load script file '{}': {}", textStore, readFileResult.error());
			}
			break;
		}
		case HEADLINE:
			createHeadline(steps[currentStep], textStore);
			break;
		case NOT_BLOCK:
			// LOG_DEBUG("Ending block when NOT_BLOCK");
			break;
		default:
			LOG_INFO("Creation of tutorial block element {} not implemented yet", magic_enum::enum_name(currentBlock));
		}

		currentBlock = NOT_BLOCK;
	};

	// [FUNC] INITIATING BLOCK ELEMENTS
	auto beginBlock = [&](EBlockType blockType) -> void {
		// drop current block if any (safety check)
		if (currentBlock != EBlockType::NOT_BLOCK)
		{
			endCurrentBlock();
		}
		// set active block
		currentBlock = blockType;
		// reset temporary block variables
		textStore.clear();
		vectorOfTextsStore.clear();
		numberStore = -1;
		vectorOfNumsStore.clear();
		// currentBlockIndent = -1; // deprecated
	};

	// [FUNC] FILLING THE STEP CLASS WITH SINGLE-LINE ELEMENTS
	auto handleSingleLine = [&](ESingleLineType type, const std::string& content) -> void {
		using enum ESingleLineType;
		// check also for current state, and show error when calling singlelines
		// which do not match nektere pripady ponechavaji state, jine ho musi
		// resetovat!

		// createTask(steps[currentStep], std::move(oneLineContent));
		//  oneLineContent = ""; // unnecessary since i reinit it every while

		switch (type)
		{
		case TASK_SINGLE:
			endCurrentBlock();
			createTask(steps[currentStep], content);
			break;
		case HINT_SINGLE:
			endCurrentBlock();
			createHint(steps[currentStep], content);
			break;
		case SCRIPT_SINGLE:
			endCurrentBlock();
			addScript(steps[currentStep], content);
			break;
		case SCRIPT_FILE:
		{
			endCurrentBlock();

			const auto scriptPath = getDirectory(header->m_filename) + content;
			auto readFileResult = FilesystemUtils::readFile(scriptPath);
			if (readFileResult)
			{
				auto scriptSource = readFileResult.value();
				addScript(steps[currentStep], scriptSource);
			}
			else
			{
				LOG_FATAL("Cannot load script file '{}': {}", content, readFileResult.error());
			}

			break;
		}
		case HEADLINE_SINGLE:
			endCurrentBlock();
			createHeadline(steps[currentStep], content);
			break;
		default:
			LOG_INFO("Creation of single-line tutorial element {} not implemented yet", magic_enum::enum_name(type));
		}
	};
	// -------------------------------------------------------

	// READ LINES
	while (std::getline(tutorialStream, line).good())
	{
		lineNumber++;
		// LOG_INFO("[{}] {}", lineNumber, line);

		// PROCESS LINE
		// make a stream again to be able to move through it
		std::istringstream lineStream(line);
		// skip spaces
		skipSpaces(lineStream);
		// tell how much indentation did we skip
		int indent = static_cast<int>(lineStream.tellg());
		// save first word
		std::string firstWord;
		lineStream >> firstWord;
		// special case when empty
		if (firstWord.empty())
		{
			endCurrentBlock();
			continue;
		}
		// special case when start of a new step
		if (firstWord == "--")
		{
			endCurrentBlock();
			steps.emplace_back();
			currentStep++;
			continue;
		}
		// special case for comments
		if (firstWord == "%")
		{
			continue; // comments do not end blocks
		}
		// recognize type of line / command
		EBlockType blockType = getBlockType(firstWord);
		ESingleLineType singleLineType = getSingleLineType(firstWord);
		// skip possible spaces between keyword and content if any
		skipSpaces(lineStream);
		// get any possible remaining text
		std::string restOfLine;
		std::getline(lineStream, restOfLine);
		// handle single-lines
		if (singleLineType != ESingleLineType::NOT_SINGLE_LINE)
		{
			// actually used as single-line
			if (!restOfLine.empty())
			{
				// take care of the single line
				handleSingleLine(singleLineType, restOfLine);
				continue;
			}
			// actually used as block but the type does not allow blocks
			else if (blockType == EBlockType::NOT_BLOCK)
			{
				LOG_FATAL("NOT A BLOCK COMMAND: " + line);
				// todo show errors to creator
			}
		}
		// handle block starts
		if (blockType != EBlockType::NOT_BLOCK)
		{
			// actually used as block start
			if (restOfLine.empty())
			{
				beginBlock(blockType);
			}
			// actually used as single line but the type does not allow single lines
			// (implicitly by if order)
			else
			{
				LOG_FATAL("NOT A SINGLELINE COMMAND: " + line);
				// todo show errors to creator
			}
		}
		// handle plain text
		else
		{
			// no current block -> start explanation
			if (currentBlock == EBlockType::NOT_BLOCK)
			{
				beginBlock(EBlockType::EXPLANATION);
			}
			// reset stream to start
			lineStream.clear();
			lineStream.seekg(0);
			// block indentation - skip 2 spaces if not simple explanation
			if (currentBlock != EBlockType::EXPLANATION)
			{
				if (indent >= 2)
				{
					// LOG_INFO("blockIndent: {}, indent: {}, diff: {}",
					// currentBlockIndent, indent, indent - currentBlockIndent);
					skipSpaces(lineStream, 2);
				}
				else
				{
					LOG_INFO("Unexpected block unindent at line [{}] {}", lineNumber, line);
				}
			}
			// add to active block
			std::getline(lineStream, restOfLine);
			textStore += restOfLine;
			textStore += '\n';
		}
	}
	// bug fix todo - if there is a non-empty line just before EOF then it wont
	// get added (there has to be eg an empty line after it) FINISH UNFINISHED
	// BLOCKS
	if (currentBlock != EBlockType::NOT_BLOCK)
	{
		endCurrentBlock();
	}

	// CHECK if parsing ended because of error
	if (!tutorialStream.eof())
	{
		LOG_FATAL("Tutorial file '{}' I/O error", header->m_filename.string());
	}

	// CREATE THE TUTORIAL
	std::shared_ptr<Tutorial> tutorial = std::make_shared<Tutorial>(
	    std::move(header), std::move(steps),
	    std::unordered_map<std::string, std::shared_ptr<GUIImage>>()); // we create our tutorial
	                                                                   // object on heap

	return tutorial;
}

// TutorialLoader::blockType_t TutorialLoader::isKeyword(const std::string&
// string)
//{
//   // is empty
//   if (string.empty()) {
//     return EMPTY;
//   }
//   // is keyword
//   static const std::unordered_map<std::string, blockType_t> stringToKeyword =
//   {
//     { "--", STEP_START },
//     { "task:", TASK },
//     { "hint:", HINT },
//     { "choice:", CHOICE },
//     { "multichoice:", MULTICHOICE },
//     { "input:", INPUT }
//   };
//   if (const auto it{ stringToKeyword.find(string) }; it !=
//   std::end(stringToKeyword)) {
//     return it->second;
//   }
//   // is anything else
//   return EXPLANATION;
// }

// todo make it a universal function (not belonging to tutorialLoader)
std::shared_ptr<GUIImage> TutorialLoader::loadImage(const std::string& path)
{
	// NOTE: we dont load dummy images here - let renderer handle nullptrs
	try
	{
		return std::make_shared<GUIImage>(path);
	}
	catch (std::runtime_error& e)
	{
		LOG_FATAL(e.what());
		return nullptr;
	}
}

std::string TutorialLoader::getDirectory(const std::filesystem::path& path)
{
	return path.parent_path().string() + "/";
}

void TutorialLoader::skipSpaces(std::istringstream& stream)
{
	while (stream.peek() == ' ' || stream.peek() == '\t')
	{                    // check if space
		stream.ignore(); // skip the character
	}
}

void TutorialLoader::skipSpaces(std::istringstream& stream, unsigned int maxCount)
{
	for (int i = 0; i < maxCount; i++)
	{
		if (stream.peek() == ' ' || stream.peek() == '\t')
		{                    // check if space
			stream.ignore(); // skip the character
		}
		else
		{
			break;
		}
	}
}

std::shared_ptr<TutorialElement>& TutorialLoader::createExplanation(TutorialStep& step, const std::string& string)
{
	return step.m_content.emplace_back(std::make_shared<Explanation>(string));
}

std::shared_ptr<TutorialElement>& TutorialLoader::createHeadline(TutorialStep& step, const std::string& string)
{
	return step.m_content.emplace_back(std::make_shared<Headline>(string));
}

std::shared_ptr<TutorialElement>& TutorialLoader::createTask(TutorialStep& step, const std::string& string)
{
	return step.m_content.emplace_back(std::make_shared<Task>(string));
}

std::shared_ptr<TutorialElement>& TutorialLoader::createHint(TutorialStep& step, const std::string& string)
{
	return step.m_content.emplace_back(std::make_shared<Hint>(string));
}

std::shared_ptr<TutorialElement>& TutorialLoader::createChoice(TutorialStep& step, const std::string& question,
                                                               const std::vector<std::string>& choices,
                                                               int correctChoice)
{
	return step.m_content.emplace_back(std::make_shared<ChoiceTask>(question, choices, correctChoice));
}

std::shared_ptr<TutorialElement>& TutorialLoader::createMultichoice(TutorialStep& step, const std::string& question,
                                                                    std::vector<std::string>& choices,
                                                                    const std::vector<int>& correctChoices)
{
	return step.m_content.emplace_back(std::make_shared<MultiChoiceTask>(question, choices, correctChoices));
}

std::shared_ptr<TutorialElement>& TutorialLoader::createInput(TutorialStep& step, const std::string& question,
                                                              const std::unordered_set<std::string>& correctAnswers)
{
	// todo
	return step.m_content.emplace_back(std::make_shared<InputTask>(question, correctAnswers));
}

void TutorialLoader::addScript(TutorialStep& step, const std::string& script)
{
	step.m_scriptToRunWhenShown += script;
}

/*
  // nicely fill it at that heap place ^^
  // if this looks like magic, feel free to change it, but remember not to copy
  unique_ptrs! tutorial->m_steps.emplace_back(TStep("Krok
  1")).m_content.push_back(std::make_unique<TWText>("yay"));
  // the first emplace returns a refernce to that newly created TStep here
  // the second can either be push or emplace, since both will automatically
  move instead of copy
  // make unique creates the <> specified object with passed arguments on heap
  (sadly a separate place) and returns the unique_ptr to it (or a referemnce to
  the uptr)

  // important is only to create the TStep with its vector first, and only then
  fill it with uptrs to widgets (not the other way around, since they would need
  to get copied into it)
  // another way of doing it:
  TStep& step = tutorial->m_steps.emplace_back(TStep("Krok 2"));
  step.m_content.push_back(std::make_unique<TWText>("fldsmdfr"));
  step.m_content.push_back(std::make_unique<TWText>("haha"));
  // WARNING! do not attempt to reassign the reference!

  TStep& step2 = tutorial->m_steps.emplace_back(TStep("Krok 3"));
  step2.m_content.push_back(std::make_unique<TWText>("juj"));
  step2.m_content.push_back(std::make_unique<TWText>("omg"));
  */
