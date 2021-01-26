//#include "Tutorial.h"
//#include "Core/Application.h"
////#include "gl/GL.h"
//#include "Logger/Logger.h"
//#include "imgui.h"
//#include "stb_image.h"
//#include "yaml-cpp/yaml.h"
//#include <utility>
//
//TutorialLoader::TutorialLoader(std::string filename)
//{
//  //	std::replace( filename.begin(), filename.end(), '\\', '/'); // replace all '\' to '/'
//  m_filename = filename;
//  loadFile();
//  loadImages();
//}
//
//void TutorialLoader::loadFile() // todo think about making an interface of tutorial and let this be tut file type dependend?
//{

//  // LOAD STEPS
//  if (!tutorial["steps"])
//  {
//    // todo LOG_FATAL("Tutorial file missing 'steps' node");
//    throw "Tutorial file missing 'steps' node";
//  }
//  YAML::Node n_steps = tutorial["steps"];
//  for (YAML::const_iterator n_steps_it = n_steps.begin(); n_steps_it != n_steps.end(); ++n_steps_it)
//  {
//    // for each STEP
//    const YAML::Node& n_step = *n_steps_it;
//    TStep step;
//    for (YAML::const_iterator n_widget_it = n_step.begin(); n_widget_it != n_step.end(); ++n_widget_it)
//    {
//      // for each WIDGET
//      const YAML::Node& n_widget = *n_widget_it;
//      TWidget widget;
//      // now n_widget is a map, we need the first (and only) entry in it
//      for (const auto& n_pair : n_widget)
//      {
//        widget.m_type = n_pair.first.as<std::string>();
//        widget.m_string = n_pair.second.as<std::string>();
//        break;
//      }
//      // add widget to step's contents
//      step.m_content.emplace_back(widget);
//      // if it is a special case, also add it to proper structures
//      if (widget.m_type == "img")
//      {
//        m_images.insert(std::pair<std::string, unsigned int>(widget.m_string, 0));
//      }
//    }
//    // add step to steps list
//    m_steps.emplace_back(step);
//  }
//}
//
//void TutorialLoader::loadImages()
//{
//  std::cout << "Tutorial::loadImages()" << std::endl;
//  for (auto& image : m_images)
//  {
//    auto& image_filename = image.first;
//    auto& image_id = image.second; // by reference
//    std::string whole_filename = Config::getAbsolutePath(("/Data/tutorials/" + image_filename).c_str());
//    // glEnable(GL_TEXTURE_2D);
//    GLuint tex_id = 0;
//    glGenTextures(1, &tex_id);
//    glBindTexture(GL_TEXTURE_2D, tex_id);
//    // set repating of pattern
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    // set linear filtering
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    // load file
//    int width, height, nr_channels;
//    unsigned char* data = stbi_load(whole_filename.c_str(), &width, &height, &nr_channels,
//                                    0); // todo if desired channels not 4 it doesnt work at all
//    if (data == nullptr)
//    {
//      throw std::string("Cannot read file " + whole_filename + stbi_failure_reason());
//    }
//    if (!(nr_channels == 3 || nr_channels == 4))
//    {
//      throw std::string("Texture has unsupported number of elements per pixel:" + nr_channels);
//    }
//
//    const GLenum format = nr_channels == 3 ? GL_RGB : GL_RGBA;
//    const GLenum internal_format = nr_channels == 3 ? GL_RGB8 : GL_RGBA8;
//    // upload data to OpenGL
//    // glPixelStorei(GL_UNPACK_ALIGNMENT, nr_channels);
//    // glPixelStorei(GL_UNPACK_ROW_LENGTH, nr_channels * width);
//    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height / 2, 0, format, GL_UNSIGNED_BYTE, data);
//    // free data
//    stbi_image_free(data);
//    // unbind the texture (just in case someone will mess up with texture calls later)
//    glBindTexture(GL_TEXTURE_2D, 0);
//    // save image id to the filename-imageID map
//    image_id = tex_id;
//  }
//}
//
//unsigned int TutorialLoader::getImageID(std::string filename)
//{
//  std::cout << "Tutorial::getImageID()" << std::endl;
//  const auto images_it = m_images.find(filename);
//  if (images_it == m_images.end())
//  {
//    throw "Image filename not found in filaname-imageID map, unexpected behaviour";
//  }
//  else
//  {
//    return images_it->second;
//  }
//}
//
//// render the tutorial content using ImGui
//void TutorialLoader::render(int step)
//{
//
//}
//
//size_t TutorialLoader::getNumSteps()
//{
//  return m_steps.size();
//}
