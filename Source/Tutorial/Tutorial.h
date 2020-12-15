#pragma once
#include <string>
#include <unordered_map>
#include <vector>

struct TWidget
{
  std::string m_type;
  std::string m_string;
};

struct TStep
{
  std::vector<TWidget> m_content;
};

class Tutorial
{
public:
  Tutorial(std::string filename);
  void render(int step);
  size_t getNumSteps();

private:
  std::string m_filename;
  std::string m_name;
  std::string m_description;
  std::string m_thumb_filename;
  std::vector<TStep> m_steps;
  std::unordered_map<std::string, unsigned int> m_images; // filename to GLuint
  std::string m_conclusion;
  void loadFile();
  void loadImages();
  unsigned int getImageID(std::string filename);
};
