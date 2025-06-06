#include "Record.hpp"
#include <cstring> // Para usar memcpy

/**
 * @brief Serializa el objeto Record en un buffer de bytes.
 *
 * Esta función convierte los miembros de la estructura (id, name, age, balance)
 * en una secuencia de bytes y los escribe en el vector 'buffer'.
 * Se utiliza memcpy para una copia de bajo nivel, que es muy eficiente
 * para estructuras de datos de tamaño fijo.
 *
 * @param buffer El vector de bytes donde se escribirá el registro.
 */
void Record::serialize(std::vector<char>& buffer) const {
    // Aseguramos que el buffer tenga el tamaño exacto necesario.
    buffer.resize(getRecordSize());

    // Creamos un puntero al inicio del buffer para ir escribiendo.
    char* ptr = buffer.data();

    // Copiamos cada miembro de la estructura en el buffer, uno tras otro.
    memcpy(ptr, &id, sizeof(id));
    ptr += sizeof(id);

    memcpy(ptr, &name, sizeof(name));
    ptr += sizeof(name);

    memcpy(ptr, &age, sizeof(age));
    ptr += sizeof(age);

    memcpy(ptr, &balance, sizeof(balance));
}

/**
 * @brief Deserializa un registro desde un buffer de bytes.
 *
 * Esta función lee una secuencia de bytes desde un buffer y la utiliza
 * para poblar los miembros del objeto Record (id, name, age, balance).
 * Se asume que el buffer contiene los datos en el mismo orden y formato
 * en que fueron serializados.
 *
 * @param buffer El vector de bytes desde donde se leerá el registro.
 * @param offset Un punto de inicio en el buffer. Se actualiza después de la lectura.
 */
void Record::deserialize(const std::vector<char>& buffer, size_t& offset) {
    // Creamos un puntero a la posición inicial desde donde leer en el buffer.
    const char* ptr = buffer.data() + offset;

    // Copiamos los bytes del buffer hacia cada miembro de la estructura.
    memcpy(&id, ptr, sizeof(id));
    ptr += sizeof(id);

    memcpy(&name, ptr, sizeof(name));
    ptr += sizeof(name);

    memcpy(&age, ptr, sizeof(age));
    ptr += sizeof(age);

    memcpy(&balance, ptr, sizeof(balance));

    // Actualizamos el offset para que la próxima lectura comience
    // justo después de este registro.
    offset += getRecordSize();
}