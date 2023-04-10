#ifndef RENDERER_H
#define RENDERER_H

class Renderer{
	Renderer();
	~Renderer();

	void init();

	void settings();

	void update();

	void load_shaders();

	void load_texture_atlas();
};


#endif // !RENDERER_H