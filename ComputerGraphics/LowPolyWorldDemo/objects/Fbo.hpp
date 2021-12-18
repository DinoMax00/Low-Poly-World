#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * 封装一个帧缓冲对象.
 */
class Fbo {
private:
	unsigned int width, height;
	unsigned int frame_buffer = 0;
	unsigned int color_buffer = 0;
	unsigned int depth_buffer = 0;

public:
	/**
	 * 生成一个帧缓冲，并绑定颜色和深度缓冲.
	 * 
	 * \param width
	 * \param height
	 */
	Fbo(unsigned int _width, unsigned int _height, bool use_texture_for_depth): width(_width), height(_height) {

		glGenFramebuffers(1, &frame_buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
		// 生成纹理
		glGenTextures(1, &color_buffer);
		glBindTexture(GL_TEXTURE_2D, color_buffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer, 0);
		// 深度缓冲
		if (use_texture_for_depth) {
			glGenTextures(1, &depth_buffer);
			glBindTexture(GL_TEXTURE_2D, depth_buffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_buffer, 0);
		}
		else {
			glGenRenderbuffers(1, &depth_buffer);
			glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, 0, GL_DEPTH_COMPONENT24, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
	
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	/**
	 * 绑定帧缓冲.
	 * 
	 */
	void bind() {
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer);
		glViewport(0, 0, width, height);
	}

	/**
	 * 恢复默认帧缓冲.
	 * 
	 */
	void unbind() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);
		glViewport(0, 0, WINDOW_W, WINDOW_H);
	}

	unsigned int getColorBuffer() {
		return color_buffer;
	}

	unsigned int getDepthBuffer() {
		return depth_buffer;
	}
};
