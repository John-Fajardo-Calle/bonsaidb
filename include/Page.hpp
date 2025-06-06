#ifndef BONSAIDB_PAGE_HPP
#define BONSAIDB_PAGE_HPP

#include "Record.hpp"
#include <vector>
#include <cstdint>

// Define un tamaño de página estándar. 4KB (4096 bytes) es un valor común.
constexpr size_t PAGE_SIZE = 4096;

/**
 * @class Page
 * @brief Representa una página de datos en disco.
 *
 * Una página es una unidad de almacenamiento de tamaño fijo que contiene
 * metadatos y una colección de registros. Esta clase gestiona el espacio
 * dentro de la página y la conversión hacia/desde un formato binario.
 */
class Page {
public:
    // --- Constructores ---

    /**
     * @brief Constructor por defecto. Crea una página vacía.
     */
    Page();

    // --- Métodos de Gestión de Registros ---

    /**
     * @brief Intenta añadir un registro a la página.
     * @param record El registro a añadir.
     * @return Verdadero si el registro fue añadido con éxito, falso si no hay espacio.
     */
    bool addRecord(const Record& record);

    /**
     * @brief Obtiene todos los registros almacenados en esta página.
     * @return Un vector que contiene los registros de la página.
     */
    std::vector<Record> getRecords() const;

    // --- Métodos de Serialización ---

    /**
     * @brief Serializa la página completa (metadatos y registros) a un buffer de bytes.
     * @param buffer El vector de bytes donde se escribirá la página.
     */
    void serialize(std::vector<char>& buffer) const;

    /**
     * @brief Deserializa una página desde un buffer de bytes.
     * @param buffer El buffer que contiene los datos de la página.
     */
    void deserialize(const std::vector<char>& buffer);

    /**
     * @brief Devuelve el número de registros actualmente en la página.
     */
    uint16_t getNumRecords() const { return numRecords; }

    /**
     * @brief Devuelve la cantidad de espacio libre en la página.
     */
    size_t getFreeSpace() const;

private:
    // --- Metadatos de la Página ---
    // Almacenados al inicio de nuestro buffer de datos.

    // ID de la página, útil para la gestión en un buffer pool.
    uint32_t pageId; 
    
    // Número de registros actualmente en la página.
    uint16_t numRecords;

    // --- Almacenamiento de Datos ---

    // Buffer de tamaño fijo que simula el bloque de datos en disco.
    std::vector<char> data; 
};

#endif //BONSAIDB_PAGE_HPP