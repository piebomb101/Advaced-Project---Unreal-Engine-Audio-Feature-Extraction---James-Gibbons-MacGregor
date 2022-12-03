# Advaced-Project---Unreal-Engine-Audio-Feature-Extraction---James-Gibbons-MacGregor

# INTRO: 

This repository contains the C++ files for using the real-time audio feature extraction system I have developed in Unreal engine. Below is a tutorial and a development log.

HOW TO USE:  Create a C++ unreal project. Then right click in the content browser and create a new C++ Class. In the window that appears search for and select “BlueprintFuntionLibrary”. YOU MUST THEN NAME THIS EXACTLY THE SAME AS THE FILENAME OF THE .h AND .cpp FOR THE NODE YOU ARE CREATING !!! Open this in your code editor and simply replace the .h and .cpp code with the files above. You can then create a blueprint and add the new nodes. Set up a blueprint as shown below and the return value pin of each node will output the data. This system shows all nodes working at once. 

![AFE TUTORIAL PIC](https://user-images.githubusercontent.com/90520843/205463508-13bf9df2-7154-46af-9bab-8cd8e2b22a05.png)


# NOTES: 

THE “START ANALYZING OUTPUT” NODE MUST HAVE ‘FTT SIZE’ SET TO ‘MAX’ AND ‘SPECTRUM TYPE’ TO ‘MAGNITUDE SPECTRUM’. 

SET THE SAMPLE RATE VARIABLE TO MATCH THE RATE OF YOUR PROJECT FOR THE PEAK FREQUENCY NODE.

BY DEFAULT THIS WILL ANALYZE THE MASTER-SUBMIX, FOR LIVE AUDIO INPUT SIMPLE ADD AN “ADD AUDIO CAPTURE COMPONENT” IN THIS OR ANY OTHER BLUEPRINT IN YOUR SCENE

THIS HAS ONLY BEEN TESTED IN UNREAL ENGINE 5.0.1

# DEVELOPMENT DIARY:

*Phase 1:

I began researching audio feature extraction methods, primarily focused on gaining an understanding of how the FFT and MFCC Algorithms work. I then reviewed the code used for feature extraction within the OpenFrameworks library add-on Maximillian. I considered how this could be implemented to Unreal Engine and experimented with creating my own plugins. 

*Phase 2:

I decided that the most effective way to handle live audio in UE5 would be using the Metasounds plugin to create a single custom node that would receive an audio input and then output all of the various feature data. I followed this dev tutorial from unreal (https://dev.epicgames.com/community/learning/tutorials/ry7p/unreal-engine-creating-metasound-nodes-in-c-quickstart) but for reasons that are unclear visual studio would not detect the required generated .h and .cpp files. This made this method impossible. 

I was able to get this working using a template found on github (https://github.com/alexirae/unreal-audio-dsp-template-UE5) . This provided me with a blank canvas to try and implement the FFT and feature extraction code from Maximillian, but using metasound’s audio stream.  

*Phase 3:

It took a great deal of time to get used to the way Metasound nodes are programmed. I found they used complex naming conventions and a read/write reference system I was unfamiliar with. After a great deal of effort, time and debugging I was still unable to get the FFT code to run within the node. Despite the audio input being a float array the FFT code would always just output 0.0 for every array element. At this point I decided to instead use the built in FFT analysis system from UE5 outside of the node to analyze the audio and output an FFT array to my Metasound node for feature extraction. 

While testing this I hit an issue that would cause a total rethink of my approach to this project. I discovered via a forum post from Epic Games staff that at present it is not possible to use outputs of any kind other than audio from Metasounds within regular blueprints, only other metasounds (https://forums.unrealengine.com/t/metasound-outputs-accessible-via-blueprint/542873/5) . As I wanted to use my metasound node to extract feature data for use with InteractML within blueprints this meant I would have to completely alter my implementation method. 

*Phase 4:

My rethink led me to the following method based on the various issues I had already encountered. I decided the best option was to, for each feature extraction method, create a unique blueprint function library. I based this on this tutorial https://trashbyte.io/blog/2019/10/20/custom-blueprint-nodes-part1.html  which allowed me to set up classes for each feature extraction method with the appropriate inputs and output. 

I then encountered a serious flaw with UE5s FFT system. It requires the frequencies for each band to be entered manually, despite them being set within the FFT analysis node. To fix this I had to create a custom function node that would allow me to create an array of frequencies of the correct values for the respective FFT size.

I tested my FFT system by mapping the values to a series of cubes to visualize the data. I realised that the top 3rd of the array elements were providing extremely small values in comparison to the lower frequencies. This is expected with most audio signals but it would still present this when given white noise. After a long time debugging I had to conclude that this was due to the nature of the UE5 FFT and not an issue with my code. However I still had enough varied values for each frame that the system would be usable so I pushed on regardless.

*Phase 5:

I began implementing the various feature extraction functions. As these were being taken directly from the Openframeworks code they required some alteration to work with the blueprint function libraries. I had to familiarize myself with UE5’s TArray system and struggled quite a bit with the pointer and reference system used for returning the arrays. 

Creating functions such as spectral flux and peak frequency was simple as the processes are quite basic, although required a bit of adjustment from the openframeworks version.

The real challenge was getting the MFCC algorithm to work. The MFFC process in the example was split between 2 separate .h and .cpp files so combining them correctly into one .h and .cpp required a great deal of experimentation. I also had a number of issues with variables not working correctly. I discovered this was due to unsigned ints being required for Unreal. 

*Phase 6:

I was eventually able to get the MFCC code to compile but when I hit play within UE5 I would immediately hit an exception error access violation. By debugging with breakpoints I was able to locate the issue, it was the deconstructor of the argument arrays used by the main MFCC analysis function. Removing this code did in fact allow it to compile but I then had a memory leak. These arrays had been created with ‘malloc’ but this requires using a free() function, not a delete[] as was being used. This code worked in the openframeworks example and I don't understand how. 

Having researched the malloc and new operators I decided to try and implement the new and delete method as this would provide better performance. This worked well.

*Phase 7:

Having tried to set up internal sound routing from Ableton to unreal I was unable to find a suitable solution. Therefore I have to use a separate laptop for audio generation. Due to a small latency that is inherent to UE5’s ‘audio capture node’, this new setup makes more sense as it is small enough to not be noticeable with visuals but unworkable from a performance tempo perspective. 

With the ability to get sound into unreal properly I started trying to use the extracted features with InteractML. After setting up a basic classification model I was able to record a data set and train a model. However when running the model with the same data inputs there is a ‘Size Error’ on the input. I contacted the Dev team and have been told there are some issues with IML’s float handling are they are looking into it.

In the meantime I experimented with realtime reactive audio visuals using the feature extractors and niagara systems, going beyond the standard emitter systems and scripting my own parameters.

*Phase 8:

After speaking to the InteractML team I discovered the issue was with the example data I was feeding in. I had been recording series of different length/sizes so it was unable to function properly. The solution was to record single example frames at a time. This required altering my controls for the record functions to fire on each frame but after some experiments a combination of sequence, gates and flip/flops on the event tick got this working. I could then successfully train then run classification models in real time on incoming audio feature data. The core aim of the project was therefore achieved. It is now a matter of fixing bugs and working out how best to use the models.

*Phase 9:

I began testing the software with a number of inputs. First I attempted to differentiate between two genres of music. I trained a model on 3 different labels of MFCC input. These were silence, a jazz trio and heavy drum and bass. The software was able to achieve reasonably accurate and stable results with a relatively small number of samples. However it became clear that some post processing of the output label data was required as other incorrect values would often briefly appear for each piece of audio. Ideally this would have been the mean of an array of previous values but I was unable to create this function. 

This is due to an issue that I’ve come across a number of times with blueprint function libraries. As they require an execute pin they can either be called ‘on play’ or ‘on tick’. This means you effectively lose the option to have a ‘setup’ and ‘update’ within your function. It can be called once or every frame, not both. Therefore I am unable to initialize an array once, instead it is initialized each frame, losing the previous values. I attempted to create an array externally in another function and pass it into the post process function, however unreal only allows the reading of external arrays and you cannot alter elements. In place of this I used the ‘moving weighted average’ node to smooth out the values as much as possible. This was somewhat effective. 

*Phase 10:

I started experimenting with multiple different methods of implementing the feature data with a particular focus on the newer / more advanced elements of UE5. Focusing heavily on the lighting and particle systems. I will cover this design process more thoroughly in the project write up of the thesis. 

*Phase 11:

Having spent a good amount of time working on the feature extraction system there were still some clear issues. I had a final attempt to de-bug these. Firstly The inaccuracy of the peak frequency node. This was addressed by increasing the resolution of the FFT to its maximum 4096 band resolution. Fortunately this did not add as much latency as anticipated and did improve the frequency accuracy, also was still not perfect due to issues with the FFT (please see thesis). 

*Phase 12:

The MFFC returning NaN issue was finally solved. A variable was hard coded to an incorrect number of bins. This has solved the issue making the MFCC node now completely operational.
