THIS FILE DETAILS HOW TO CREATE AND WRITE TUTORIALS FOR I3T (AS OF 2023)
________________________________________________________________
TYPES OF TUTORIALS
- Main Tutorials - _TUT - contain instructions, tips and tasks, usual goal is to teach
                          the user some essential information for interaction with i3t
- Excercise - EXC_ - contain a singular complicated goal, that is described using tasks and screenshots
- Customs - other formats - additional tutorials, one offs

You can create new tutorials by looking at old versions and learning from them.

Alternatively:
HOW TO CREATE TUTORIALS_______________________________________________________
- Create a new folder in Data/Tutorials
    - Each tutorial needs a separate folder to store the .tut file and all images
    - Name the folder according to the tutorial type
- Create a file with the same name as the folder with the .tut file type
- Open the file and paste this header into it:
"""
    ---
    title: title_here
    description: description_here
    thumbnail: image_to_use_as_thumbnail.png
    language: cs
    scene: scene_to_load_on_start.scene
    ---
"""
- Modify the header parameters as needed
    - title: title of the tutorial in start menu
    - description: description of the tutorial in start menu
    - thumbnail: image to be loaded into start menu
    - language: language to be used in the file, either "cs" or "en"
    - scene: scene to be loaded into the workspace when tutorial is selected (optional)
- Write the body of the tutorial - markdown language
    - pages are divided by "--" symbol as demonstrated here:
"""
    Hello! welcome to the tutorial on how to work with I3T.

    Click on the next button to continue.
    --                                                      //this divides the pages
    First you have to learn how to add blocks into a scene.
"""
KEYWORDS_____________________________________________________________________
    - tutorials use keywords to render premade elements into the tutorial
    - we use these keywords by typing their name, followed by a colon, example:
"""
    headline: Adding an object

    task: Add an object into the scene!

    hint:
    You can add objects using the add menu.
    Select the object you need from the menu to add it.

    Now that you added the object continue to the next page.
"""
You can see that we have two notations:
    - single line - keyword formats the following line (task in the example)
    - multiple line - keyword formats all lines until newline (hint in the example)
All keywords:
    - headline - used to create a heading to a page
    - task - used to signify a task for the user
    - hint - used to show a button, that expands and shows a tip for the user
Other functions:
    - *foo* - used to make text bold
    - lists - used to create itemised lists
        - item1
        - item2
        - item3
        ...
IMAGES___________________________________________________________
Adding images: ![description](image.png)
    - descriptions appear in tooltips if an image is hovered over
    - !!!images need to be in the tutorial directory to be loaded!!!


MISC_____________________________________________________________
Loading the english/czech version should be done according to a determining string in filename (currently - "en" as last two characters)
English versions should be loaded in the same way as the czech versions and reloaded on language change
Tutorials are always ordered in alphabetical order according to the filename


That is everything you need to create a tutorial in I3T, good luck!
Adam Loucký
