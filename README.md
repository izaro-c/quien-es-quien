# Guess Who? in C++

**English** · [Español](README.es.md)

A program that plays *Guess Who?*. It reads a table of characters and their attributes,
builds a binary decision tree of yes/no questions and uses it to guess the character the
user has chosen. It can run in the terminal or with a Qt window that shows the board.

Final assignment for the **Data Structures** course (Double Degree in Computer
Engineering and Mathematics, University of Granada, 2024).

| Start | During the game | End |
|:-:|:-:|:-:|
| ![Start](doc/img/inicio.png) | ![During the game](doc/img/partida.png) | ![End](doc/img/final.png) |

The program asks questions on the left and crosses out the characters that no longer
match. In this game it guesses Pedro after four questions.

## What I implemented

The course provided the `bintree<T>` class, the Qt interface and the base code. My work is
in [`src/quienesquien.cpp`](src/quienesquien.cpp):

- **Building the tree** (`crear_arbol`): at each node it chooses the attribute with the
  highest entropy among the remaining characters, i.e. the question that splits them most
  evenly. Each node stores the question and each leaf stores a character.
- **Playing** (`iniciar_juego`): it goes down the tree according to the answers. In
  graphical mode it crosses out the characters that have been ruled out.
- **Removing redundant nodes** (`eliminar_nodos_redundantes`): it removes questions that
  have only one child.
- **Average leaf depth** (`profundidad_promedio_hojas`): the average number of questions
  needed to guess a character.
- **Adding and removing characters** (`aniade_personaje`, `elimina_personaje`): they
  update an existing tree without rebuilding it. They are implemented but the
  command-line program doesn't use them.
- **Game information** (`informacion_jugada`, `preguntas_formuladas`): the characters that
  are still possible and the questions answered so far.

Average number of questions for the sample boards:

| Characters | Average questions | log₂(n) |
|-----------:|------------------:|--------:|
| 10         | 3.4               | 3.32    |
| 100        | 6.72              | 6.64    |
| 512        | 9                 | 9       |
| 1000       | 10.003            | 9.97    |

## Building

You need a C++17 compiler, CMake 3.16 or later and Qt 5.

```bash
# Fedora:        sudo dnf install qt5-qtbase-devel cmake gcc-c++
# Debian/Ubuntu: sudo apt install qtbase5-dev cmake g++
cmake -S . -B build
cmake --build build
```

## Usage

Run it from the root of the repository:

```bash
# Play in the terminal
./build/bin/quienesquien datos/personajes-1.csv

# Play with the graphical board
./build/bin/quienesquien datos/personajes_graficos.csv datos/Tapada.png

# Build the tree and print it without playing
./build/bin/quienesquien datos/personajes-1.csv limpiar

# Generate a random board (asks for the number of characters)
./build/bin/quienesquien aleatorio
```

Before playing, the program prints the tree. In each question node, the first branch
printed is "no" and the second is "yes":

```
-- Pregunta: ¿Es Mujer? (num_personajes=5)
   |-- Pregunta: ¿Es Ojos marrones? (num_personajes=3)
   |   |-- Personaje: Juan (num_personajes=1)
   |    -- Pregunta: ¿Es Pelo castaño? (num_personajes=2)
   |       |-- Personaje: Antonio (num_personajes=1)
   |        -- Personaje: Ernesto (num_personajes=1)
    -- Pregunta: ¿Es Ojos marrones? (num_personajes=2)
       |-- Personaje: Ana (num_personajes=1)
        -- Personaje: Pilar (num_personajes=1)
Profundidad promedio de las hojas del arbol: 2.4
```

## Board format

Tab-separated files. The first row contains the attributes and ends with
`Nombre personaje`. Each following row has a 0 or 1 for each attribute, the character's
name and, optionally, the path to their image:

```
Mujer	Ojos marrones	Pelo castaño	Tiene gafas	Nombre personaje
0	1	1	1	Ernesto
1	0	0	0	Ana
```

## Structure

```
include/   headers
src/       source code (the game logic is in quienesquien.cpp)
datos/     sample boards and character images
doc/       screenshots and Doxygen main page
```

## Credits

- Game logic: Izaro Cuerdo Larraza
- `bintree<T>` class: Miguel García Silvente and Juan F. Huete Guadix
- Qt interface (`consola`, `ventana`, `tablerografico`): Rosa Mª Rodríguez Sánchez
