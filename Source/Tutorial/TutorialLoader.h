#pragma once
#include "Tutorial.h"

#include <optional>

class TutorialLoader
{
public:
  // todo zmenit na nullptry
  static std::shared_ptr<TutorialHeader> loadTutorialHeader(std::string& path);
  static std::shared_ptr<Tutorial> loadTutorial(std::shared_ptr<TutorialHeader> header);
  
private:
  TutorialLoader() = default;

  // enum state_t
  // {
  //   STATE_NO_ELEMENT, STATE_EXPLANATION, STATE_TASK, STATE_HINT, STATE_CHOICE, STATE_MULTICHOICE, STATE_INPUT
  // };

  enum blockType_t
  {
    NOT_BLOCK = 0, EXPLANATION, TASK, HINT, CHOICE, MULTICHOICE, INPUT
  };

  enum singleLineType_t
  {
    NOT_SINGLE_LINE = 0, TASK_SINGLE, HINT_SINGLE, CORRECT_ANSWER, WRONG_ANSWER, ANSWER_LIST
  };

  /**
   * @brief simple wrapper that tries to load an image
   * @param path whole path to the image
   * @return ptr to image if successful, nullptr otherwise
  */
  static std::shared_ptr<GUIImage> loadImage(const std::string& path);

  //enum keyword_t
  //{
  //  NO_KEYWORD, EMPTY, STEP_START, TASK, HINT, CHOICE, MULTICHOICE, INPUT, CORRECT_ANSWER, WRONG_ANSWER, ANSWER_LIST
  //};

  //static state_t m_state;
  //static int m_currentStep;
  //static std::shared_ptr<TutorialElement>& m_currentElement;
  //static int m_currentIndent;

  /**
   * @brief returns according enum if such keyword was specified, EMPTY / 1 if the string is empty and NO_KEYWORD / 0 if none of the previous
  */
  //static blockType_t isKeyword(const std::string& string);
  static std::string getDirectoryPath(std::string& path);
  static void skipSpaces(std::istringstream& stream);
  static void skipSpaces(std::istringstream& stream, unsigned int maxCount);


  /**
   * @brief create a new explanation element in the referenced step and return a also reference to it
   * @param step a step at which the new element should be created
   * @param string starting value of the element
   * @return a reference to the shared_ptr of the newly created explanation element 
  */
  static std::shared_ptr<TutorialElement>& createExplanation(TStep& step, const std::string& string);
  static std::shared_ptr<TutorialElement>& createTask(TStep& step, const std::string& string);
  static std::shared_ptr<TutorialElement>& createHint(TStep& step, const std::string& string);
  static std::shared_ptr<TutorialElement>& createChoice(TStep& step, const std::string& question, const std::vector<std::string>& choices, int correctChoice);
  static std::shared_ptr<TutorialElement>& createMultichoice(TStep& step, const std::string& question, std::vector<std::string>& choices, const std::vector<int>& correctChoices);
  static std::shared_ptr<TutorialElement>& createInput(TStep& step, const std::string& question,  const std::unordered_set<std::string>& correctAnswers);
  // todo possibly make this a templated function
  // todo maybe accept strings as rvalue references and move it

  //static void startTask();
};
