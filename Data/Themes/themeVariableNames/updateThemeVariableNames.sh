#/bin/sh
# simple replacement of item keys in Theme files
# run in Themes directory as ./themeVariableNames/updateThemeVariableNames.sh
# do not include in the distribution

for file in *.yml
do
  echo "Working on $file file..."
#  sed -i 's/nops_Item selected border/ngen_Item selected border/g' $file
#  sed -i 's/nops_General node Background/ngen_General node Background/g' $file
#  sed -i 's/nops_General node Header/ngen_General node Header/g' $file
#  sed -i 's/npop_Connection is possible/npop_Connection is possible (text)/g' $file
#  sed -i 's/npop_Connection is not possible/npop_Connection is not possible (text)/g' $file  
  sed -i 's/Bacground/Background/g' $file  
done