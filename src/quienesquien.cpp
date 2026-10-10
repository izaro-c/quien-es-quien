#include "../include/quienesquien.h"
#include <sstream>
#include <iostream>
#include <iterator>
#include <math.h>
#include <algorithm>

// Funciones para modularizar
template<typename T>
int indice(const std::vector<T> &v, const T &elemento) {
     int indice = -1;

     for (int i = 0; i < v.size(); ++i) {
          if (v[i] == elemento) {
               indice = i;
               break;
          }
     }

     return indice;
}

QuienEsQuien::QuienEsQuien(){
}

QuienEsQuien::QuienEsQuien(const QuienEsQuien &quienEsQuien){
}

QuienEsQuien& QuienEsQuien::operator= (const QuienEsQuien &quienEsQuien){
     return *this;
}

QuienEsQuien::~QuienEsQuien(){
     this->limpiar() ;
}

void QuienEsQuien::limpiar(){
     this->personajes.clear();
     this->atributos.clear();
     this->personajes_images.clear();
     for(vector<vector<bool>>::iterator it_tablero = this->tablero.begin();
         it_tablero != this->tablero.end();
         it_tablero++){
          (*it_tablero).clear();
     }
     this->tablero.clear();
     this->arbol.clear() ;
}

template <typename T>
std::ostream& operator<<  (std::ostream& out, const std::vector<T>& v) {
     if ( !v.empty() ) {
          out <<  '[' ;
          std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
          out <<  "\b\b]";
     }

     return out;
}

void QuienEsQuien::mostrar_estructuras_leidas(){
     cout <<  "personajes: " <<  (this->personajes) <<  endl;
     cout <<  "atributos: " <<  (this->atributos) <<  endl;
     cout <<  "tablero:    " <<  endl;
     // Escribe la cabecera del tablero
     for(vector<string>::iterator it_atributos = this->atributos.begin() ;
         it_atributos != this->atributos.end() ;
         it_atributos++){
          cout <<  *it_atributos << "\t";
     }
     cout << endl;

     int indice_personaje = 0;
     for(vector<vector<bool>>::iterator it_tablero_atributos = tablero.begin();
         it_tablero_atributos!= tablero.end();
         it_tablero_atributos++){
          string personaje = this->personajes[indice_personaje];
          int indice_atributo = 0;
          for(vector<bool>::iterator it_atributos_personaje = (*it_tablero_atributos).begin();
              it_atributos_personaje != (*it_tablero_atributos).end() ;
              it_atributos_personaje++){
               cout <<  *it_atributos_personaje<< "\t";
               indice_atributo++;
          }
          cout <<  personaje <<  endl;
          indice_personaje++;
     }
}

/**
  * @brief Devuelve una copia de la cadena original sin las subcadenas no deseadas.
  *
  * @param cadena_original Cadena de la que se eliminan las subcadenas no deseadas.
  * @param cadena_a_eliminar Subcadena que se busca y se elimina.
  *
  * @return Copia de la cadena original sin las subcadenas no deseadas.
  */


string limpiar_string(string cadena_original,string cadena_a_eliminar){
     string linea(cadena_original);
     while(linea.find_first_of(cadena_a_eliminar) != std::string::npos){
          linea.erase(linea.find_first_of(cadena_a_eliminar),cadena_a_eliminar.length());
     }

     return linea;
}

istream& operator >>  (istream& is, QuienEsQuien &quienEsQuien) {
     quienEsQuien.limpiar();
     if(is.good()){ //Lee la linea con los nombres de las columnas
          string linea;
          getline(is, linea, '\n');
          linea = limpiar_string(linea,"\r");
          bool salir=false;
          while(!salir && linea.find('\t') != string::npos ){
               string atributo = linea.substr(0,linea.find('\t'));
               quienEsQuien.atributos.push_back(atributo);
               linea = linea.erase(0,linea.find('\t')+1);
               if (linea.substr(0,linea.find('\t'))=="Nombre personaje")
                   salir=true;
          }
     }
     //LEE los atributos para cada personaje
     while( is.good()) {
          string linea;
          getline(is, linea, '\n');
          linea = limpiar_string(linea,"\r");

          //Si la linea contiene algo extrae el personaje.
          //Si no la ignora.
          if(linea != ""){;
               vector<bool> atributos_personaje;
               int indice_atributo=0;
               bool salir=false;
               while(!salir && linea.find('\t') != string::npos){
                   string valor = linea.substr(0,linea.find('\t'));
                   if (valor == "0" || valor == "1"){
                    bool valor_atributo = valor == "1";
                    atributos_personaje.push_back(valor_atributo);
                    linea = linea.erase(0,linea.find('\t')+1) ;
                    indice_atributo++;
                   }
                   else{
                       salir=true;
                   }
               }
               string nombre_personaje;
               string imagen_personaje;
               if (linea.find('\t') == string::npos){
                nombre_personaje = linea;
                imagen_personaje=""; //no tiene imagen
               }
               else{
                nombre_personaje=linea.substr(0,linea.find('\t'));
                linea = linea.erase(0,linea.find('\t')+1) ;
                imagen_personaje=linea;

               }
               quienEsQuien.personajes.push_back(nombre_personaje);
               quienEsQuien.tablero.push_back(atributos_personaje);
               quienEsQuien.personajes_images.push_back(imagen_personaje);
          }
     }

     quienEsQuien.mostrar_estructuras_leidas();
     quienEsQuien.arbol = quienEsQuien.crear_arbol();
     quienEsQuien.tg=nullptr; //puntero al tablero grafico
     quienEsQuien.con=nullptr; //puntero a la consola
     if (quienEsQuien.modo_graph){
        int nper = quienEsQuien.personajes.size();
        int nrows,ncols=3;
        if (nper<8)
           nrows = nper/ncols;
        else{
            ncols=4;
            nrows = nper/ncols;
        }
        //se crea un tablero grafico
        quienEsQuien.tg = new TableroGrafico(400,400,nrows,ncols);
        //ponemos en el tablero grafico las imágenes
        for (int i=0;i<nper;i++){

            quienEsQuien.tg->putImagen(i/ncols,i%ncols,quienEsQuien.personajes_images[i].c_str());
        }
        //creamos la consola
        quienEsQuien.con=new Consola();
     }
     return is;
}

ostream& operator <<  (ostream& os, const QuienEsQuien &quienEsQuien){
     //Escribimos la cabecera, que contiene los atributos y al final una columna para el nombre
     for(vector<string>::const_iterator it_atributos = quienEsQuien.atributos.begin();
         it_atributos != quienEsQuien.atributos.end();
         it_atributos++){
          os <<  *it_atributos <<  "\t";
     }
     os <<  "Nombre personaje" <<  endl;

     //Rellenamos con ceros y unos cada linea y al final ponemos el nombre del personaje.
     for(int indice_personaje=0;indice_personaje<quienEsQuien.personajes.size();indice_personaje++){
          for(int indice_atributo=0;indice_atributo<quienEsQuien.atributos.size();indice_atributo++){
               os << quienEsQuien.tablero[indice_personaje][indice_atributo] <<  "\t";
          }
          os << quienEsQuien.personajes[indice_personaje] << endl;
     }

     return os;
}

/**
* @brief Convierte un numero a un vector de bool que corresponde
*        con su representadon en binario con un numero de digitos
*              f i j o.
*
* @param n Numero a convertir en binario.
* @param digitos Numero de digitos de la representaddn binaria.
*
* @return Vector de booleanos con la representadon en binario de @e n
*      con el numero de elementos especificado por @e digitos.
*/
vector<bool> convertir_a_vector_bool(int n, int digitos) {
     vector<bool> ret;
     while(n) {
          if (n&1)
               ret.push_back(true);
          else
               ret.push_back(false);

          n>>=1;
     }

     while(ret.size()<digitos)
          ret.push_back(false);

     reverse(ret.begin(),ret.end());
     return ret;
}

bintree<Pregunta> QuienEsQuien::crear_arbol( vector<string> atributos,
                                             vector<string> personajes,
                                             vector<bool> personajes_restantes,
                                             vector<vector<bool>> tablero){

     // Contamos personajes restantes
     int num_restantes = count(personajes_restantes.begin(), personajes_restantes.end(), true);

     // Caso base: Sólo queda un personaje
     if (num_restantes == 1) {

          // Identificamos el índice del personaje restante
          int indice_personaje = indice(personajes_restantes, true);

          // Creamos un nodo hoja con el personaje
          Pregunta pregunta(personajes[indice_personaje], 1);
          return bintree<Pregunta>(pregunta);
    }

     // Buscamos el atributo con la máxima entropía
     int mejor_atributo = -1;
     double max_entropia = -1;

     for (int j = 0; j < atributos.size(); j++) {
          double si = 0, no = 0;

          // Contamos cuántos personajes dicen "sí" o "no" al atributo actual
          for (int i = 0; i < personajes_restantes.size(); i++) {
               if (personajes_restantes[i]) {
                    if (tablero[i][j]) {
                         si++;
                    } else {
                         no++;
                    }
               }
          }

          if (si > 0 && no > 0) {   // Ignoramos atributos que no discriminan entre los personajes
               double p_si = si / num_restantes;
               double p_no = no / num_restantes;
               double entropia = -(p_si * log2(p_si) + p_no * log2(p_no));

               // Actualizamos el mejor atributo si encontramos una entropía mayor
               if (entropia > max_entropia) {
                    max_entropia = entropia;
                    mejor_atributo = j;
               }
          }
     }

     // Si ningún atributo distingue a los personajes restantes (personajes con los
     // mismos atributos), no se puede seguir dividiendo: nos quedamos con el primero
     if (mejor_atributo == -1) {
          cerr << "Aviso: hay personajes indistinguibles en el tablero." << endl;
          int indice_personaje = indice(personajes_restantes, true);
          return bintree<Pregunta>(Pregunta(personajes[indice_personaje], 1));
     }

     // Caso recursivo: dividimos los personajes según el mejor atributo
     Pregunta pregunta(atributos[mejor_atributo], num_restantes); // Creamos la raíz con la pregunta
     bintree<Pregunta> arbol(pregunta); // Inicializamos el árbol con la pregunta como raíz

     // Dividimos los personajes en "sí" y "no" según el mejor atributo
     vector<bool> si_restantes(personajes_restantes.size(), false);
     vector<bool> no_restantes(personajes_restantes.size(), false);

     for (int i = 0; i < personajes_restantes.size(); ++i) {
          if (personajes_restantes[i]) {
               if (tablero[i][mejor_atributo])
                    si_restantes[i] = true;
               else
                    no_restantes[i] = true;
          }
    }

     // Llamadas recursivas para crear los subárboles izquierdo (sí) y derecho (no)
     if (count(si_restantes.begin(), si_restantes.end(), true) > 0) {
          bintree<Pregunta> subarbol_si = crear_arbol(atributos, personajes, si_restantes, tablero);
          if (!subarbol_si.empty()) {
               arbol.insert_left(arbol.root(), subarbol_si);
          }
     }

     if (count(no_restantes.begin(), no_restantes.end(), true) > 0) {
          bintree<Pregunta> subarbol_no = crear_arbol(atributos, personajes, no_restantes, tablero);
          if (!subarbol_no.empty()) {
               arbol.insert_right(arbol.root(), subarbol_no);
          }
     }

     return arbol;
}

bintree<Pregunta> QuienEsQuien::crear_arbol(){
     int indice_atributo = 0;
     vector<bool> personajes_restantes;
     for(vector<string>::iterator it = personajes.begin();it!=personajes.end();it++){
          personajes_restantes.push_back(true);
     }

     return crear_arbol( this->atributos, this->personajes,
                         personajes_restantes, this->tablero);;
}

void QuienEsQuien::usar_arbol(bintree<Pregunta> arbol_nuevo){
     arbol = arbol_nuevo;
}

void QuienEsQuien::iniciar_juego(){

     if (arbol.empty()) {
          cerr << "El árbol está vacío. No se puede iniciar el juego." << endl;
          return;
     }

    Ventana v(tg,con,"WisW");
    if (modo_graph){
        v.show();
    }

     // Nodo actual: comenzamos en la raíz
     jugada_actual = arbol.root();

     // Iteramos hasta llegar a un nodo hoja
     while (!jugada_actual.left().null() || !jugada_actual.right().null()) {
          string respuesta;

          // Mostramos la pregunta en la consola o en la interfaz gráfica
          if (modo_graph) {
               con->WriteText("¿" + (*jugada_actual).obtener_pregunta() + "? (Si/No): ");
               respuesta = con->ReadString();
          } else {
               cout << "¿" << (*jugada_actual).obtener_pregunta() << "? (Si/No): ";
               cin >> respuesta;
          }

          // Pasamos la respuesta a minúsculas
          for (int i = 0; i < respuesta.size(); i++) {
               respuesta[i] = tolower(respuesta[i]);
          }

          // Procesamos la respuesta para decidir la siguiente jugada
          if (respuesta == "si")
               jugada_actual = jugada_actual.left();
          else if (respuesta == "no")
               jugada_actual = jugada_actual.right();
          else
               cout << "Por favor, responde con \"Si\" para sí o \"No\" para no." << endl;

          // Mostramos las preguntas formuladas hasta el momento
          cout << preguntas_formuladas(jugada_actual) << endl;

          if (modo_graph){
               // Actualizar el tablero gráfico
               set<string> personajes_activos = informacion_jugada(jugada_actual);
               ocultar_personajes_graph(personajes_activos);
          }
     }

     // Cuando llegamos a un nodo hoja, adivinamos el personaje
     if (!jugada_actual.null()) {
          string personaje = (*jugada_actual).obtener_personaje();
          cout << "¡Ya lo sé! Tu personaje es " << personaje << endl;

          if (modo_graph) {
               con->WriteText("¡Ya lo sé! Tu personaje es " + personaje);
               char c;
               do {
                    con->WriteText("Pulsa 0 para salir");
                    c = con->ReadChar();
               } while (c != '0');
          }
     } else {
          cout << "Ha habido un error, revisa tus respuestas." << endl;
     }

     v.cerrar();
}

set<string> QuienEsQuien::informacion_jugada(bintree<Pregunta>::node jugada_actual){

     set<string> personajes_levantados;

     // Caso base: si el nodo actual es un personaje (hoja), lo añadimos al conjunto
     if ((*jugada_actual).es_personaje()){
          personajes_levantados.insert((*jugada_actual).obtener_personaje());
          return personajes_levantados;
     }

     // Caso recursivo: exploramos las ramas izquierda y derecha del nodo actual
     set<string> personajes_izquierda;
     set<string> personajes_derecha;

     // Exploramos el subárbol izquierdo si no es nulo
     if (!jugada_actual.left().null()) {
          personajes_izquierda = informacion_jugada(jugada_actual.left());
     }

     // Exploramos el subárbol derecho si no es nulo
     if (!jugada_actual.right().null()) {
          personajes_derecha = informacion_jugada(jugada_actual.right());
     }

     // Unimos los resultados de ambos subárboles en el conjunto principal
     personajes_levantados.insert(personajes_izquierda.begin(), personajes_izquierda.end());
     personajes_levantados.insert(personajes_derecha.begin(), personajes_derecha.end());

     return personajes_levantados;
}

void escribir_esquema_arbol(ostream& ss, const bintree <Pregunta>& a,
                            bintree<Pregunta>::node n, string& pre){
     if (n.null())
          ss <<  pre <<  "-- x" <<  endl;
     else {
          ss <<  pre <<  "-- " <<  (*n) <<  endl;
          if ( !n.right().null() || !n.left().null()) {// Si no es una hoja
               pre += "   |";
               escribir_esquema_arbol(ss,a, n.right(), pre);
               pre.replace(pre.size()-4, 4, "    ");
               escribir_esquema_arbol (ss,a, n.left(), pre);
               pre.erase(pre.size()-4, 4);
          }
     }
}

void QuienEsQuien::escribir_arbol_completo() const{
     string pre = "";
     escribir_esquema_arbol(cout,this->arbol,this->arbol.root(),pre);
}

void QuienEsQuien::eliminar_nodos_redundantes_aux(bintree<Pregunta>::node nodo_actual) {
    if (nodo_actual.null()) {
        return; //Caso base
    }

    // Caso recursivo
    if (!nodo_actual.left().null()) {
        eliminar_nodos_redundantes_aux(nodo_actual.left());
    }
    if (!nodo_actual.right().null()) {
        eliminar_nodos_redundantes_aux(nodo_actual.right());
    }

     bintree<Pregunta>::node abuelo = nodo_actual.parent();
     bintree<Pregunta> subarbol_a_cambiar;

     // La raíz no tiene padre al que enganchar el subárbol
     if (abuelo.null()) {
          return;
     }


     // En caso de tener un hijo vacio se corta el arbol del otro hijo que sustituye al padre
    if (nodo_actual.left().null() && !nodo_actual.right().null()) {
         arbol.prune_right(nodo_actual, subarbol_a_cambiar);
         if (nodo_actual == nodo_actual.parent().right())
              arbol.insert_right(abuelo, subarbol_a_cambiar);
         else
              arbol.insert_left(abuelo, subarbol_a_cambiar);

    } else if (!nodo_actual.left().null() && nodo_actual.right().null()) {
         arbol.prune_left(nodo_actual, subarbol_a_cambiar);
         if (nodo_actual == nodo_actual.parent().right())
              arbol.insert_right(abuelo, subarbol_a_cambiar);
         else
              arbol.insert_left(abuelo, subarbol_a_cambiar);
    }
}

void QuienEsQuien::eliminar_nodos_redundantes(){
     eliminar_nodos_redundantes_aux(arbol.root());
}

void QuienEsQuien::calcular_profundidad(bintree<Pregunta>::node n, int profundidad, float &suma_profundidades, float &numero_hojas) {
     if (!n.null()) {
          // Si el nodo es una hoja sumamos su profundidad
          if ((*n).es_personaje()) {
               suma_profundidades += profundidad;
               numero_hojas++;
          } else{
               // Si no es una hoja, seguimos recorriendo a izquierda y derecha, incrementando la profundidad
               calcular_profundidad(n.left(), profundidad + 1, suma_profundidades, numero_hojas);
               calcular_profundidad(n.right(), profundidad + 1, suma_profundidades, numero_hojas);
          }
     }
}

float QuienEsQuien::profundidad_promedio_hojas() {
     float suma_profundidades = 0;
     float numero_hojas = 0;

     calcular_profundidad(arbol.root(), 0, suma_profundidades, numero_hojas);

     // Si no hay hojas, retornamos 0 para evitar división por cero
     if (numero_hojas == 0)
          return 0;

     // Calculamos y devolvemos el promedio de profundidades
     return suma_profundidades / numero_hojas;
}

string QuienEsQuien::preguntas_formuladas(bintree<Pregunta>::node jugada) {
     if (jugada.null()) return "";

     string preguntas = "El personaje oculto tiene las siguientes características:\n";
     bintree<Pregunta>::node actual = jugada;

     // Recorremos desde el nodo actual hasta la raíz
     while (actual != arbol.root()) {
          bintree<Pregunta>::node padre = actual.parent();

          preguntas += "\t";
          preguntas += (*padre).obtener_pregunta();
          preguntas += " - ";

          // Determinamos si el nodo es hijo izquierdo (sí) o derecho (no)
          preguntas += (padre.left() == actual) ? "sí" : "no";
          preguntas += "\n";

          // Subimos al nodo padre
          actual = padre;
     }
     return preguntas;
}

bintree<Pregunta>::node QuienEsQuien::encontrar_nodo(const vector<bool> &caracteristicas) {

     bintree<Pregunta>::node actual = arbol.root(); // Empezamos en la raíz

     while (!(*actual).es_personaje()) {

          // Obtenemos el atributo del nodo actual
          string atributo_actual = (*actual).obtener_pregunta();
          int indice_atributo = indice(atributos, atributo_actual);

          // Avanzamos por la rama correspondiente según la característica del nuevo personaje
          if (caracteristicas[indice_atributo]) {
               actual = actual.left();
          } else {
               actual = actual.right();
          }
     }

     return actual;
}


void QuienEsQuien::aniade_personaje(string nombre, vector<bool> caracteristicas, string nombre_imagen_personaje) {

     bintree<Pregunta>::node personaje = encontrar_nodo(caracteristicas);

     // Si hemos llegado a un nodo hoja, significa que hay un personaje ya existente
     // con esas características en ese punto del árbol.
     if (!personaje.null()) {
          string personaje_existente = (*personaje).obtener_personaje();
          vector<bool> caracteristicas_existente;

          // Buscamos las características del personaje ya existente
          for (int i = 0; i < personajes.size(); ++i) {
               if (personajes[i] == personaje_existente) {
                    caracteristicas_existente = tablero[i];
                    break;
               }
          }

          // Encontramos el primer atributo que distingue a los dos personajes
          int atributo_diferenciador = -1;
          for (int i = 0; i < caracteristicas.size(); ++i) {
               if (caracteristicas[i] != caracteristicas_existente[i]) {
                    atributo_diferenciador = i;
                    break;
               }
          }

          // Si no hay atributo que los diferencie, no podemos añadir el nuevo personaje
          if (atributo_diferenciador == -1) {
               cerr << "Error: No se puede distinguir el personaje nuevo del existente." << endl;
               return;
          }

          // Creamos una nueva pregunta con el atributo diferenciador
          Pregunta nueva_pregunta(atributos[atributo_diferenciador], 2);
          bintree<Pregunta> nuevo_nivel(nueva_pregunta);

          // Creamos las hojas que representarán a los dos personajes
          bintree<Pregunta> nodo_nuevo(Pregunta(nombre, 1));
          bintree<Pregunta> nodo_existente(Pregunta(personaje_existente, 1));

          // Insertamos los personajes en función del atributo diferenciador
          if (caracteristicas[atributo_diferenciador]) {
               nuevo_nivel.insert_left(nuevo_nivel.root(), nodo_nuevo);
               nuevo_nivel.insert_right(nuevo_nivel.root(), nodo_existente);
          } else {
               nuevo_nivel.insert_left(nuevo_nivel.root(), nodo_existente);
               nuevo_nivel.insert_right(nuevo_nivel.root(), nodo_nuevo);
          }

          // Reemplazamos el nodo hoja por el nuevo nivel
          bintree<Pregunta>::node padre = personaje.parent();
          if (padre.left() == personaje) {
               arbol.insert_left(padre, nuevo_nivel);
          } else {
               arbol.insert_right(padre, nuevo_nivel);
          }
     } else {
          // Si no hay hoja (el árbol estaba vacío o se llegó a un null), simplemente insertamos
          // el nuevo personaje debajo del padre
          bintree<Pregunta>::node padre = personaje.parent();
          if (padre.left() == personaje) {
               arbol.insert_left(padre, Pregunta(nombre,1));
          } else {
               arbol.insert_right(padre, Pregunta(nombre,1));
          }
     }


    // Añadimos el nuevo personaje al tablero y lista de personajes
    personajes.push_back(nombre);
    personajes_images.push_back(nombre_imagen_personaje);
    tablero.push_back(caracteristicas);
}

void QuienEsQuien::elimina_personaje(string nombre) {

     // Buscamos el índice del personaje a eliminar
     int indice_personaje = indice(personajes, nombre);

     if (indice_personaje == -1) {
          cerr << "Error: personaje no encontrado. " << endl;
     } else {
          bintree<Pregunta>::node personaje = encontrar_nodo(tablero[indice_personaje]);

          // Tenemos que eliminar este personaje del árbol.
          bintree<Pregunta>::node padre = personaje.parent();
          bintree<Pregunta> subarbol_a_cambiar;

          // Si el personaje es la raíz, era el único: el árbol queda vacío
          if (padre.null()) {
               arbol.clear();
               personajes.erase(personajes.begin() + indice_personaje);
               personajes_images.erase(personajes_images.begin() + indice_personaje);
               tablero.erase(tablero.begin() + indice_personaje);
               return;
          }

          bintree<Pregunta>::node abuelo = padre.parent();

          // Según si la hoja es hijo izquierdo o derecho, "podamos" el lado opuesto
          // del padre para quedarnos con el subárbol que reemplazará al padre.
          if (padre.left() == personaje) {
               arbol.prune_right(padre, subarbol_a_cambiar);
          } else {
               arbol.prune_left(padre, subarbol_a_cambiar);
          }

          // Cambiamos el subarbol que empieza el padre por el que queremos cambiar.
          // Si el padre es la raíz, el subárbol pasa a ser el árbol completo.
          if (abuelo.null()) {
               arbol = subarbol_a_cambiar;
          } else if (padre == abuelo.right()) {
               arbol.insert_right(abuelo, subarbol_a_cambiar);
          } else {
               arbol.insert_left(abuelo, subarbol_a_cambiar);
          }

          // Borramos el personaje del vector de personajes, de imágenes y del tablero
          personajes.erase(personajes.begin() + indice_personaje);
          personajes_images.erase(personajes_images.begin() + indice_personaje);
          tablero.erase(tablero.begin() + indice_personaje);
     }

}

/**
* @brief Genera numero enteros positivos aleatorios en el rango [min,max].
**/
int generaEntero(int min, int max){
     int tam= max-min;
     return ((rand( )%tam)+min) ;
}

void QuienEsQuien::tablero_aleatorio(int numero_de_personajes){
     srand(0);
     this->limpiar();
     float log_2_numero_de_personajes = log(numero_de_personajes)/log(2);
     int numero_de_atributos = ceil(log_2_numero_de_personajes);

     cout <<  "Peticion para generar " <<  numero_de_personajes << " personajes ";
     cout <<  "con " << numero_de_atributos <<  " atributos" << endl;
     cout <<  "Paso 1: generar " <<  pow(2, numero_de_atributos) <<  " personajes" << endl;

     //Fase 1: completar el tablero con todos los personajes posibles
     //Complete el tablero y los nombres de personajes a la vez
     for(int indice_personaje=0;indice_personaje< pow(2,numero_de_atributos);indice_personaje++){
          vector<bool> atributos_personaje =
                                    convertir_a_vector_bool(indice_personaje,numero_de_atributos);
          string nombre_personaje = "Personaje_"+to_string(indice_personaje);

          this->personajes.push_back(nombre_personaje);
          this->tablero.push_back(atributos_personaje);
     }

     // Completo los nombres de los atributos.
     for( int indice_atributo=0;indice_atributo<numero_de_atributos;indice_atributo++){
          string nombre_atributo = "Atributo_"+to_string(indice_atributo);
          this->atributos.push_back(nombre_atributo);
     }
     cout <<  "Paso 2: eliminar " << (pow(2,numero_de_atributos)-numero_de_personajes) <<  "personajes" << endl;

     //Fase 2. Borrar personajes aleatoriamente hasta que quedan solo los que hemos pedido.
     while(personajes.size()>numero_de_personajes){
          int personaje_a_eliminar = generaEntero(0,personajes.size());
          personajes.erase(personajes.begin()+personaje_a_eliminar);
          tablero.erase(tablero.begin()+personaje_a_eliminar);
     }
}
void QuienEsQuien::ocultar_personajes_graph(const set<string> &personajes_activos){
    //ocultamos los personajes
    int idx=0;
    int ncols=tg->getNcols();
    for (auto it=personajes.begin();it!=personajes.end();++it,idx++){
        if ( personajes_activos.find(*it)==personajes_activos.end())
            tg->putImagen(idx/ncols,idx%ncols,imagen_ocultar.c_str());
    }
}


void QuienEsQuien::setImagenOcultar(const char * n){
    imagen_ocultar=n;
}

void QuienEsQuien::setModoGraph(bool m){
    modo_graph=m;
}


