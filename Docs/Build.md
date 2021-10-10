# Sestavení ze zdrojového kódu
Přehled podporovaných platform je uveden v [tabulce](../README.md) v souboru README.md.

## 1. Získání zdrojového kódu
Nejprve je nutno stáhnout zdrojový kód. Přepínač `--recursive` zajistí to, 
že budou spolu s hlavním repozitářem staženy i git submoduly s knihovnami.
````shell script
git clone --recursive https://gitlab.fel.cvut.cz/i3t-diplomky/i3t-bunny.git
````

Nebyly-li všechny submoduly inicializovány nebo byl-li nějaký v nové verzi
odebrán, je nutné je znovu inicializovat.
````shell
git submodule update --init --recursive
````

## Instalace potřebných závislotí
### Linux (aptitude)
````shell
apt install libgl-dev libglx-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev -y
````

## 2. CMake
Sestavení projektu je realizováno pomocí utility `cmake`. Všechny potřebné závislosti
jsou obsažené v adresáři `Dependencies/`.

Adresář, ve kterém je CMake spuštěn se bere jako adresář výstupní, do nejž jsou 
vygenrovány soubory pro build systémy (Makefile, *.vxproj, ...).

````shell script
# In the project root create cmake output directory 'out/' and move into it.
mkdir out && cd out

# Build in the directory MSVC project files.
cmake -DCMAKE_BUILD_TYPE=Debug -A Win32 -G "Visual Studio 16 2019" ..
# or for Code-Blocks projekt on Linux
cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ..
````
Tento příklad spuštění CMake z příkazové řádky vygeneruje soubory projektu pro Visual 
 Studio 2019 v adresáři, ve kterém je CMake spuštěn, a to pro konfiguraci ``Debug | x86``.
Dvě tečky uvádí cestu k hlavnímu souboru ``CMakeLists.txt``, který se nachází v kořenovém
adresáři projektu.

Výsledné binární soubory, sdílené knihovny a spustitelný soubor IT3(.exe), jsou
vytvořeny v adresáři ``Binaries`` do podadresáře odpovídajícímu typu konfigurace.

## Poznámka k překladu ve Visual Studiu 15 2017 (PF)
- Ve Visual studiu jsem musel stáhnout:
  - Vývoj desktopových aplikací pomocí C++ 
	- Nástroje Visual C++ pro CMake
	- Windows 8.1 SDK a UCRT SDK (vytvořený projekt ho vždy obsahuje)
- Viz také 	
  - Vývoj her pomocí C++ / Windows 8.1 SDK a UCRT SDK (vytvořený projekt SDK 8.1 vždy obsahuje)
  - Vývoj pro Linux pomocí C++ / Nástroje Visual C++ pro CMake a Linux
- Použil jsem mkdr build;cd build; cmake-gui ..
  - při spuštění jsem nastavil Visual Studio 15 2017	
  - a pak proměnné 
	- CMAKE_BUILD_TYPE=Debug 
	- ASSIMP_BUILD_TESTS - prázdné (nezakliknuto) - vadilo při překladu variantou C ++17,
    - Hack pro assimp nepomohl #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING")

	
## .vsconfig  
````
{
	"version": "1.0",
	"components": [
		"Microsoft.VisualStudio.Workload.NativeDesktop",
		"Microsoft.VisualStudio.Workload.Universal",
		"Microsoft.VisualStudio.Workload.NativeGame",
		"Microsoft.VisualStudio.Workload.NativeCrossPlat",
		"microsoft.visualstudio.component.intellitrace.frontend",
		"microsoft.visualstudio.component.debugger.justintime",
		"microsoft.visualstudio.component.vc.diagnostictools",
		"microsoft.visualstudio.component.vc.cmake.project",
		"microsoft.visualstudio.component.vc.atl",
		"microsoft.visualstudio.component.vc.testadapterforboosttest",
		"microsoft.visualstudio.component.vc.testadapterforgoogletest",
		"microsoft.visualstudio.componentgroup.nativedesktop.win81",
		"microsoft.visualstudio.componentgroup.uwp.vc",
		"component.linux.cmake"
	]
}
````
