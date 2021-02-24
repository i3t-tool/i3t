/**
 * \file	GUI\frameBuffer.h
 *
 * Declares the frame buffer class
 */
#pragma once

#include <iostream>
#include <vector>

#include "pgr.h"
#include <Logger/LoggerInternal.h>
#include <glm/glm.hpp>

/**
 * \brief Stack of OpenGL frame buffer objects. The FBO on TOS is bounded
 */
class FrameBuffer
{
private:
  static std::vector<FrameBuffer*> frameBuffers; ///< stack of bounded frame buffers, TOS is at the end

  static GLuint defaultBuffer;

  bool generated; ///< OpenGL framebuffer object is generated (with its texture and renderbuffer)

  GLuint texID;         ///< OpenGL output texture name
  GLuint bufferID;      ///< OpenGL renderbuffer name
  GLuint depthBufferID; ///< OpenGL depth buffer name

  bool depth; ///< is depth buffer present?

  int width;  ///< Framebuffer width
  int height; ///< Framebuffer height

  /**
   * \brief Delete the framebuffer and its texture and renderbuffer
   */
  void dispose()
  {
    if (generated)
    {
      glDeleteTextures(1, &texID);
      if (depth)
        glDeleteRenderbuffers(1, &depthBufferID);
      glDeleteFramebuffers(1, &bufferID);

      generated = false;
    }
  }

public:
  /**
   * Set buffer for rendering.
   * @param buffer
   */
  static void setDefaultBuffer(unsigned int buffer) { defaultBuffer = buffer; }

  /**
   * \brief Bind the \a buffer and set its viewport. Push it to the stack of bounded \a frameBuffers.
   * \param buffer Framebuffer to bind.
   */
  static void bindBuffer(FrameBuffer* buffer)
  {
    frameBuffers.push_back(buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, buffer->getBufferID());
    glViewport(0, 0, buffer->width, buffer->height);
  }

  /**
   * \brief Unbinds the buffer on TOS (currently bounded buffer
   */
  // static void unbindBuffer(FrameBuffer * buffer) {
  static void unbindBuffer()
  {
    frameBuffers.pop_back();

    // If no buffer on the stack, bind the screen.
    if (frameBuffers.empty())
    {
      // glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, defaultBuffer);
      /// \todo Get screen size, only if FrameBuffer class will be used in a future.
      // const glm::vec2 ss = GUIProjection::getScreenSize();
      // glViewport(0, 0, int(ss.x), int(ss.y));
      return;
    }

    // bind the TOS buffer and set its viewport
    FrameBuffer* fb = frameBuffers[frameBuffers.size() - 1];
    glBindFramebuffer(GL_FRAMEBUFFER, fb->getBufferID());
    glViewport(0, 0, fb->width, fb->height);
  }

  /**
   * \brief Constructor. Generates the OpenGL FBO of given size and with a depth renderbuffer if \a _depth is true.
   *        Stores the generated texID, bufferID and depthBufferID.
   * \param _width Width.
   * \param _height Height.
   * \param _depth Flag. True if the depth buffer should be present and generated
   */
  FrameBuffer(int _width, int _height, bool _depth) : generated(false), width(_width), height(_height), depth(_depth)
  {
    generate();
  }

  ~FrameBuffer() { dispose(); }

  void generate()
  {
    dispose();

    // generate texture
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    // glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach depth buffer to FBO
    if (depth)
    {
      glGenRenderbuffers(1, &depthBufferID);
      glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
      // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
      glBindRenderbuffer(GL_RENDERBUFFER, 0); /// PF
    }

    // generate buffer
    glGenFramebuffers(1, &bufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);
    // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texID, 0);

    if (depth)
    {
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, defaultBuffer);

    // generateMipMaps();

    generated = true;

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
      // cout << "frame buffer generated, id = " << bufferID << std::endl;
    }
    else
    {
      LOG_FATAL("frame buffer generating ERROR ...");
    }

    CHECK_GL_ERROR();
  }

  /**
   * \brief Clear the framebuffer (color and depth, if present)
   */
  void clearBuffer() const
  {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    if (depth)
    {
      glClearDepth(1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else
    {
      glClear(GL_COLOR_BUFFER_BIT);
    }
  }

  /**
   * \brief Clear the framebuffer (color and depth, if present) by given \ a color and \a alpha.
   *		  It is used for box background (sequence, operator, ...).
   * \param color Clear color
   * \param alpha Clear alpha
   */
  void clearBuffer(const glm::vec3 color, const float alpha) const
  {
    glClearColor(color.x, color.y, color.z, alpha);

    if (depth)
    {
      glClearDepth(1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else
    {
      glClear(GL_COLOR_BUFFER_BIT);
    }
  }

  /**
   * \brief Bind this frame buffer
   */
  void bind()
  {
    bindBuffer(this); // glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
  }

  /**
   * \brief Unbind the currently bounded frame buffer (on TOS of frameBuffers)
   */
  void unbind()
  {
    // unbindBuffer(this);  // parameter was unused
    unbindBuffer();
  }

  /**
   * \brief Resize the buffer to given size (dispose it and regenerate)
   * \param _width New width - clamped to 10
   * \param _height New height - clamped to 10
   */
  void resize(const int _width, const int _height)
  {
    width = glm::max(_width, 10);
    height = glm::max(_height, 10);

    dispose();
    generate();
  }

  void generateMipMaps() const
  {
    glBindTexture(GL_TEXTURE_2D, texID);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  int getBufferID() const { return bufferID; }

  int getTextureID() const { return texID; }

  int getDepthBufferID() const { return depthBufferID; }

  int getWidth() const { return width; }

  int getHeight() const { return height; }

  /*
  void FBO_2_PPM_file(char * filename) {

    FILE    *output_image;
    int     output_width, output_height;

    output_width = width;
    output_height = height;

    /// READ THE PIXELS VALUES from FBO AND SAVE TO A .PPM FILE
    int             i, j, k;
    unsigned char   *pixels = (unsigned char*)malloc(output_width*output_height*3);

    /// READ THE CONTENT FROM THE FBO
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, output_width, output_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    output_image = fopen(strcat(filename, ".ppm"), "wt");
    fprintf(output_image,"P3\n");
    fprintf(output_image,"# Created by Ricao\n");
    fprintf(output_image,"%d %d\n",output_width,output_height);
    fprintf(output_image,"255\n");

    k = 0;
    for(i=0; i<output_width; i++)
    {
      for(j=0; j<output_height; j++)
      {
        fprintf(output_image,"%u %u %u ",(unsigned int)pixels[k],(unsigned int)pixels[k+1],
                          (unsigned int)pixels[k+2]);
        k = k+3;
      }
      fprintf(output_image,"\n");
    }
    free(pixels);
  }
  */
};
