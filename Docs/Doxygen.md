Doxygen version 1.8.8
Copyright Dimitri van Heesch 1997-2014

You can use doxygen in a number of ways:

1) Use doxygen to generate a template configuration file:
    doxygen [-s] -g [configName]

    If - is used for configName doxygen will write to standard output.

2) Use doxygen to update an old configuration file:
    doxygen [-s] -u [configName]

3) Use doxygen to generate documentation using an existing configuration file:
    doxygen [configName]

    If - is used for configName doxygen will read from standard input.

4) Use doxygen to generate a template file controlling the layout of the
   generated documentation:
    doxygen -l layoutFileName.xml

5) Use doxygen to generate a template style sheet file for RTF, HTML or Latex.
    RTF:        doxygen -w rtf styleSheetFile
    HTML:       doxygen -w html headerFile footerFile styleSheetFile [configFile]
    LaTeX:      doxygen -w latex headerFile footerFile styleSheetFile [configFile]

6) Use doxygen to generate a rtf extensions file
    RTF:   doxygen -e rtf extensionsFile

If -s is specified the comments of the configuration items in the config file will be omitted.
If configName is omitted `Doxyfile' will be used as a default.

-v print version string
