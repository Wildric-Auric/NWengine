#NWEngine

##Dev Historical note
After my adventure with game engine, firstly GameMaker and secondly Unity, I passed to low level learning.
Before trying graphics API, I looked for a librairy to make a complete game. Thanks to a pretty solid knowledge in Python and after listenting to some videos of [DaFluffyPotato](https://www.youtube.com/c/DaFluffyPotato/videos) which praises it, I used Pygame. I tried to focus on the gameplay programming side but I dave in the technical side I figured out then that the use of Pygame is too unfortunate, especially when I tried to enhance the game with lighting. Using C++ with a framework such as sdl is a bad idea in my case since it has same problems encountred in Pygame (because of this being just a wrapper which inhererent it's problem and adds to them by the way). The main issue of SDL is it usage of shaders: It just does not allow you to use them or to pass any calculation to them directly! It is absurd in the sens that SDL is a framework built on top of OpenGL but it remove some important funcionalities from it.
The abstraction of SDL is not too complete; everything it does (graphically) can be built directly easily by you, using OpenGL, and even more. In contrast with SFML, SDL fails to be high level and fails also to be low level.



![Capture d’écran 2021-12-25 211448](https://user-images.githubusercontent.com/70033490/147395215-b24eca90-21f1-4b33-b3f3-a505bee34a0d.png)
![WATER](https://user-images.githubusercontent.com/70033490/147420571-3e49d7a0-9139-4da4-b962-bca525d3542d.png)


You can download dependencies folder [here](https://drive.google.com/drive/folders/12KNRHFGtMr7wbPIBco3QYLIMWeVaVTaj?usp=sharing) 
