#include <iostream>
#include <string>
#include <list>
#include <map>
#include <functional>
#include <exception>

using namespace std;
using Command = function<void(const list<string>&)>;

//Entity
class Entity {
    int x = 0;
    int y = 0;
    float hp = 100.0f;
    string name = "Player1";

public:
    void move(int dx, int dy) {
        x += dx;
        y += dy;
        cout << "Entity se movio a (" << x << ", " << y << ")\n";
    }

    void heal(float amount) { //void
        hp += amount;
        if (hp > 100.0f) hp = 100.0f;
        cout << "Entity curada. HP actual: " << hp << "\n";
    }

    void damage(float amount) { //void
        hp -= amount;
        if (hp < 0.0f) hp = 0.0f;
        cout << "Entity recibio danio. HP actual: " << hp << "\n";
    }

    void reset() {
        x = 0; y = 0; hp = 100.0f;
        cout << "Entity reiniciada.\n";
    }

    string getStatusString() const {
        return "Pos: (" + to_string(x) + ", " + to_string(y) + ") | HP: " + to_string(hp);
    }

    void status() const {
        cout << "[STATUS] " << getStatusString() << "\n";
    }
};

//funcion free
void freeFunctionReset(Entity& e, const list<string>& args) {
    if (!args.empty()) {
        cout << "Error: no recibe argumentos.\n";
        return;
    }
    e.reset();
}


//Functor
class DamageFunctor {
private:
    Entity& entity;
    int useCount = 0;

public:
    DamageFunctor(Entity& e) : entity(e) {}

    void operator()(const list<string>& args) {
        if (args.empty()) {
            cout << "Faltan argumentos. Uso: damage <cantidad>\n";
            return;
        }
        try {
            float dmg = stof(args.front());
            useCount++;
            cout << "[DamageFunctor - Uso #" << useCount << "] ";
            entity.damage(dmg);
        } catch (const exception& e) {
            cout << "Error: Argumento invalido para 'damage'. Debe ser un numero.\n";
        }
    }
};

//Comandos centrales
class CommandCenter {
private:
    map<string, Command> commands;
    map<string, list<pair<string, list<string>>>> macros;
    list<string> history;
    Entity& sharedEntity;

public:
    CommandCenter(Entity& e) : sharedEntity(e) {}

    void registerCommand(const string& name, Command cmd) {
        commands[name] = cmd;
    }

    void removeCommand(const string& name) {
        map<string, Command>::iterator it = commands.find(name);
        if (it != commands.end()) {
            commands.erase(it);
            cout << "-> Comando '" << name << "' eliminado exitosamente.\n";
        } else {
            cout << "-> Error: No se puede eliminar. Comando '" << name << "' no existe.\n";
        }
    }

    bool execute(const string& name, const list<string>& args) {
        map<string, Command>::iterator it = commands.find(name);
        if (it != commands.end()) {
            string stateBefore = sharedEntity.getStatusString();

            it->second(args);

            string stateAfter = sharedEntity.getStatusString();

            string historyEntry = "Cmd: " + name + " | Antes: [" + stateBefore + "] -> Despues: [" + stateAfter + "]";
            history.push_back(historyEntry);
            return true;
        } else {
            cout << "Error: Comando '" << name << "' no encontrado en el CommandCenter.\n";
            return false;
        }
    }

    void registerMacro(const string& name, const list<pair<string, list<string>>>& steps) {
        macros[name] = steps;
        cout << "-> Macro '" << name << "' registrada exitosamente.\n";
    }

    void executeMacro(const string& name) {
        map<string, list<pair<string, list<string>>>>::iterator macroIt = macros.find(name);

        if (macroIt != macros.end()) {
            cout << "\n=== Ejecutando Macro: " << name << " ===\n";

            list<pair<string, list<string>>>::iterator stepIt;
            for (stepIt = macroIt->second.begin(); stepIt != macroIt->second.end(); ++stepIt) {
                const string& cmdName = stepIt->first;
                const list<string>& cmdArgs = stepIt->second;

                cout << " Paso Macro -> Ejecutando: " << cmdName << "\n";
                bool success = execute(cmdName, cmdArgs);

                if (!success) {
                    cout << "Error critico en macro: Ejecucion de '" << name << "' detenida.\n";
                    break;
                }
            }
            cout << "====================================\n\n";
        } else {
            cout << "Error: Macro '" << name << "' no encontrada.\n";
        }
    }

    void showHistory() {
        cout << "\n--- Historial de Ejecucion ---\n";
        list<string>::iterator it;
        for (it = history.begin(); it != history.end(); ++it) {
            cout << *it << "\n";
        }
        cout << "------------------------------\n";
    }
};

// Demostracion en main()
int main() {
    cout << "Iniciando simulacion CommandCenter++\n\n";

    Entity player;
    CommandCenter center(player);

    center.registerCommand("reset", bind(freeFunctionReset, ref(player), placeholders::_1));

    center.registerCommand("move", [&player](const list<string>& args) {
        if (args.size() != 2) {
            cout << "Error: 'move' requiere 2 argumentos (x y).\n";
            return;
        }
        try {
            list<string>::const_iterator it = args.begin();
            int dx = stoi(*it);
            ++it;
            int dy = stoi(*it);
            player.move(dx, dy);
        } catch (const exception& e) {
            cout << "Error: Argumentos invalidos para 'move'. Deben ser enteros.\n";
        }
    });

    center.registerCommand("status", [&player](const list<string>& args) {
        player.status();
    });

    center.registerCommand("heal", [&player](const list<string>& args) {
        if (!args.empty()) {
            try {
                float amount = stof(args.front());
                player.heal(amount);
            } catch (...) {
                cout << "Error en 'heal'.\n";
            }
        }
    });

    DamageFunctor dmgCmd(player);
    center.registerCommand("damage", dmgCmd);

    cout << "--- PRUEBAS DE COMANDOS SIMPLES ---\n";
    center.execute("status", {});
    center.execute("move", {"10", "15"});
    center.execute("damage", {"40.5"});
    center.execute("move", {"-5"});
    center.execute("volar", {"100"});

    list<pair<string, list<string>>> macro1 = {
        {"heal", {"20"}},
        {"status", {}}
    };
    center.registerMacro("curarse_y_verificar", macro1);

    list<pair<string, list<string>>> macro2 = {
        {"move", {"5", "5"}},
        {"damage", {"15"}},
        {"move", {"-5", "-5"}}
    };
    center.registerMacro("ataque_combinado", macro2);

    list<pair<string, list<string>>> macro3 = {
        {"reset", {}},
        {"comando_falso", {"10"}},
        {"status", {}}
    };
    center.registerMacro("macro_con_error", macro3);

    center.executeMacro("curarse_y_verificar");
    center.executeMacro("ataque_combinado");
    center.executeMacro("macro_con_error");

    cout << "\n--- PRUEBA DE ELIMINACION ---\n";
    center.removeCommand("damage");
    center.execute("damage", {"10"});

    center.showHistory();

    cout << "--- ESTADO FINAL DEL SISTEMA ---\n";
    player.status();

    return 0;
}