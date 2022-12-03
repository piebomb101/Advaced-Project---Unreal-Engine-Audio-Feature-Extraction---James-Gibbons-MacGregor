# Advaced-Project---Unreal-Engine-Audio-Feature-Extraction---James-Gibbons-MacGregor

INTRO: This repository contains the C++ files for using the real-time audio feature extraction system I have developed in Unreal engine. Below is a tutorial and a development log.

HOW TO USE:  Create a C++ unreal project. Then right click in the content browser and create a new C++ Class. In the window that appears search for and select “BlueprintFuntionLibrary”. YOU MUST NAME THIS EXACTLY THE SAME AS THE FILENAME OF THE CODE!!! Open this in your code editor and simply replace the .h and .cpp code with the files above. You can then create a blueprint and add the new nodes. Set up a blueprint as shown below and the return value pin of each node will output the data. This system shows all nodes working at once. 

![AFE TUTORIAL PIC](https://user-images.githubusercontent.com/90520843/205463508-13bf9df2-7154-46af-9bab-8cd8e2b22a05.png)


NOTES: 

THE “START ANALYZING OUTPUT” NODE MUST HAVE ‘FTT SIZE’ SET TO ‘MAX’ AND ‘SPECTRUM TYPE’ TO ‘MAGNITUDE SPECTRUM’. 

SET THE SAMPLE RATE VARIABLE TO MATCH THE RATE OF YOUR PROJECT FOR THE PEAK FREQUENCY NODE.

BY DEFAULT THIS WILL ANALYZE THE MASTER-SUBMIX, FOR LIVE AUDIO INPUT SIMPLE ADD AN “ADD AUDIO CAPTURE COMPONENT” IN THIS OR ANY OTHER BLUEPRINT IN YOUR SCENE

THIS HAS ONLY BEEN TESTED IN UNREAL ENGINE 5.0.1
