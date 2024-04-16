![Banner](https://github.com/Wildric-Auric/NWengine/assets/70033490/1b4b391f-ff1f-4c4a-be3b-08570a9ee4f7)

###  What is NWengine?
**NWengine** is a game engine made with focus on modularity and customizability.
It empowers developpers to build fast **2D games** and graphics simulations, with the least depedencies possible.<br>
It is written in **C++** uses **OpenGL** API for rendering and targets only **Windows** 
for now.<br>

### Features
- **Modular**: Whenever possible, separation of logic is enforced.
- **User friendly**: The engine is written in simple **C++11** 
   and its source code can be easily integrated in any project.
- **Fast**: Unacessary overhead is avoided, we trust the programmer to know what he is doing.

### Architecture
The engine itself is called the **Core**, in contrst with the **Editor** which was present
at early stages of development, both were not seprated. Now only the **Core** persists. If
an editor is ever to be made, it will be a completely separate project which will use the **Core**
to achieve its goals.

#### Modules
Everything in the **Core** is either a **GameObject** (only one fundamental class), a **component**, 
a **system** or a **utility**. 
* A GameObject is a container of components, it is updated at each game loop iteation and has draw function method
    that redirect drawing to its components.
* A component is a class that inherits from `Component` and is attached to a GameObject.
* **System** is quite an ambiguous term, it should not as be confused with *system* in **ECS**, as logic related
components is conceptually embeded within them. **System** has additional logic, it is every class that 
manages the engine **GameObjects** and **Components** to perform a specific task or hold a specific data. 
Systems in general have **static** methods, they can inherit from `Asset` class to be able to load unload and manage
resources. Examples of systems are `Scene`, `Sound`,`Font`, `Batch`, etc.
* **Utility** is every piece of code that is a helper that does not interact internally with the **Core**, it can be every 
mathematical object and function, or a wrapper arround the Windows API, etc.
#### Static and private
Many systems use extensively static methods. This is a design choice that will be changed in the future as it limits 
the ability of the engine to be used as dynamic library.
To avoid unacessary overhead, almost every member is public, setters and getters are written for high level classes to 
avoid unsafe access to data. Variables prefixed with underscore `_` hints they are intended to be readonly or private.
As this rule was added lately, nothing garuntees it is the case for every class.<br>
#### Inheritance 
As you may have noticed, the **Core** uses sometimes inheritance (for assets and components for example), however
this is not a the general case. Inheritance is to be avoided as much as possible as it has unacessary overhead and makes
the code less readable and harder to maintain.
#### Memory management
Systems manage their memory and the memory used by the components they own. Systems can make use of methods 
`Init`, `Destroy` and `SetUp`, each has a specific role that should be detailed in the technical documentation.
Is is avoided to run logic in every type of constructor or destructors 
*(it wasn't the case in earlier versions though, and residue can still be observed in the code)*.
As a consequence, initialization of objects and their destruction is done within custom methods 
(like those evoked bofore), and smart pointers are not used.

### Issues and Contributions
If you have any issue, bug, question, or suggestion related to **NWengine**, 
please [report](https://bin.disroot.org/?ccf76f0fffc9caca#C7AZCCqwWHXh8D2ghgYrLUQwHSsQLCnnYFYVjko7LDty) 
it in the corresponding section of the repository.<br>
There are still many features the engine lacks, many bugs to be founded, and many improvements to be made, all contributions
are welcome. A pull request should cover the following points:
- **Description**: A clear summary of the changes you made.
- **Motivation**: Why you made these changes.
- **Breakdown**: A list of the main changes you made.
- **Results and tests**: How you tested the changes you made, the results and the potential issuses 
that can be due to these changes.
Significant changes require significant description. Unit tests and integration tests are welcome.

### Getting Started
Download your assets from the release page. 
You should then have a template project with `premake5.lua`, the binary `NWEngineCore64.lib`/`NWEngineCore32.lib` 
and an `include` folder with the headers architectured in a tree structure. A single `NWEngineCore.h` is provided too,
is is equivalent of having all the headers separetely, it is better for developpers who don't
like big amounts of files and includes in their project.
Once you have that, and you compile your the template compiles, you are ready to start programming yout first
game powered by **NWengine** !

Teleport to the [tutorial](https://nwengine-doc.vercel.app/index.html) !
