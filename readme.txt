Last updated: 2014/12/29 by Naozumi Hiranuma 

This is a github copy of the project. See https://bitbucket.org/hiranumn/neatxcode_2 for the original repo.
See gecco_predprey_paper_2014.pdf for more datailed description of this project. 

This version of the project is trying to recreate the experiment done by Luke et al in "Evolving teamwork and coordination with genetic programming", except using neural networks (NEAT) instead of GP.

###How to complie this with visiual studio 12###
~INSTALLING SFML~
1. Get the sfml-2.1 source code from http://www.sfml-dev.org/download/sfml/2.1/
2. Configure the sfml code with cmake by following the instruction in the tutorial page.
3. Complie it using visual studio 12 (ALLL_build).

~COMPILING NEAT~
1. Generate a project file by typing the following command: cmake -G "visiual studio 12" 
2. Set the include path to the include folder in the sfml-source code.
 (project->properties->configuration properties->C/C++->General)
3. Set the library path to the lib files that you just complied.
 (project->properties->configuration properties->Linker->General)
4. Add sfml-graphics-d.lib, sfml-system-d.lib, and sfml-window-d.lib to the additional dependency section.
 (project->properties->configuration properties->Linker->Input)
5. BUILD IT.
6. When you run, it probably say that dll files are missing so just put them under the same folder as neat.exe 

###TO DO LIST###
1. Tournament selection is not implemented in the way we want. It needs to be fixed soon.
2. We need to discuss what we want for the outputs of a neural network. 