# NWEngine

## Dev Historical note
After my adventure with game engine, firstly GameMaker and secondly Unity, I passed to low level learning.
Before trying graphics API, I looked for a librairy to make a complete game. Thanks to a pretty solid knowledge in Python and after listenting to some videos of [DaFluffyPotato](https://www.youtube.com/c/DaFluffyPotato/videos) which praises it, I used Pygame. I tried to focus on the gameplay programming side but I dave in the technical side I figured out then that the use of Pygame is too unfortunate, especially when I tried to enhance the game with lighting. Using C++ with a framework such as sdl is a bad idea in my case since it has same problems encountred in Pygame (because of this being just a wrapper which inhererent it's problem and adds to them by the way). The main issue of SDL is it usage of shaders: It just does not allow you to use them or to pass any calculation to them directly! It is absurd in the sens that SDL is a framework built on top of OpenGL but it remove some important funcionalities from it.
The abstraction of SDL is not too complete; everything it does (graphically) can be built directly easily by you, using OpenGL, and even more. In contrast with SFML, SDL fails to be high level and fails also to be low level.

## OpenGL rendering 
The renderer is meant to be 2D. It's based on rendering quads through vertex buffer and element buffer since the draw method uses ```GL_ELEMENT_ARRAY_BUFFER```; they are associated to each GameObject (see abstraction). Every texture is mapped in a quad and shown on the screen.
## Engine abstraction
### GameObject vs GameObjectClone
Each object you would need to draw in the game is loaded initially and stored in an object of the class ```GameObject```.This information consist in a texture which is generated from ```Image``` class object and stored in ```Texture``` class object. GameObject *static* by convention which mean it characteristics are defined once and that it' a very bad practice to change after creation. Note also and even if GameObject has a Draw method, it should not be called directly, which leads us to ```GameObjectClone```.

GameObjectClone is an object which can be seen as GameObject but which characteristics can be changed, meanwhile GameObjects attributes are constants and are basis to defone GameObjectClone objects. Let's say for example you have mapped your texture in a GameObject **a** and that you gave it the size of *100px* by *100px*.
Creating two GameObjectClone **b** and **c** will allow you to do such manipulation.
```
b = GameObjectClone(&a);
c = GameObjectClone(&a);
b.scale.x = 2.0f;
c.scale.x = .5f;
```
Hence, the width of **b** is two times **a**'s one, and 1/2 time for **c**.
GameObjectClone does not contain texture nor VBO nor EBO; it's Draw function is a wrapper of GameObject draw function.
This system can be seen as a prefab system; the name can be changed later to be more representative.
### Drawable, Updatable...
```Drawable``` and ```Updatable``` are two classes containing corresponding methods. Objects having those methods; should inherit from these classes and override base method.
Drawable for instance has it class virtual; and is defined as following: 
```
class Drawable {
public:
	virtual void Draw(uint8_t slot = 0) {};
};
```
GameObjectClone inherit from it: 
```
class GameObjectClone : public Drawable {...}
```
This system is useful in that it allows you to put pointers of Drawable objects for example in a map with draw order and an iteration over its elements draws objects. Note that this won't be simple otherwise since those objects aren't of same type. I'm taking advantage here of C++ [polymorphism](http://labmaster.mi.infn.it/Laboratorio2/serale/www.cplusplus.com/doc/tutorial/polymorphism/)
### Maths
Initially, **glm** librairy was used. It's a solid mathematical librairy and a complete one. However, to have full control on the engine and its processing speed, in a long term perspective, building an internal maths librairy is necessary.
Everything there is set as templates. There are some fucntions like min, max among others; and classes like Matrix and Vector2, Vector3, Vector4. 
Matrix is defined for the general case (n row m column) meanwhile size of vector is 2,3 or 4. Such choice was done for processing speed, since a general size vector requires more time for operations to be done. Look at this example:
``` 
#include <iostream>
#include <chrono>

void func(int i) {
  i+=1;
  i+=1;
  i+=1;
  i+=1;
  i+=1;
  i+=1;
  i+=1;
  i+=1;
  i+=1;
  i+=1;
}
void func1(int i) {
  for (int j = 0; j < 10; j++) {
    i += 1;
  }
}
int main() {
    long ITER = 10e7;
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i<ITER; i++) {
      func(0);
      };
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> t = end-start;
    std::cout<< t.count()<<std::endl;

}

```
Compiled on Replit; the program ouputs: *2.68272*
Changing func(0) by func1(0) make its output: *4.42551*
It a big difference especially when you are making an engine where speed is essential.
### TileMaps

### Gui 

### Physics engine

### Text rendering

### Animation

### Particle system
### Audio 

###
## Dependencies

You can download dependencies folder [here](https://drive.google.com/drive/folders/12KNRHFGtMr7wbPIBco3QYLIMWeVaVTaj?usp=sharing) 


## Showcase

![Capture d’écran 2021-12-25 211448](https://user-images.githubusercontent.com/70033490/147395215-b24eca90-21f1-4b33-b3f3-a505bee34a0d.png)
![WATER](https://user-images.githubusercontent.com/70033490/147420571-3e49d7a0-9139-4da4-b962-bca525d3542d.png)


