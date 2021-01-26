﻿#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct TWidget
{
  virtual ~TWidget() = default;
  //std::string m_id;
};

struct TWText : TWidget  // todo can also contain bullet list???
{
  explicit TWText(std::string text) : m_text(std::move(text)) {}
  std::string m_text;
};

struct TWImage : TWidget
{
  explicit TWImage(std::string filename) : m_filename(std::move(filename)) {}

  std::string m_filename;
};

struct TWAnimatedImage : TWidget
{
  std::string m_filename;
  // todo animation stuf
};

struct TWButton : TWidget
{
  std::string m_text;
  // todo command
};

struct TStep
{
  // HACK using unique pointers because I need pointers in general in order to avoid slicing of subclass objects (TWText,...) into the base class (TWidget) when storing them in this vector.
  // also, it is preffered to have steps also as unique pointers, 
  std::vector<std::unique_ptr<TWidget>> m_content;
};

/**
 * \brief Structure for holding information need for showing a specific tutorial. Is filled by \fn TutorialLoader::loadFile() function.
 */
struct Tutorial
{
  ~Tutorial();
  // general
  std::string m_filename;
  std::string m_title;
  std::string m_description;
  std::string m_thumb_filename;
  std::string m_scene_filename;

  // step content
  std::vector<TStep> m_steps;
  // support structures
  std::unordered_map<std::string, unsigned int> m_images; // filename to GLuint
  // properties
  int getStepCount() const { return m_steps.size(); }
};

inline Tutorial::~Tutorial()
{
  // todo free loaded images from video memory
}
