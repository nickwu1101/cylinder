1. Put the source directory and the path is "$path/cylinder/".

2. Make a directory to store your build of simulation. Suppose its path is "$path/cy-build".

3. Type "cmake -DGeant4_PREFIX_PATH=/$your-geant4-install %path/cylinder/" to make the build file.

4. Type "make" to compile the code and make an executable file.

5. Type "./cylinder" only will open a visualized interface for showing the geomatry. You can type commands or execute macro files for simulation.

6. Or type "./cylinder $macrofile" to conduct macro file command. There is no visualized interface and conduct in the background. The execution will make a output file when simulation over.

7. The manual of Geant4: https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/index.html