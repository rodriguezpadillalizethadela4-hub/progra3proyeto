
// =================================================================================
// PROYECTO: Sistema de Gestión de Gimnasio
// CURSO: Programación 3
// DESCRIPCIÓN: Sistema para gestionar miembros, clases y asistencias
// ESTRUCTURA: Simulación de archivos separados (.h, .cpp) en un solo fichero
// =================================================================================

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>

// =================================================================================
// ARCHIVO: ClaseGym.h
// RESPONSABILIDAD: Representa una clase/entrenamiento del gimnasio
// =================================================================================

class ClaseGym {
private:
    std::string nombreClase;
    std::string instructor;
    std::string horario;
    std::string codigoClase;
    int capacidadMaxima;
    int inscritos;
    std::vector<int> idsMiembrosInscritos;

public:
    ClaseGym(const std::string& nombre, const std::string& instructor,
             const std::string& horario, const std::string& codigo, int capacidad);

    // Getters
    std::string getNombre() const { return nombreClase; }
    std::string getInstructor() const { return instructor; }
    std::string getHorario() const { return horario; }
    std::string getCodigo() const { return codigoClase; }
    int getCapacidadMaxima() const { return capacidadMaxima; }
    int getInscritos() const { return inscritos; }
    bool tieneCupo() const { return inscritos < capacidadMaxima; }

    // Métodos de operación
    bool inscribirMiembro(int idMiembro);
    bool cancelarInscripcion(int idMiembro);
    bool estaMiembroInscrito(int idMiembro) const;
    void mostrarInformacion() const;
};

// =================================================================================
// ARCHIVO: Miembro.h
// RESPONSABILIDAD: Representa un miembro del gimnasio
// =================================================================================

class Miembro {
private:
    std::string nombre;
    int idMiembro;
    std::string tipoMembresia;
    bool membresiaActiva;
    int diasAsistencia;
    std::vector<std::string> clasesInscritas;

public:
    Miembro(const std::string& nombre, int idMiembro, const std::string& tipoMembresia);

    // Getters
    std::string getNombre() const { return nombre; }
    int getIdMiembro() const { return idMiembro; }
    std::string getTipoMembresia() const { return tipoMembresia; }
    bool estaActivo() const { return membresiaActiva; }
    int getDiasAsistencia() const { return diasAsistencia; }

    // Setters
    void setMembresiaActiva(bool estado) { membresiaActiva = estado; }
    void setTipoMembresia(const std::string& tipo) { tipoMembresia = tipo; }

    // Métodos de operación
    void registrarAsistencia();
    void inscribirseAClase(const std::string& codigoClase);
    void cancelarClase(const std::string& codigoClase);
    int getCantidadClasesInscritas() const { return clasesInscritas.size(); }
    void mostrarInformacion() const;
};

// =================================================================================
// ARCHIVO: Gimnasio.h
// RESPONSABILIDAD: Orquesta todas las operaciones del gimnasio
// =================================================================================

class Gimnasio {
private:
    std::string nombre;
    std::vector<Miembro> miembros;
    std::vector<ClaseGym> clases;
    int totalAsistenciasHoy;

    // Métodos auxiliares privados
    Miembro* buscarMiembroPorId(int idMiembro);
    ClaseGym* buscarClasePorCodigo(const std::string& codigo);

public:
    Gimnasio(const std::string& nombre);

    // Gestión de miembros
    void registrarMiembro(const std::string& nombre, int idMiembro, 
                         const std::string& tipoMembresia);
    void mostrarMiembros() const;
    void suspenderMembresia(int idMiembro);
    void reactivarMembresia(int idMiembro);

    // Gestión de clases
    void crearClase(const std::string& nombre, const std::string& instructor,
                   const std::string& horario, const std::string& codigo, 
                   int capacidad);
    void mostrarHorarioClases() const;

    // Operaciones principales
    void registrarAsistencia(int idMiembro);
    void inscribirMiembroAClase(int idMiembro, const std::string& codigoClase);
    void cancelarInscripcionClase(int idMiembro, const std::string& codigoClase);

    // Reportes
    void mostrarResumenDiario() const;

    // Persistencia de datos
    void guardarDatos(const std::string& archivo);
    void cargarDatos(const std::string& archivo);
};

// =================================================================================
// ARCHIVO: ClaseGym.cpp
// =================================================================================

ClaseGym::ClaseGym(const std::string& nombre, const std::string& instructor,
                   const std::string& horario, const std::string& codigo, int capacidad)
    : nombreClase(nombre), instructor(instructor), horario(horario),
      codigoClase(codigo), capacidadMaxima(capacidad), inscritos(0) {}

bool ClaseGym::inscribirMiembro(int idMiembro) {
    if (!tieneCupo()) {
        return false;
    }
    
    auto it = std::find(idsMiembrosInscritos.begin(), 
                       idsMiembrosInscritos.end(), idMiembro);
    if (it != idsMiembrosInscritos.end()) {
        return false;
    }
    
    idsMiembrosInscritos.push_back(idMiembro);
    inscritos++;
    return true;
}

bool ClaseGym::cancelarInscripcion(int idMiembro) {
    auto it = std::find(idsMiembrosInscritos.begin(), 
                       idsMiembrosInscritos.end(), idMiembro);
    if (it != idsMiembrosInscritos.end()) {
        idsMiembrosInscritos.erase(it);
        inscritos--;
        return true;
    }
    return false;
}

bool ClaseGym::estaMiembroInscrito(int idMiembro) const {
    return std::find(idsMiembrosInscritos.begin(), 
                    idsMiembrosInscritos.end(), 
                    idMiembro) != idsMiembrosInscritos.end();
}

void ClaseGym::mostrarInformacion() const {
    std::cout << " - Codigo: " << codigoClase << "\n"
              << "   Clase: " << nombreClase << "\n"
              << "   Instructor: " << instructor << "\n"
              << "   Horario: " << horario << "\n"
              << "   Cupos: " << inscritos << "/" << capacidadMaxima
              << " (" << (tieneCupo() ? "Disponible" : "LLENO") << ")\n";
}

// =================================================================================
// ARCHIVO: Miembro.cpp
// =================================================================================

Miembro::Miembro(const std::string& nombre, int idMiembro, const std::string& tipoMembresia)
    : nombre(nombre), idMiembro(idMiembro), tipoMembresia(tipoMembresia),
      membresiaActiva(true), diasAsistencia(0) {}

void Miembro::registrarAsistencia() {
    diasAsistencia++;
}

void Miembro::inscribirseAClase(const std::string& codigoClase) {
    clasesInscritas.push_back(codigoClase);
}

void Miembro::cancelarClase(const std::string& codigoClase) {
    auto it = std::find(clasesInscritas.begin(), clasesInscritas.end(), codigoClase);
    if (it != clasesInscritas.end()) {
        clasesInscritas.erase(it);
    }
}

void Miembro::mostrarInformacion() const {
    std::cout << " - ID: " << idMiembro << "\n"
              << "   Nombre: " << nombre << "\n"
              << "   Membresia: " << tipoMembresia << "\n"
              << "   Estado: " << (membresiaActiva ? "ACTIVA" : "INACTIVA") << "\n"
              << "   Asistencias: " << diasAsistencia << " dias\n"
              << "   Clases inscritas: " << clasesInscritas.size() << "\n";
}

// =================================================================================
// ARCHIVO: Gimnasio.cpp
// =================================================================================

Gimnasio::Gimnasio(const std::string& nombre) 
    : nombre(nombre), totalAsistenciasHoy(0) {
    std::cout << "\n********************************************\n"
              << "*  Bienvenido a " << nombre << "  *\n"
              << "********************************************\n" << std::endl;
}

Miembro* Gimnasio::buscarMiembroPorId(int idMiembro) {
    for (auto& miembro : miembros) {
        if (miembro.getIdMiembro() == idMiembro) {
            return &miembro;
        }
    }
    return nullptr;
}

ClaseGym* Gimnasio::buscarClasePorCodigo(const std::string& codigo) {
    for (auto& clase : clases) {
        if (clase.getCodigo() == codigo) {
            return &clase;
        }
    }
    return nullptr;
}

void Gimnasio::registrarMiembro(const std::string& nombre, int idMiembro, 
                                const std::string& tipoMembresia) {
    if (buscarMiembroPorId(idMiembro) != nullptr) {
        std::cout << "ERROR: Ya existe un miembro con ID " << idMiembro << "\n";
        return;
    }

    miembros.emplace_back(nombre, idMiembro, tipoMembresia);
    std::cout << "INFO: Miembro '" << nombre << "' registrado exitosamente (ID: " 
              << idMiembro << ")\n";
}

void Gimnasio::mostrarMiembros() const {
    std::cout << "\n====== MIEMBROS REGISTRADOS ======\n";
    if (miembros.empty()) {
        std::cout << "No hay miembros registrados.\n";
    } else {
        std::cout << "Total de miembros: " << miembros.size() << "\n\n";
        for (const auto& miembro : miembros) {
            miembro.mostrarInformacion();
            std::cout << "\n";
        }
    }
    std::cout << "==================================\n";
}

void Gimnasio::crearClase(const std::string& nombre, const std::string& instructor,
                         const std::string& horario, const std::string& codigo, 
                         int capacidad) {
    if (buscarClasePorCodigo(codigo) != nullptr) {
        std::cout << "ERROR: Ya existe una clase con codigo " << codigo << "\n";
        return;
    }

    clases.emplace_back(nombre, instructor, horario, codigo, capacidad);
    std::cout << "INFO: Clase '" << nombre << "' creada exitosamente (Codigo: " 
              << codigo << ")\n";
}

void Gimnasio::mostrarHorarioClases() const {
    std::cout << "\n======= HORARIO DE CLASES =======\n";
    if (clases.empty()) {
        std::cout << "No hay clases programadas.\n";
    } else {
        for (const auto& clase : clases) {
            clase.mostrarInformacion();
            std::cout << "\n";
        }
    }
    std::cout << "=================================\n";
}

void Gimnasio::registrarAsistencia(int idMiembro) {
    Miembro* miembro = buscarMiembroPorId(idMiembro);

    if (miembro == nullptr) {
        std::cout << "ERROR: Miembro con ID " << idMiembro << " no encontrado.\n";
        return;
    }

    if (!miembro->estaActivo()) {
        std::cout << "ERROR: La membresia de " << miembro->getNombre() 
                  << " esta INACTIVA.\n";
        return;
    }

    miembro->registrarAsistencia();
    totalAsistenciasHoy++;
    std::cout << "EXITO: Asistencia registrada para " << miembro->getNombre() 
              << " (Total: " << miembro->getDiasAsistencia() << " dias)\n";
}

void Gimnasio::inscribirMiembroAClase(int idMiembro, const std::string& codigoClase) {
    Miembro* miembro = buscarMiembroPorId(idMiembro);
    ClaseGym* clase = buscarClasePorCodigo(codigoClase);

    if (miembro == nullptr) {
        std::cout << "ERROR: Miembro con ID " << idMiembro << " no encontrado.\n";
        return;
    }

    if (clase == nullptr) {
        std::cout << "ERROR: Clase con codigo " << codigoClase << " no encontrada.\n";
        return;
    }

    if (!miembro->estaActivo()) {
        std::cout << "ERROR: Membresia inactiva.\n";
        return;
    }

    if (!clase->tieneCupo()) {
        std::cout << "ERROR: La clase '" << clase->getNombre() << "' esta LLENA.\n";
        return;
    }

    if (clase->estaMiembroInscrito(idMiembro)) {
        std::cout << "ERROR: El miembro ya esta inscrito en esta clase.\n";
        return;
    }

    int limiteClases = 0;
    if (miembro->getTipoMembresia() == "Basica") limiteClases = 2;
    else if (miembro->getTipoMembresia() == "Premium") limiteClases = 5;
    else if (miembro->getTipoMembresia() == "VIP") limiteClases = 999;

    if (miembro->getCantidadClasesInscritas() >= limiteClases) {
        std::cout << "ERROR: Limite de clases alcanzado para membresia "
                  << miembro->getTipoMembresia() << " (" << limiteClases << " clases).\n";
        return;
    }

    clase->inscribirMiembro(idMiembro);
    miembro->inscribirseAClase(codigoClase);
    
    std::cout << "EXITO: " << miembro->getNombre() << " inscrito en '" 
              << clase->getNombre() << "'\n";
}

void Gimnasio::cancelarInscripcionClase(int idMiembro, const std::string& codigoClase) {
    Miembro* miembro = buscarMiembroPorId(idMiembro);
    ClaseGym* clase = buscarClasePorCodigo(codigoClase);

    if (miembro == nullptr || clase == nullptr) {
        std::cout << "ERROR: Miembro o clase no encontrado.\n";
        return;
    }

    if (!clase->estaMiembroInscrito(idMiembro)) {
        std::cout << "ERROR: El miembro no esta inscrito en esta clase.\n";
        return;
    }

    clase->cancelarInscripcion(idMiembro);
    miembro->cancelarClase(codigoClase);

    std::cout << "EXITO: Inscripcion cancelada exitosamente.\n";
}

void Gimnasio::suspenderMembresia(int idMiembro) {
    Miembro* miembro = buscarMiembroPorId(idMiembro);
    if (miembro != nullptr) {
        miembro->setMembresiaActiva(false);
        std::cout << "INFO: Membresia de " << miembro->getNombre() << " suspendida.\n";
    } else {
        std::cout << "ERROR: Miembro no encontrado.\n";
    }
}

void Gimnasio::reactivarMembresia(int idMiembro) {
    Miembro* miembro = buscarMiembroPorId(idMiembro);
    if (miembro != nullptr) {
        miembro->setMembresiaActiva(true);
        std::cout << "INFO: Membresia de " << miembro->getNombre() << " reactivada.\n";
    } else {
        std::cout << "ERROR: Miembro no encontrado.\n";
    }
}

void Gimnasio::mostrarResumenDiario() const {
    std::cout << "\n********** RESUMEN DEL DIA **********\n"
              << "Gimnasio: " << nombre << "\n"
              << "Total de miembros: " << miembros.size() << "\n"
              << "Asistencias registradas hoy: " << totalAsistenciasHoy << "\n"
              << "Clases programadas: " << clases.size() << "\n";
    
    int cuposOcupados = 0, cuposTotales = 0;
    for (const auto& clase : clases) {
        cuposOcupados += clase.getInscritos();
        cuposTotales += clase.getCapacidadMaxima();
    }
    
    std::cout << "Cupos ocupados: " << cuposOcupados << "/" << cuposTotales << "\n"
              << "************************************\n";
}

// =================================================================================
// PERSISTENCIA DE DATOS
// =================================================================================

void Gimnasio::guardarDatos(const std::string& archivo) {
    std::ofstream file(archivo);
    
    if (!file.is_open()) {
        std::cout << "ERROR: No se pudo crear el archivo " << archivo << "\n";
        return;
    }

    // Guardar miembros
    file << miembros.size() << "\n";
    for (const auto& m : miembros) {
        file << m.getNombre() << "|" 
             << m.getIdMiembro() << "|"
             << m.getTipoMembresia() << "|"
             << m.estaActivo() << "|"
             << m.getDiasAsistencia() << "\n";
    }

    // Guardar clases
    file << clases.size() << "\n";
    for (const auto& c : clases) {
        file << c.getNombre() << "|"
             << c.getInstructor() << "|"
             << c.getHorario() << "|"
             << c.getCodigo() << "|"
             << c.getCapacidadMaxima() << "\n";
    }

    file.close();
    std::cout << "\n✅ Datos guardados exitosamente en '" << archivo << "'\n";
}

void Gimnasio::cargarDatos(const std::string& archivo) {
    std::ifstream file(archivo);
    
    if (!file.is_open()) {
        std::cout << "⚠️  No se encontro archivo de datos previos. Iniciando desde cero.\n";
        return;
    }

    miembros.clear();
    clases.clear();

    // Cargar miembros
    int numMiembros;
    file >> numMiembros;
    file.ignore();

    for (int i = 0; i < numMiembros; i++) {
        std::string linea;
        std::getline(file, linea);
        std::stringstream ss(linea);
        
        std::string nombre, tipo;
        int id, asistencias;
        bool activo;
        char delimiter;

        std::getline(ss, nombre, '|');
        ss >> id >> delimiter;
        std::getline(ss, tipo, '|');
        ss >> activo >> delimiter >> asistencias;

        Miembro m(nombre, id, tipo);
        m.setMembresiaActiva(activo);
        for (int j = 0; j < asistencias; j++) {
            m.registrarAsistencia();
        }
        miembros.push_back(m);
    }

    // Cargar clases
    int numClases;
    file >> numClases;
    file.ignore();

    for (int i = 0; i < numClases; i++) {
        std::string linea;
        std::getline(file, linea);
        std::stringstream ss(linea);
        
        std::string nombre, instructor, horario, codigo;
        int capacidad;
        char delimiter;

        std::getline(ss, nombre, '|');
        std::getline(ss, instructor, '|');
        std::getline(ss, horario, '|');
        std::getline(ss, codigo, '|');
        ss >> capacidad;

        clases.emplace_back(nombre, instructor, horario, codigo, capacidad);
    }

    file.close();
    std::cout << "✅ Datos cargados: " << miembros.size() << " miembros, " 
              << clases.size() << " clases\n";
}

// =================================================================================
// ARCHIVO: main.cpp
// PUNTO DE ENTRADA CON MENÚ INTERACTIVO
// =================================================================================

void mostrarMenu() {
    std::cout << "\n╔════════════════════════════════════╗\n";
    std::cout << "║   SISTEMA DE GESTION DE GIMNASIO  ║\n";
    std::cout << "╚════════════════════════════════════╝\n";
    std::cout << " 1. Registrar Miembro\n";
    std::cout << " 2. Crear Clase\n";
    std::cout << " 3. Registrar Asistencia\n";
    std::cout << " 4. Inscribir a Clase\n";
    std::cout << " 5. Cancelar Inscripcion\n";
    std::cout << " 6. Suspender Membresia\n";
    std::cout << " 7. Reactivar Membresia\n";
    std::cout << " 8. Mostrar Miembros\n";
    std::cout << " 9. Mostrar Clases\n";
    std::cout << "10. Resumen Diario\n";
    std::cout << "11. Guardar Datos\n";
    std::cout << " 0. Salir\n";
    std::cout << "────────────────────────────────────\n";
    std::cout << "Seleccione una opcion: ";
}

void limpiarBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    Gimnasio fitPro("FitPro Gym & Wellness");
    
    // Intentar cargar datos previos
    fitPro.cargarDatos("gimnasio_datos.txt");
    
    int opcion;
    
    do {
        mostrarMenu();
        std::cin >> opcion;
        
        if (std::cin.fail()) {
            limpiarBuffer();
            std::cout << "\n❌ Opcion invalida. Intente de nuevo.\n";
            continue;
        }
        
        limpiarBuffer();
        
        switch(opcion) {
            case 1: { // Registrar Miembro
                std::string nombre, tipo;
                int id;
                
                std::cout << "\n--- REGISTRAR MIEMBRO ---\n";
                std::cout << "Nombre completo: ";
                std::getline(std::cin, nombre);
                std::cout << "ID (numero): ";
                std::cin >> id;
                limpiarBuffer();
                std::cout << "Tipo (Basica/Premium/VIP): ";
                std::getline(std::cin, tipo);
                
                fitPro.registrarMiembro(nombre, id, tipo);
                break;
            }
            
            case 2: { // Crear Clase
                std::string nombre, instructor, horario, codigo;
                int capacidad;
                
                std::cout << "\n--- CREAR CLASE ---\n";
                std::cout << "Nombre de la clase: ";
                std::getline(std::cin, nombre);
                std::cout << "Instructor: ";
                std::getline(std::cin, instructor);
                std::cout << "Horario: ";
                std::getline(std::cin, horario);
                std::cout << "Codigo (ej: YG001): ";
                std::getline(std::cin, codigo);
                std::cout << "Capacidad maxima: ";
                std::cin >> capacidad;
                limpiarBuffer();
                
                fitPro.crearClase(nombre, instructor, horario, codigo, capacidad);
                break;
            }
            
            case 3: { // Registrar Asistencia
                int id;
                std::cout << "\n--- REGISTRAR ASISTENCIA ---\n";
                std::cout << "ID del miembro: ";
                std::cin >> id;
                limpiarBuffer();
                
                fitPro.registrarAsistencia(id);
                break;
            }
            
            case 4: { // Inscribir a Clase
                int id;
                std::string codigo;
                
                std::cout << "\n--- INSCRIBIR A CLASE ---\n";
                std::cout << "ID del miembro: ";
                std::cin >> id;
                limpiarBuffer();
                std::cout << "Codigo de la clase: ";
                std::getline(std::cin, codigo);
                
                fitPro.inscribirMiembroAClase(id, codigo);
                break;
            }
            
            case 5: { // Cancelar Inscripción
                int id;
                std::string codigo;
                
                std::cout << "\n--- CANCELAR INSCRIPCION ---\n";
                std::cout << "ID del miembro: ";
                std::cin >> id;
                limpiarBuffer();
                std::cout << "Codigo de la clase: ";
                std::getline(std::cin, codigo);
                
                fitPro.cancelarInscripcionClase(id, codigo);
                break;
            }
            
            case 6: { // Suspender Membresía
                int id;
                std::cout << "\n--- SUSPENDER MEMBRESIA ---\n";
                std::cout << "ID del miembro: ";
                std::cin >> id;
                limpiarBuffer();
                
                fitPro.suspenderMembresia(id);
                break;
            }
            
            case 7: { // Reactivar Membresía
                int id;
                std::cout << "\n--- REACTIVAR MEMBRESIA ---\n";
                std::cout << "ID del miembro: ";
                std::cin >> id;
                limpiarBuffer();
                
                fitPro.reactivarMembresia(id);
                break;
            }
            
            case 8: { // Mostrar Miembros
                fitPro.mostrarMiembros();
                break;
            }
            
            case 9: { // Mostrar Clases
                fitPro.mostrarHorarioClases();
                break;
            }
            
            case 10: { // Resumen Diario
                fitPro.mostrarResumenDiario();
                break;
            }
            
            case 11: { // Guardar Datos
                fitPro.guardarDatos("gimnasio_datos.txt");
                break;
            }
            
            case 0: { // Salir
                std::cout << "\n¿Desea guardar los datos antes de salir? (s/n): ";
                char respuesta;
                std::cin >> respuesta;
                if (respuesta == 's' || respuesta == 'S') {
                    fitPro.guardarDatos("gimnasio_datos.txt");
                }
                std::cout << "\n*** Gracias por usar el Sistema de FitPro Gym ***\n";
                break;
            }
            
            default:
                std::cout << "\n❌ Opcion invalida. Intente de nuevo.\n";
        }
        
    } while(opcion != 0);
    
    return 0;
}

// =================================================================================
// MEJORAS IMPLEMENTADAS:
// =================================================================================
// ✅ Menú interactivo completo
// ✅ Persistencia de datos (guardar/cargar en archivo .txt)
// ✅ Validación de IDs y códigos duplicados
// ✅ Manejo robusto de errores de entrada
// ✅ Estructura modular simulando archivos separados
// ✅ Funcionalidades esenciales sin sobrecarga
// =================================================================================

