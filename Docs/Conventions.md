# Konvence
Aneb, jak by měl kód od každého přispěvatele vypadat.
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

### Doporučení pro C++
- **Nikdy** nepoužívat `using namespace std;`. Může kvůli tomu docházet ke konfliktům.
- **Nikdy** nepoužívat v cestách velmi velmi ošklivá zpětná lomítka `path\\to\\file`, je preferován POSIX 
styl `path/to/file` (Windows „umí“ i tyto cesty).
- `auto` proměnné jen pro iterátory nebo pro přímou inicializaci proměnných. Například
`auto map = std::map<std::string, GUI::Elements::Tab>`.

### Struktura kódu
- **Statické proměnné** deklarujeme na konci `.cpp` souboru.
- **Hlavičkové soubory** includujeme ve skupinách oddělených prázdným řádkem,
a to v následujícím pořadí:
    - **1.** hlavičkový soubor přidružený k danému `.cpp` souboru,
    - **2.** STL knikovny,
    - **3.** externí knihovny projektu,
    - **4.** interní hlavičkové soubory.

### Formátování
Pravidla formátování kódu jsou definována v souboru `.clang-format`. Nelze, aby každý kus kódu
byl odsazen jiným počtem mezer a podobně. IDE umí pravidla formátování načíst a na jejich nedodržení upozornit.

Obecně platí, že je podoba našeho kódu založena na stylu kódu Microsoftu. Upozornil bych zejména na psaní
otevřené závorky bloku kódu na nový řádek (viz příklad).

- Testováno pro **CLion**, **Visual Studio** a nainstalovaný **ReSharper** (Mohou detekovat `.clang-format`
automaticky, pokud ne, je třeba podporu ručně nastavit.)
    - Nastavení pro [CLion](https://www.jetbrains.com/help/clion/clangformat-as-alternative-formatter.html#clion-support)

### Pojmenovávání
- Všechny **třídy, struktury a výčtové typy** ``ClassName, StructName`` pojmenovávat ve stylu camel-case s počátečním velkým písmenem.
- Všechny **proměnné** a **funkce** ``myVariable`` pojmenovávat také ve stylu camel-case, ale s malým počátečním písmenem.
- Všechny **C++ soubory** ``FileUtils.[cpp/h]`` pojmenovávat stejně jako třídy a struktury.
- **Globální proměnné** s prefixem `g_`.
- **Členské proměnné** stejně jako proměnné, ale s prefixem `m_`.
- **Členské statické** proměnné s prefixem `s_`.