
# Manual del proyecto

El grupo de 4 integrantes se dividió el trabajo bajo los siguientes roles:
- Facundo Madotta: Elaboración de lobby de cliente y editor de niveles con Qt
- Manuel Peñalva: Elaboración del diseño del videojuego en cliente mediante dibujado con SDL
- Fabricio Batastini: Elaboración del protocolo de comunicación, sistema de hilos y comunicación del lobby del servidor
- Juan Pablo Pessat: Diseño de la logica del juego de parte del servidor.

## Herramientas

A la hora de chequear el codigo implementado se utilizaron los 3 pre commit hooks de verificacion utilizados en trabajos practicos anteriores, CppLint, Clang Format y CppCheck, los cuales permitieron mantener algunas buenas prácticas y formato en el código.
Sumado a esto, se utilizo Cmake para la compilacion del programa durante todo el proceso de desarrollo, basandonos en la plantilla de ejemplo que fue brindada por la cátedra

## Documentación
Además de los temas dictados en clase, se utlizaron algunas documentaciones con el objetivo de complementar los conocimientos, sobre todo de las tecnologias de dibujado.

Por el lado de Box2d, se utilizó la documentación presente en su pagina oficial, para comprender algunos aspectos como las físicas, shapes, sensores o ray cast.
https://box2d.org/documentation/index.html

En el caso de SDL, se vieron algunos de los ejemplos presentes en el repositorio mostrado por la catedra.

Por ultimo, para la comprension de Qt, se recurrió a la documentación presente en https://doc.qt.io/qt-6, sumado al uso de la aplicación QtCreator para el diseño de las interfaces.


## Implementación
Durante el proceso de implementación del trabajo práctico, se buscó que cada semana se cuente con un entregable o nueva funcionalidad. Esta distribución se dio de la siguiente manera.
1. Sincronización cliente-servidor y movimiento básico de los autos
2. Implementación de multiples partidas en paralelo
3. Funcionamiento de carrera con checkpoints y hints junto con colisiones
4. Asignación de diversos autos segun la elección del jugador y fase de mejoras
5. Implementación de posiciones, NPCs y fisicas de puentes

## Analisis del trabajo
Al día de la fecha de la primera entrega obligatoria, se llegó con todos los requisitos presentes en el enunciado. Sin embargo, hubieron ciertos aspectos problemáticos, como la implementación de los NPCs, donde hubo una puesta en común con los correctores y nos inclinamos a mapear las esquinas de los mapas para asignarles un recorrido. Por otro lado, las fisicas de los puentes no son totalmente exactas, lo que dificulta el paso de los autos, sobre todo los mas grandes

Si bien, se logró implementar todos los requerimientos, la distribucion del trabajo a lo largo de las semanas resultó disparejo, ya que gran parte del trabajo fue realizado durante las ultimas dos semanas, requiriendo de mucho tiempo.
