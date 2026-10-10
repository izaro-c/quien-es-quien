/**
 * @file quienesquien.h
 * @brief Fichero cabecera del QuienEsQuien.
 *
 * Almacena el árbol de preguntas para jugar al ¿Quién es quién?.
 */

#ifndef _QUIENESQUIEN_H_
#define _QUIENESQUIEN_H_

#define DEBUG_QUIENESQUIEN 0

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <cassert>
#include <set>
#include <sstream>
#include "pregunta.h"
#include "bintree.h"
#include "ventana.h"
#include "consola.h"
#include "tablerografico.h"
using namespace std;

/**
 * @brief T.D.A. QuienEsQuien
 *
 * Una instancia del tipo de dato abstracto QuienEsQuien permite almacenar y manipular un árbol
 * binario de personajes con sus atributos.
 *
 * El TDA QuienEsQuien proporciona operaciones para crear un árbol, optimizar el árbol,
 * iniciar el juego y formass de modificación del árbol como añadir y eliminar personajes ajustando
 * por tanto el árbol de preguntas en base a los cambios.
 *
 * @authors Ángel Olmo Sáez e Izaro Cuerdo Larraza
 */

class QuienEsQuien{
private:
	/**
	  * @brief Almacena el nombre de los personajes. Su índice en
	  *        este vector corresponde con el índice de su
	  *        descripción en el tablero.
	  */
	vector<string> personajes;

	 /**
      * @brief Almacena el nombre de las imagenes en disco de los  personajes. Su índice en
      *        este vector corresponde con el índice de su
      *        descripción en el tablero.
      */
    vector<string> personajes_images;


	/**
	  * @brief Almacena los atributos.  Su índice en este vector
	  *        corresponde con el índice correspondiente de cada
	  *        vector del tablero.
	  */
	vector<string> atributos;

	/**
	  * @brief Matriz que almacena para cada personaje su valor para
	  *        cada atributo.
	  */
	vector<vector<bool>> tablero;

	/**
	  * @brief Arbol de preguntas para adivinar personajes.
	  */
	bintree<Pregunta> arbol;

	/**
	  * @brief Nodo del arbol de preguntas correspondiente a la jugada
	  *        actual.
	  */
	bintree<Pregunta>::node jugada_actual;

	 /**
     * @brief tg es el tablero grafico donde se muestra los personajes
     */
    TableroGrafico *tg;

    /**
     * @brief con consola para interactuar con el usuario
     */
    Consola *con;

    /**
     * @brief imagen_ocultar nombre de la imagen en disco para ocultar el personaje
     */
    string imagen_ocultar;

    /**
     * @brief modo_graph flag que indica si se usa el modelo grafico o solo texto
     */
    bool modo_graph;


	/**
	  * @brief Construye un árbol binario de preguntas para identificar personajes en base
	  * a los argumentos pasados, haciéndolo de forma recursiva
	  *
	  * @param atributos Vector de strings que representa los nombres de los atributos.
	  *                  Cada atributo corresponde a una pregunta posible.
	  * @param personajes Vector de strings que contiene los nombres de los personajes
	  *                   involucrados en el juego.
	  * @param personajes_restantes Vector booleano donde cada elemento indica si un
	  *                              personaje específico sigue siendo considerado
	  *                              para la construcción del árbol.
	  * @param tablero Matriz de booleanos que representa las respuestas de cada personaje
	  *                a cada atributo (true para "sí" y false para "no").
	  *
	  * @return Un árbol binario de preguntas que permite diferenciar
	  *         los personajes basándose en sus características.
	  *
	  */
	 bintree<Pregunta> crear_arbol(vector<string> atributos,
                                    vector<string> personajes,
                                    vector<bool> personajes_restantes,
                                    vector<vector<bool>> tablero);
public:
	/**
	  * @brief Constructor básico de la clase
	  */
	QuienEsQuien();
	/**
	  * @brief Constructor de copia.
	  * @param quienEsQuien QuienEsQuien a copiar.
	  */
	QuienEsQuien(const QuienEsQuien &quienEsQuien);
	/**
	  * @brief Destructor del pregunta.
	  */
	~QuienEsQuien();
	/**
	  * @brief Vacia todos los datos del QuienEsQuien receptor.
	  */
	void limpiar();
	/**
	* @brief Sobrecarga del operador de asignación.
	* @param quienEsQuien objeto a copiar.
	* @return Referencia al objeto copiado.
	*/
	QuienEsQuien& operator = (const QuienEsQuien &quienEsQuien);

	/**
	  * @brief Operador de extracción de flujo.
	  * @param is Stream de entrada
          * @param quienEsQuien Quien es quien a leer.
	  * @return Referencia al stream de entrada.
	  *
	  * Lee de @e is un quien es quien y lo almacena en @e
	  * quienEsQuien. El formato aceptado para la lectura es un TSV
	  * (tab-separated values) en el que las columnas tienen cabecera y
	  * son las preguntas. La última columna corresponde al nombre del
	  * personaje. Tras la cabecera se especifica en cada línea un
	  * personaje, teniendo el valor 1 o 0 si tiene/no tiene el
	  * atributo de la columna. En la última columna se da el
	  * nombre del personaje.
	  */
	friend istream& operator >> (istream& is, QuienEsQuien &quienEsQuien);

	/**
	  * @brief Operador de inserción de flujo.
	  * @param os Stream de salida
	  * @param quienEsQuien Quien es quien a escribir.
	  * @return Referencia al stream de salida.
	  *
	  * Escribe en @e is un quien es quien.
	  * El formato usado para la escritura es un TSV
	  * (tab-separated values) en el que las columnas tienen cabecera y
	  * son las preguntas. La última columna corresponde al nombre del
	  * personaje. Tras la cabecera se especifica en cada línea un
	  * personaje, teniendo el valor 1 o 0 si tiene/no tiene el
	  * atributo de la columna. En la última columna se da el
	  * nombre del personaje.
	  */
	friend ostream& operator << (ostream& os, const QuienEsQuien &quienEsQuien);

	/**
	  * @brief Escribe en la salida estandard las estructuras atributos, personajes y tablero.
	  */
	void mostrar_estructuras_leidas();

	/**
	  * @brief Este metodo construye el arbol de preguntas para todos los personajes del tablero.
	  */
	bintree<Pregunta> crear_arbol();

	/**
	  * @brief Sustituye el arbol actual por el arbol pasado por parámetro.
	  *
	  * @param arbol_nuevo Arbol de preguntas que sustituye al actual.
	  *
	  */
	void usar_arbol(bintree<Pregunta> arbol_nuevo);

	/**
	  * @brief Escribe el arbol generado en la salida estandard.
	  */
	void escribir_arbol_completo() const;

	/**
	* @brief Metodo que recorre el arbol a partir de un nodo de forma recursiva
	* buscando donde hay preguntas redundantes y sustituyendolas por su hijo no nulo
	* @param nodo_actual Nodo a partir de el cuál se eliminan los nodos redundantes
	* @post El arbol de preguntas se modifica.
	*/
	void eliminar_nodos_redundantes_aux(bintree<Pregunta>::node nodo_actual);

	/**
	  * @brief Metodo que modifica el arbol de preguntas para que no haya
	  *        preguntas redundantes.
	  * @post El árbol de preguntas se modifica.
	  */
	void eliminar_nodos_redundantes();

	/**
	  * @brief Inicializa el juego.
	  * @post  Si la partida anterior no había terminado se
	  *        pierde el progreso.
	  */
	void iniciar_juego();

	/**
	  * @brief Dado un estado del tablero devuelve los nombres de
	  *        los personajes que aún no han sido tumbados en el
	  *        tablero.
	  *
	  * @param jugada_actual Nodo del estado del tablero.
	  *
	  * @return Conjunto de personajes que no han sido tumbados en el tablero.
	  *
	  * @pre El arbol del QuienEsQuien receptor debe haber sido construido previamente.
	  * @pre El nodo indicado debe ser un nodo del arbol del QuienEsQuien receptor.
	  */
	set<string> informacion_jugada(bintree<Pregunta>::node jugada_actual);

	/**
	 * @brief Función auxiliar para calcular la profundidad promedio de las hojas
	 * @param n Nodo actual del subárbol en el que se realiza el cálculo.
	 * @param profundidad Entero que representa la profundidad actual del nodo `n`.
	* @param suma_profundidades Referencia a un acumulador que guarda la suma de
 *                              las profundidades de todas las hojas encontradas.
	* @param numero_hojas Referencia a un contador que mantiene el número total
 *                        de hojas encontradas en el subárbol.
	 */
	void calcular_profundidad(bintree<Pregunta>::node n, int profundidad, float &suma_profundidades, float &numero_hojas);

	/**
	  * @brief Este método permite calcular la media de la profundidad de las hojas del árbol.
	  * Este valor representa el número (promedio) de preguntas necesarias para adivinar cada personaje.
	  * A menor profundidad promedio, mejor solución. Esta métrica es un indicador para evaluar la calidad de vuestra solución.
	  *
	  * @return Profundidad media del arbol de preguntas.
	  *
	  * @pre El arbol de preguntas debe haber sido construido previamente.
	  */
	float profundidad_promedio_hojas();

	/**
	 * @brief Genera una descripción de las preguntas formuladas y las respuestas dadas
	 *        desde la raíz hasta el nodo actual del árbol de preguntas.
	 *
	 * @param jugada Nodo actual en el árbol de preguntas.
	 * @return Una cadena de texto con la descripción de las preguntas y respuestas.
	 */
	std::string preguntas_formuladas(bintree<Pregunta>::node jugada);

	/**
	 * @brief Desciende en el árbol desde la raíz hasta un nodo hoja basándose en las características.
	 *
	 * @param caracteristicas Un vector booleano que representa las respuestas a los atributos
	 *                        (true para "sí" y false para "no") que guían el descenso en el árbol.
	 * @return bintree<Pregunta>::node Nodo hoja encontrada en el árbol.
	 */
	bintree<Pregunta>::node encontrar_nodo(const vector<bool> &caracteristicas);

	/**
	 * @brief Añade un nuevo personaje al árbol de preguntas.
	 *
	 * @param nombre Nombre del nuevo personaje.
	 * @param caracteristicas Vector de características (bool) del nuevo personaje.
	 * @param nombre_imagen_personaje Nombre de archivo de imagen asociado al personaje (opcional).
	 */
	void aniade_personaje(string nombre, vector<bool> caracteristicas, string nombre_imagen_personaje = "");

	/**
	 * @brief Elimina un personaje del árbol y actualiza el tablero.
	 *
	 * @param nombre Nombre del personaje a eliminar.
	 */
	void elimina_personaje(string nombre);

	/**
	  * @brief Rellena los datos del QuienEsQuien con un tablero calculado aleatoriamente.
	  *
	  * @param numero_de_personajes Número de personajes que tiene el tablero a crear.
	  */
	void tablero_aleatorio(int numero_de_personajes);

    /**
     * @brief setImagenOcultar Modifica el nombre de la imagen en disco para ocultar los personajes en modo
     * grafico
     * @param n:nombre del fichero en disco
     */
    void setImagenOcultar(const char * n);

    /**
     * @brief setModoGraph Modifica el modelo grafico
     * @param m: True si se desea modo grafico false solo texto
     */
    void setModoGraph(bool m);

     /**
     * @brief Oculta en el tablero grafico los personajes que no estan activos
     * @param personajes_activos: personajes que están activos
     */
    void ocultar_personajes_graph(const set<string> &personajes_activos);


};

#endif

