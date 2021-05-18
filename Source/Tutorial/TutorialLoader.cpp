﻿#include "TutorialLoader.h"
#include <iostream>
#include <fstream>
#include <cstdint>
#include <filesystem>
#include "yaml-cpp/yaml.h"
#include "imgui.h"
#include "stb_image.h"
#include "pgr.h"
#include "Logger/Logger.h"
#include "Tutorial/Tutorial.h"



std::shared_ptr<TutorialHeader> TutorialLoader::loadTutorialHeader(std::string& path)
{
  // PARSE GENERAL INFO (YAML)
  YAML::Node tutorial_yaml;
  try {
    tutorial_yaml = YAML::LoadFile(path);
  } catch (...) {
    LOG_ERROR("Tutorial file '" + path + "' not found or YAML header unparsable") //todo change logging
    return nullptr; // return nothing
  }
  //std::cout << tutorial_yaml << std::endl;

  // title
  std::string title = "undefined";
  if (tutorial_yaml["title"])
  {
    title = tutorial_yaml["title"].as<std::string>();
    LOG_DEBUG(title);
  }
  else
  {
    LOG_ERROR("Tutorial title not specified");
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
    LOG_ERROR("Tutorial description not specified");
  }
  // thumbnail
  std::shared_ptr<GUIImage> thumbnail = nullptr; // dummy image here? - NOPE rather later when rendering and encountering a nullptr (safer in case of loader errors)
  if (tutorial_yaml["thumbnail"]) {
    thumbnail = loadImage(getDirectoryPath(path) + tutorial_yaml["thumbnail"].as<std::string>());
  }
  else {
    LOG_ERROR("Thumbnail not specified");
  }
  // scene
  std::string sceneFile;
  if (tutorial_yaml["scene"]) {
    sceneFile = tutorial_yaml["scene"].as<std::string>();
    LOG_DEBUG(sceneFile);
  }
  else
  {
    LOG_ERROR("Scene file not specified");
  }

  // we create our tutorial header object on heap, we are using shared ptr, so that when there arent any references, we can eg properly free the loaded image and destroy it
  return std::make_shared<TutorialHeader>(std::move(path), std::move(title), std::move(description), std::move(thumbnail), std::move(sceneFile)); 
}

std::shared_ptr<Tutorial> TutorialLoader::loadTutorial(std::shared_ptr<TutorialHeader> header)
{
  // CHECK
  std::ifstream tutorialStream(header->m_filename);
  if (!tutorialStream.good()) {
    LOG_ERROR("Tutorial file '" + header->m_filename + "' was not found")
		return nullptr;  // return nothing
  }

  // SKIP YAML (MOVE STREAM POINTER PAST IT)
  int yamlSeparatorCount = 0;
  while (yamlSeparatorCount < 2) {
    // GET FIRST WORD OF THE LINE AND MOVE TO NEXT ONE
    std::string firstWord;
    tutorialStream >> firstWord;
    if (firstWord == "---"){
      yamlSeparatorCount++;  // if its a --- then note it
    }
    tutorialStream.ignore(std::numeric_limits<std::streamsize>::max(), tutorialStream.widen('\n'));  // skip to the next line
    // CHECK
    if (!tutorialStream.good()) {
      if (tutorialStream.eof()) {
        LOG_ERROR("Tutorial file '" + header->m_filename + "' missing 2 '---' YAML marks at the beginning of file or no further content behind them")
      } else {
        LOG_ERROR("Tutorial file '" + header->m_filename + "' I/O error")
      }
      return nullptr;
    }
  }

  // PREPARE PARSING MACHINE
  std::vector<TStep> steps;  // we will be filling this vector and then creating a tutorial with it
  steps.emplace_back();  // add the first step
  int currentStep = 0;
  std::unordered_map<std::string, std::shared_ptr<GUIImage>> images; // we will be loading found images in it
  int currentBlockIndent = -1;
  blockType_t currentBlock = NOT_BLOCK;
  std::string line;
  // temporaries for accumulating multiple line content:
  std::string textStore;
  std::vector<std::string> vectorOfTextsStore;
  int numberStore = -1;
  std::vector<int> vectorOfNumsStore;
 

  // auto canBeSingleLine = [](blockType_t keyword) -> bool {
  //   static const std::unordered_set<blockType_t> singleLines {
  //     EMPTY, STEP_START, TASK, HINT
  //   };
  //   return singleLines.find(keyword) != singleLines.end();
  // };
  // auto canBeMultiLine = [](blockType_t keyword) -> bool {
  //   static const std::unordered_set<blockType_t> multiLines {
  //     TASK, HINT, 
  //   };
  //   return singleLines.find(keyword) != singleLines.end();
  // };

  auto findAndLoadImages = [&](const std::string& string)
  {
    int tmpState = 0; // 0 - start, 1 - [, 2 - ](, 3 - )
    std::string imageFilename;
    for (size_t i = 1; i < string.size(); i++) {
      switch(string[i]) {
        case '[':
          if (string[i - 1] == '!' && tmpState == 0) {
            tmpState = 1;
          }
          break;
        case ']':
          if (tmpState == 1) {
            if (i < string.size() - 1 && string[i + 1] == '(') { // needs to be ]( in susccession, otherwise reset
              tmpState = 2;
            }
            else {
              tmpState = 0;
            }
          }
          break;
        case '(':
          break;
        case ')':
          if (tmpState == 2) {
            if (!images.contains(imageFilename)) {
              images[imageFilename] = loadImage(getDirectoryPath(header->m_filename) + imageFilename);
            }
            imageFilename = "";
            tmpState = 0;
          }
          break;
        default:
          if (tmpState == 2 && string[i] != ' ') {
            imageFilename += string[i];
          }
      }
    } 
  };

  auto isBlockType = [](const std::string& string)
  {
    // is keyword
    static const std::unordered_map<std::string, blockType_t> stringToBlockType = {
        { "task:", TASK },
        { "hint:", HINT },
        { "choice:", CHOICE },
        { "multichoice:", MULTICHOICE },
        { "input:", INPUT }
    };
    if (const auto it{ stringToBlockType.find(string) }; it != std::end(stringToBlockType)) {
      return it->second;
    }
    // is anything else
    return NOT_BLOCK;
  };

  auto isSingleLineType = [](const std::string& string)
  {
    // is keyword
    static const std::unordered_map<std::string, singleLineType_t> stringToSingleLineType = {
        { "task:", TASK_SINGLE },
        { "hint:", HINT_SINGLE },
        { "x:", CORRECT_ANSWER },
        { "o:", WRONG_ANSWER },
        { "answers:", ANSWER_LIST }
    };
    if (const auto it{ stringToSingleLineType.find(string) }; it != std::end(stringToSingleLineType)) {
      return it->second;
    }
    // is anything else
    return NOT_SINGLE_LINE;
  };

  // do i need this funtion? i think i dont even need a switch, i just add to the stores
  //auto addToCurrentBlock = [&](const std::string& string) -> void {
  //  switch (currentBlock) {
  //  case EXPLANATION:
  //    textStore
  //  }
  //};

  auto endCurrentBlock = [&]() -> void {
    switch (currentBlock) {
    case EXPLANATION:
      createExplanation(steps[currentStep], textStore);
      break;
    case TASK:
      createTask(steps[currentStep], textStore);
      break;
    case HINT:
      createHint(steps[currentStep], textStore);
      break;
    default:
      LOG_INFO("Creation of " + std::to_string(currentBlock) + " not implemented yet");
    }
    
    currentBlock = NOT_BLOCK;
  };

  auto beginBlock = [&](blockType_t blockType) -> void {
    // drop current block if any (safety check)
    if (currentBlock) {
      endCurrentBlock();
    }
    // set active block
    currentBlock = blockType;
    // reset temporary block variables
    textStore.clear();
    vectorOfTextsStore.clear();
    numberStore = -1;
    vectorOfNumsStore.clear();
  };

  auto handleSingleLine = [&](singleLineType_t type, const std::string& content) -> void {
    // check also for current state, and show error when calling singlelines which do not match
    // nektere pripady ponechavaji state, jine ho musi resetovat!

    //createTask(steps[currentStep], std::move(oneLineContent));
    // oneLineContent = ""; // unnecessary since i reinit it every while

    switch (type) {
    case TASK_SINGLE:
      endCurrentBlock();
      createTask(steps[currentStep], content);
      break;
    case HINT_SINGLE:
      endCurrentBlock();
      createHint(steps[currentStep], content);
      break;
    default:
      LOG_INFO("Creation of " + std::to_string(type) + " not implemented yet");
    }
  };

  // READ LINES
  while (std::getline(tutorialStream, line).good()) {
    // std::cout << "| " << line << std::endl; // todo logging using info

    // PROCESS LINE
    // make a stream again to be able to move through it
    std::istringstream lineStream(line); 
    // skip spaces
    skipSpaces(lineStream);  //NOTE: 
    // tell how much indentation did we skip
    int indent = static_cast<int>(lineStream.tellg()); // todo use
    // save first word
    std::string firstWord;
    lineStream >> firstWord;
    // special case when empty
    if (firstWord.empty()) {
      endCurrentBlock();
      continue;
    }
    // special case when start of a new step
    if (firstWord == "--") {
      endCurrentBlock();
      steps.emplace_back();
      currentStep++;
      continue;
    }
    // recognize type of line / command
    blockType_t blockType = isBlockType(firstWord);
    singleLineType_t singleLineType = isSingleLineType(firstWord);
    // skip possible spaces between keyword and content if any
    skipSpaces(lineStream);
    // get any possible remaining text
    std::string restOfLine;
    std::getline(lineStream, restOfLine);
    // handle single-lines
    if (singleLineType) {
      // actually used as single-line
      if (!restOfLine.empty()) {
        // take care of the single line
        handleSingleLine(singleLineType, restOfLine);
        // load any images in it if any
        findAndLoadImages(restOfLine);
        continue;
      }
      // actually used as block but the type does not allow blocks
      else if (!blockType) {
        std::cerr << "NOT A BLOCK COMMAND: " << line << std::endl;
        // todo handle error
      }
    }
    // handle block starts
    if (blockType) {
      // actually used as block start
      if (restOfLine.empty()) {
        beginBlock(blockType);
      }
      // actually used as single line but the type does not allow single lines (implicitly by if order)
      else {
        std::cerr << "NOT A SINGLELINE COMMAND: " << line << std::endl;
        // todo handle error
      }
    }
    // handle other text
    else {
      // no current block -> start explanation
      if (!currentBlock) {
        beginBlock(EXPLANATION);
      }
      // set this block's indent if this is the first line
      if (currentBlockIndent == -1) {
        currentBlockIndent = indent;
      }
      // skip that many spaces
      // todo temporary
      lineStream.seekg(0);
      skipSpaces(lineStream, indent);
      // add to active block
      std::getline(lineStream, restOfLine);
      textStore += restOfLine;
      textStore += '\n';
      // load any images found
      findAndLoadImages(restOfLine);
      //addToCurrentBlock(line);
    }
  }

  // CHECK if parsing ended because of error
  if (!tutorialStream.eof()) { 
    LOG_ERROR("Tutorial file '" + header->m_filename + "' I/O error");
  }


  
  // CREATE THE TUTORIAL
  std::shared_ptr<Tutorial> tutorial = std::make_shared<Tutorial>(std::move(header), std::move(steps), std::move(images)); // we create our tutorial object on heap

  return tutorial;
  // OLD NOTE: this will automatically move the unique pointer, no need to worry 
}

//TutorialLoader::blockType_t TutorialLoader::isKeyword(const std::string& string)
//{
//  // is empty
//  if (string.empty()) {
//    return EMPTY;
//  }
//  // is keyword
//  static const std::unordered_map<std::string, blockType_t> stringToKeyword = {
//    { "--", STEP_START },
//    { "task:", TASK },
//    { "hint:", HINT },
//    { "choice:", CHOICE },
//    { "multichoice:", MULTICHOICE },
//    { "input:", INPUT }
//  };
//  if (const auto it{ stringToKeyword.find(string) }; it != std::end(stringToKeyword)) {
//    return it->second;
//  }
//  // is anything else
//  return EXPLANATION;
//}

std::shared_ptr<GUIImage> TutorialLoader::loadImage(const std::string & path)
{
  // NOTE: we dont load dummy images here - let renderer handle nullptrs
  try {
    return std::make_shared<GUIImage>(path);
  }
  catch (std::runtime_error& e) {
    LOG_ERROR(e.what())
    return nullptr;
  }
}

std::string TutorialLoader::getDirectoryPath(std::string& path)
{
  std::filesystem::path p(path);
  return p.parent_path().string() + "/"; 
} 

void TutorialLoader::skipSpaces(std::istringstream& stream)
{
  while (stream.peek() == ' ' || stream.peek() == '\t') { // check if space
    stream.ignore(); // skip the character
  }
}

void TutorialLoader::skipSpaces(std::istringstream& stream, unsigned int maxCount)
{
  for (int i=0; i < maxCount; i++) { 
    if (stream.peek() == ' ' || stream.peek() == '\t') { // check if space
      stream.ignore(); // skip the character 
    }
    else {
      break;
    }
  }
}

std::shared_ptr<TutorialElement>& TutorialLoader::createExplanation(TStep& step, const std::string& string)
{
  return step.m_content.emplace_back(std::make_shared<Explanation>(string));
}

std::shared_ptr<TutorialElement>& TutorialLoader::createTask(TStep& step, const std::string& string)
{
  return step.m_content.emplace_back(std::make_shared<Task>(string));
}

std::shared_ptr<TutorialElement>& TutorialLoader::createHint(TStep& step, const std::string& string)
{
  return step.m_content.emplace_back(std::make_shared<Hint>(string));
}

std::shared_ptr<TutorialElement>& TutorialLoader::createChoice(TStep& step, const std::string& question, const std::vector<std::string>& choices, int correctChoice)
{
  return step.m_content.emplace_back(std::make_shared<ChoiceTask>(question, choices, correctChoice));
}

std::shared_ptr<TutorialElement>& TutorialLoader::createMultichoice(TStep& step, const std::string& question, std::vector<std::string>& choices, const std::vector<int>& correctChoices)
{
  return step.m_content.emplace_back(std::make_shared<MultiChoiceTask>(question, choices, correctChoices));
}

std::shared_ptr<TutorialElement>& TutorialLoader::createInput(TStep& step, const std::string& question,  const std::unordered_set<std::string>& correctAnswers)
{
  // todo
  return step.m_content.emplace_back(std::make_shared<InputTask>(question, correctAnswers));
}


  /*
  // nicely fill it at that heap place ^^
  // if this looks like magic, feel free to change it, but remember not to copy unique_ptrs!
  tutorial->m_steps.emplace_back(TStep("Krok 1")).m_content.push_back(std::make_unique<TWText>("yay"));
  // the first emplace returns a refernce to that newly created TStep here
  // the second can either be push or emplace, since both will automatically move instead of copy
  // make unique creates the <> specified object with passed arguments on heap (sadly a separate place) and returns the unique_ptr to it (or a referemnce to the uptr)

  // important is only to create the TStep with its vector first, and only then fill it with uptrs to widgets (not the other way around, since they would need to get copied into it)
  // another way of doing it:
  TStep& step = tutorial->m_steps.emplace_back(TStep("Krok 2"));
  step.m_content.push_back(std::make_unique<TWText>("fldsmdfr"));
  step.m_content.push_back(std::make_unique<TWText>("haha"));
  // WARNING! do not attempt to reassign the reference!

  TStep& step2 = tutorial->m_steps.emplace_back(TStep("Krok 3"));
  step2.m_content.push_back(std::make_unique<TWText>("juj"));
  step2.m_content.push_back(std::make_unique<TWText>("omg"));
  */

// void TutorialLoader::loadImages()
// {
//   // todo for cyklus
//
// }
//
// unsigned int TutorialLoader::loadImageOpenGL(std::string& filename)
// {
//   // todo opengl image loading, zvazit co pouzit za knihovnu
//
//   // return pgr::createTexture(filename);
//
//   return 0;
// }
//
//
//
// std::shared_ptr<Tutorial> TutorialLoader::getDummyTutorial(std::string message)
// {
//   
// }