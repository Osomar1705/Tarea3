<div align="center">

# 🎮 Command Dispatcher

**A tiny command system: text commands are mapped to `std::function` handlers that drive a game entity.**
*Sistema de comandos: comandos de texto mapeados a handlers `std::function` que controlan una entidad de juego.*

![C++](https://img.shields.io/badge/C++-00599C?style=flat-square&logo=cplusplus&logoColor=white)

</div>

---

## 🌟 The idea

An `Entity` holds position, HP and a name. Instead of a `switch` over command strings, the dispatcher keeps a `map<string, Command>` where `Command = function<void(const list<string>&)>`.

Adding a new verb means registering one more entry in the map — no existing code changes. That's the **Command pattern** expressed with modern C++ callables, plus exception handling for malformed input.

---

## 🚀 Build & run

```bash
g++ -std=c++17 -o tarea3 "OneDrive/Documents/Progra_3/semana4/Tarea/main.cpp"
./tarea3
```

---

## 🇪🇸 En español

Una `Entity` guarda posición, HP y nombre. En lugar de un `switch` sobre cadenas, el despachador mantiene un `map<string, Command>` donde `Command = function<void(const list<string>&)>`.

Agregar un verbo nuevo es registrar una entrada más en el mapa, sin tocar el código existente: es el **patrón Command** expresado con callables modernos de C++, más manejo de excepciones para entradas mal formadas.

> 📌 Trabajo del curso **Programación III (UTEC), semana 4**. Repositorio archivado como referencia.

---

<div align="center">

[**Osmar Vilchez Aguirre**](https://github.com/Osomar1705) · UTEC 🇵🇪

</div>
