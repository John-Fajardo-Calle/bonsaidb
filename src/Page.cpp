#include "Page.hpp"
#include <cstring> // Para memcpy

/**
 * @brief Constructor que inicializa una página vacía.
 */
Page::Page() : pageId(0), numRecords(0) {
    // La página se crea vacía, así que inicializamos el buffer de datos
    // con ceros. data.resize() asigna la memoria y la inicializa.
    data.resize(PAGE_SIZE, 0);
}

/**
 * @brief Calcula el espacio libre restante en la página.
 * @return El número de bytes disponibles.
 */
size_t Page::getFreeSpace() const {
    // El espacio usado es el tamaño de los metadatos más el espacio de
    // todos los registros almacenados.
    const size_t metadataSize = sizeof(pageId) + sizeof(numRecords);
    const size_t recordsSize = numRecords * Record::getRecordSize();
    return PAGE_SIZE - metadataSize - recordsSize;
}

/**
 * @brief Intenta añadir un nuevo registro a la página.
 * @param record El registro que se va a añadir.
 * @return true si se añadió con éxito, false si no había espacio.
 */
bool Page::addRecord(const Record& record) {
    // Comprobamos si hay suficiente espacio libre para un nuevo registro.
    if (getFreeSpace() < Record::getRecordSize()) {
        return false; // No hay espacio suficiente.
    }

    // Serializamos el registro a un buffer temporal.
    std::vector<char> recordBuffer;
    record.serialize(recordBuffer);

    // Calculamos la posición donde comenzará el nuevo registro.
    // Es después de los metadatos y de los registros existentes.
    const size_t offset = sizeof(pageId) + sizeof(numRecords) + (numRecords * Record::getRecordSize());

    // Copiamos los datos del registro serializado en el buffer de la página.
    memcpy(data.data() + offset, recordBuffer.data(), Record::getRecordSize());

    // Incrementamos el contador de registros.
    numRecords++;

    return true;
}

/**
 * @brief Extrae y devuelve todos los registros de la página.
 * @return Un vector con los objetos Record deserializados.
 */
std::vector<Record> Page::getRecords() const {
    std::vector<Record> records;
    if (numRecords == 0) {
        return records;
    }

    records.reserve(numRecords); // Optimizacion: reservamos memoria.

    // El primer registro comienza justo después de los metadatos.
    size_t offset = sizeof(pageId) + sizeof(numRecords);

    // Iteramos y deserializamos cada registro.
    for (uint16_t i = 0; i < numRecords; ++i) {
        Record rec;
        // La función deserialize de Record leerá desde el offset y lo actualizará.
        rec.deserialize(this->data, offset);
        records.push_back(rec);
    }

    return records;
}

/**
 * @brief Serializa la página completa a un buffer externo.
 * Escribe los metadatos actualizados y los datos de los registros.
 * @param buffer El buffer de destino.
 */
void Page::serialize(std::vector<char>& buffer) const {
    buffer.resize(PAGE_SIZE);

    // Puntero para escribir en el buffer de salida.
    char* ptr = buffer.data();

    // Escribimos los metadatos (pageId y numRecords) primero.
    memcpy(ptr, &this->pageId, sizeof(this->pageId));
    ptr += sizeof(this->pageId);

    memcpy(ptr, &this->numRecords, sizeof(this->numRecords));
    ptr += sizeof(this->numRecords);

    // Copiamos el resto de los datos (que son los registros ya serializados).
    // La fuente es el buffer interno 'data' de nuestra página, saltando sus metadatos.
    const size_t recordsDataSize = this->numRecords * Record::getRecordSize();
    if (recordsDataSize > 0) {
        memcpy(ptr, this->data.data() + sizeof(pageId) + sizeof(numRecords), recordsDataSize);
    }
}

/**
 * @brief Deserializa (carga) el estado de una página desde un buffer.
 * @param buffer El buffer de origen, debe tener un tamaño de PAGE_SIZE.
 */
void Page::deserialize(const std::vector<char>& buffer) {
    if (buffer.size() != PAGE_SIZE) {
        // En un motor real, aquí se lanzaría una excepción o se manejaría el error.
        return;
    }

    // Copiamos todo el bloque de datos.
    this->data = buffer;

    // Extraemos los metadatos del inicio del buffer que acabamos de copiar.
    const char* ptr = this->data.data();

    memcpy(&this->pageId, ptr, sizeof(this->pageId));
    ptr += sizeof(this->pageId);

    memcpy(&this->numRecords, ptr, sizeof(this->numRecords));
}