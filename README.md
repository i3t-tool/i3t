# I3T
Věnujte prosím velkou pozornost [programovacím konvencím projektu](Docs/Conventions.md).

## Sestavení
Následující tabulka popisuje, na jaké platformě s jakými nástroji je projekt možné sestavit.
Naší ambicí je, aby fungoval na všech uvedených platformách.

| Platforma  | Arch.       | Konfigurace | CMake generátor      | Kompilátor | Stav  |
|------------|-------------|-------------|----------------------|------------|-------|
| Windows    | Win32, x64  | Debug       | Visual Studio        | cl.exe     | ✅    |
| Windows    | Win32, x64  | Debug       | NMake Makefiles      | cl.exe     | ✅    |
| Linux      | x86/x64     | -           | Default              | g++/clang  | ⛔    |
| Unix       | x86/x64     | -           | Default              | clang      | ⛔    |

Pro sestavení postupujte prosím dle [návodu](Docs/Build.md).

## Struktura projektu
````
Dependencies/           Adresář s knihovnami, které I3T potřebuje.
Docs/
    - Doxygen/
Source/
    - Commands/             Definice příkazů (událostí). 
    - Core/                 Základní funkcionality.
    - GUI/                  ImGui okna, dialogy a modální okna.
        - Elements/         Jednotlivé GUI elementy.
            - Dialogs/      Vyskakovací okna. 
            - Modals/       Vyskakovací blokující okna.
            - Windows/      Regulerní dokovací okna.
        - ImGui/            Implementace ImGui callbacků pro GLFW.
Test/                       Adresář s testy, kopíruje strukturu adresáře 'Source/'
````

## Přispívání - Git
Dbejte prosím na to, abyste zahrnovali k verzování (``git add``) pouze zdrojový kód 
a soubory potřebné pro sestavení nebo pro správné fungování aplikace (konfigurace, data).

Řekněte gitu, aby ignoroval to, co nemá v repozitáři být, jako je například cmake output.
- [Jak psát .gitignore](https://www.atlassian.com/git/tutorials/saving-changes/gitignore)

## Konvence
Takto má vypadat kód v C++, pozorně čtěte [konkrétní pravidla zde](Docs/Conventions.md).

````cpp
/**                                                             
 * \file        GUI\EditBox.h       
 * \author      Jméno Příjmení
 * \date        24.10.2020
 *                                                               
 * Declares the edit box class.                                   
 */                                                             
#ifndef _EDIT_BOX_H_
#define _EDIT_BOX_H_

#include <string>

const int g_val = 0;

class EditBox
{
public:
  void setName(const std::string& name)
  {
    m_boxName = name;
  }

private:
  std::string m_boxName;
};

#endif // _EDIT_BOX_H_
```` 
