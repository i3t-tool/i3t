# Tutorials
Tutorials are a way to teach the user how to use I3T and how transformations in 3D work.

## Types of tutorials
- **Main Tutorials** (_TUT) - contain instructions, tips and tasks, usual goal is to teach 
the user some essential information for interaction with i3t
- **Excercise** (EXC_) - contain a singular complicated goal, that is described using tasks and screenshots
- **Customs** (other formats) - additional tutorials, one offs

You can create new tutorials by looking at old versions and learning from them.

## How to create tutorials
- Create a new folder in ``Data/Tutorials``
    - Each tutorial needs a separate folder to store the .tut file and all images
    - Name the folder according to the tutorial type
- Create a file with the same name as the folder with the .tut file type
- Open the file and paste this header into it:
````
    ---
  title: title_here
  description: description_here
  thumbnail: image_to_use_as_thumbnail.png
  language: cs
  scene: scene_to_load_on_start.scene
    ---
````
- Modify the header parameters as needed
    - **title:** title of the tutorial in start menu
    - **description:** description of the tutorial in start menu
    - **thumbnail:** image to be loaded into start menu
    - **language:** language to be used in the file, either "cs" or "en"
    - **scene:** scene to be loaded into the workspace when tutorial is selected (optional)
- Write the body of the tutorial - markdown language
    - pages are divided by "--" symbol as demonstrated here:

````
  Hello! welcome to the tutorial on how to work with I3T.

  Click on the next button to continue.
  --                                                      //this divides the pages
  First you have to learn how to add blocks into a scene.
````

## Keywords
- Tutorials use keywords to render pre-made elements into the tutorial
- We use these keywords by typing their name, followed by a colon
- Example:
````     
  headline: Adding an object

  task: Add an object into the scene!

  hint:
  You can add objects using the add menu.
  Select the object you need from the menu to add it.

  Now that you added the object continue to the next page.
````
You can see that we have two notations:
- **single line** - keyword formats the following line (*task* in the example)
- **multiple line** - keyword formats all lines until newline (*hint* in the example)

All keywords:
- **headline** - used to create a heading to a page, makes the text larger and bold
- **task** - used to signify a task for the user, adds a blue square and a gray border
- **hint** - used to show a blue hint button, that expands and shows the text to the the user
  Other functions:
- \*foo\*  - used to make text bold
- **lists** - used to create itemised lists
````
    * item1
    * item2
    * item3
      ...
````
## Images
Adding images: ``![description](image.png)``
- descriptions appear in tooltips if an image is hovered over
- ``image.png`` signifies the path to the image that should be shown
- *images need to be in the tutorial directory to be loaded!*


That is everything you need to create a tutorial in I3T, good luck!

