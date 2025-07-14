import pandas as pd
import matplotlib.pyplot as plt
import os

plt.style.use('seaborn-v0_8-whitegrid')
plt.rcParams['font.family'] = 'sans-serif'
plt.rcParams['font.sans-serif'] = 'Segoe UI'
plt.rcParams['axes.titlesize'] = 16
plt.rcParams['axes.labelsize'] = 12
plt.rcParams['xtick.labelsize'] = 10
plt.rcParams['ytick.labelsize'] = 10
plt.rcParams['legend.fontsize'] = 12

results_path = os.path.join('resultados', 'performance_results.csv')
if not os.path.isfile(results_path):
    raise FileNotFoundError(
        f"No se encontró '{results_path}'. Ejecuta primero el benchmark de C++ (performance_test)."
    )

df = pd.read_csv(results_path)
required_cols = {'num_records', 'insert_time_ms', 'search_time_ns'}
if not required_cols.issubset(df.columns):
    raise ValueError(f"El archivo debe contener las columnas: {required_cols}")

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 12))
fig.suptitle('Análisis de Rendimiento de BonsaiDB', fontsize=20, y=0.95)

ax1.plot(df['num_records'], df['insert_time_ms'], marker='o', linestyle='-', color='#007BFF', label='Tiempo de Inserción con B+ Tree')
ax1.set_title('Tiempo de Inserción vs. Tamaño de la Base de Datos')
ax1.set_xlabel('Número de Registros')
ax1.set_ylabel('Tiempo (milisegundos)')
ax1.set_xticks(df['num_records'])
ax1.xaxis.set_major_formatter(plt.FuncFormatter(lambda x, _: format(int(x), ',')))
ax1.legend()

ax2.plot(df['num_records'], df['search_time_ns'], marker='s', linestyle='--', color='#28a745', label='Tiempo de Búsqueda con B+ Tree')
ax2.set_title('Tiempo Promedio de Búsqueda vs. Tamaño de la Base de Datos')
ax2.set_xlabel('Número de Registros')
ax2.set_ylabel('Tiempo Promedio (nanosegundos)')
ax2.set_xticks(df['num_records'])
ax2.xaxis.set_major_formatter(plt.FuncFormatter(lambda x, _: format(int(x), ',')))
ax2.legend()

output_path = os.path.join('resultados', 'grafica_rendimiento.png')
plt.tight_layout(rect=[0, 0, 1, 0.95])
plt.savefig(output_path, dpi=150)

print(f"Gráfica generada y guardada exitosamente en:\n  {output_path}")
