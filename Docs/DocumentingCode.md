# Jak dokumentovat kód
Následující soubor obsahuje užitečné příklady a triky pro Doxygen.

## Skupiny (Groups)
Často se hodí seskupit takové členské funkce tříd, které mají něco společného.
Doxygen to umožňuje pomocí značek.
````c++
///@{
    ...
///@}
````
Pozor, nelze zanožit zkupinu do skupiny.

[Odkaz na Doxygen](https://www.doxygen.nl/manual/grouping.html#memgroup)
