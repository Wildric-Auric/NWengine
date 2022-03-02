#pragma once 
#include "Texture.h"
#include "RessourcesLoader.h"
#include <GL/glew.h>
#include <glfw3.h>
class FrameBuffer {
private:
		unsigned int framebuffer;
public: 
	 FrameBuffer() {
		 glGenFramebuffers(1, &framebuffer);
		 glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	    textures["grabTex"] = Texture(ORIGINAL_WIDTH,ORIGINAL_HEIGHT, NULL, 1,0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures["grabTex"].texture, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	 }
	 void Bind(){
		 glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	 }
	 void Unbind() {
		 glBindFramebuffer(GL_FRAMEBUFFER, 0);
	 }
};