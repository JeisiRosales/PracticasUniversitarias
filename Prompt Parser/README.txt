README — Prompt Parser (Proyecto de Lenguajes de Programación)

Descripción del Proyecto:
     El Prompt Parser es un programa en lenguaje C diseñado para analizar, clasificar y validar prompts escritos en un archivo de texto (.pmt) según una gramática formal definida en EBNF. El objetivo es reconocer correctamente cuatro tipos de prompts:
- Brilla
- Avanza
- Crear
- Fluye

    Cada tipo de prompt sigue una estructura específica compuesta por campos clave como: Breve contexto, Rol deseado, Intención, Lenguaje, Lógica, Aspecto, etc. El parser recorre el archivo, verifica si cada prompt cumple la estructura definida en la gramática y genera mensajes de validación.

Objetivos del Proyecto:
- Implementar un analizador sintáctico (parser) manual en C.
- Aplicar una gramática EBNF para reconocer estructuras complejas.
- Leer y procesar un archivo de texto con múltiples prompts.
- Detectar errores de sintaxis y clasificar cada prompt.
- Comprender la aplicación de gramáticas formales en lenguajes de programación.

Estructura EBNF

<Archivo>::= {<Prompt>}+
<Prompt>::= <Brilla>|<Avanza>|<Crear>|<Fluye>

<Brilla>::= <BreveContexto><SaltoLi> <RolDeseado><SaltoLi> <Intencion><SaltoLi> <Lenguaje><SaltoLi> <Logica><SaltoLi> <Aspecto><FinPrompt>
<BreveContexto>::= 'Breve contexto:' <Espacio> <Texto> <SaltoLi>
<RolDeseado>::= 'Rol deseado:' <Espacio> ['Actua como'] <Texto> <SaltoLi>

<Avanza>::= <Antecedentes><SaltoLi> <Vision><SaltoLi> <Intencion><SaltoLi> <Lenguaje><SaltoLi> <Logica><SaltoLi> <Aspecto><FinPrompt>
<Antecedentes>::= 'Antecedentes:' <Espacio> <Texto> <SaltoLi>
<Vision>::= 'Visión o propósito:' <Espacio> <Texto> <SaltoLi>
<Intencion>::= 'Intención o meta:' <Espacio> <Comando> <Espacio> <Texto> <SaltoLi>
<Lenguaje>::= 'Lenguaje o tono:' <Espacio> <Texto> <SaltoLi>
<Logica>::= 'Lógica y estructura:' <Espacio> <Texto> <SaltoLi>
<Aspecto>::= 'Aspecto del resultado:' <Espacio> <Texto> 

<Crear>::= <Contexto><SaltoLi> <Rol><SaltoLi> <Enfoque><SaltoLi> <Accion><SaltoLi> <Resultado><FinPrompt>
<Contexto>::= 'Contexto:' <Espacio> <Texto> <SaltoLi>
<Rol>::= 'Rol:' <Espacio> ['Actua como'] <Texto> <SaltoLi>
<Enfoque>::= 'Enfoque:' <Espacio> <Texto> <SaltoLi>
<Accion>::= 'Acción:' <Espacio> <Comando> <Espacio> <Texto> <SaltoLi>
<Resultado>::= 'Resultado:' <Espacio> <Texto> 

<Fluye>::= <Foco><SaltoLi> <Lugar><SaltoLi> <Usuario><SaltoLi> <YoInterior><SaltoLi> <Ejercicio><FinPrompt>
<Foco>::= 'Foco:' <Espacio> <Texto> <SaltoLi>
<Lugar>::= 'Lugar:' <Espacio> <Texto> <SaltoLi>
<Usuario>::= 'Usuario:' <Espacio> <Texto> <SaltoLi>
<YoInterior>::= 'Yo Interior:' <Espacio> <Texto> <SaltoLi>
<Ejercicio>::= 'Ejercicio:' <Espacio> <Texto>

<FinPrompt>::= <SaltoLi> <SaltoLi>
<SaltoLi> ::= "\n" | "\r\n" 
<Comando>::= 'Describe' | 'Analiza' | 'Compara' | 'Explica como' | 'Lista'
<Texto>::= {<Caracter>}+
<Caracter>::= <Letra>|<Digito>|<Espacio>|<Simbolo>
<Digito>::= 0|1|2|3|4|5|6|7|8|9
<Espacio>::= ' '
<Simbolo>::= ','|';'|':'|'¿'|'?'|'¡'|'!'|'('|')'|'['|']'|'{'|'}'|'-'|'_'|'/'|'*'|'+'|'='|'<'|'>'|'$'|'%'|'&'|'@'|'#'|'"'
<Letra>::= 'a'|'b'|'c'|'d'|'e'|'f'|'g'|'h'|'i'|'j'|'k'|'l'|'m'|'n'|'ñ'|'o'|'p'|'q'|'r'|'s'|'t'|'u'|'v'|'w'|'x'|'y'|'z'|
           'A'|'B'|'C'|'D'|'E'|'F'|'G'|'H'|'I'|'J'|'K'|'L'|'M'|'N'|'Ñ'|'O'|'P'|'Q'|'R'|'S'|'T'|'U'|'V'|'W'|'X'|'Y'|'Z'|
           'á'|'é'|'í'|'ó'|'ú'|'ü'|
           'Á'|'É'|'Í'|'Ó'|'Ú'|'Ü'