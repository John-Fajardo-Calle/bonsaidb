#ifndef BONSAIDB_RECORD_HPP
#define BONSAIDB_RECORD_HPP

#include <string>
#include <vector>
#include <cstdint> // Para tipos de enteros de tamaño fijo como int32_t

// Definición de la estructura de un registro.
// Basado en los comandos del CLI, contendrá id, nombre, edad y saldo. 
struct Record {
    // --- Miembros de Datos ---
    int32_t id;
    char name[50]; // Usamos un array de tamaño fijo para simplificar la serialización.
    int32_t age;
    double balance;

    // --- Métodos de Serialización ---

    /**
     * @brief Serializa el objeto Record a un stream de bytes.
     * @param buffer El vector de bytes donde se escribirá el registro.
     */
    void serialize(std::vector<char>& buffer) const;

    /**
     * @brief Deserializa un registro desde un stream de bytes.
     * @param buffer El vector de bytes desde donde se leerá el registro.
     * @param offset El punto de inicio en el buffer desde donde leer.
     */
    void deserialize(const std::vector<char>& buffer, size_t& offset);

    /**
     * @brief Devuelve el tamaño total del registro en bytes una vez serializado.
     * @return El tamaño fijo del registro.
     */
    static constexpr size_t getRecordSize() {
        return sizeof(id) + sizeof(name) + sizeof(age) + sizeof(balance);
    }
};

#endif //BONSAIDB_RECORD_HPP