# BonsaiDB 🌳

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Language](https://img.shields.io/badge/language-C%2B%2B17-blue)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

**BonsaiDB** is a high-performance, lightweight database engine written from scratch in C++17. Its purpose is to demonstrate a deep understanding of data structures, memory management, disk access, and concurrent programming—foundational concepts in high-performance backend software development.

---

## ✨ Key Features

* **B+ Tree Index**: Implements a B+ Tree index to massively accelerate search and write operations.
* **Storage Management**: The engine manages storage through a 4 KB paging system, reading and writing directly to binary files.
* **Concurrency Support**: Uses `std::shared_mutex` to allow multiple simultaneous reads and ensure safe writes, maximizing performance in multi-threaded environments.
* **Interactive CLI**: Includes a command-line interface to interact directly with the database.
* **Modern Build with CMake**: Uses CMake for clean, cross-platform build management.

---

## 🚀 Build and Run

The easiest way to compile and run the project is by using the benchmarking script, which handles the entire process.

### Automated Execution (Recommended)

This script will compile the project, run the benchmarks, and generate a performance results graph.

```bash
# Grant execution permissions (only once)
chmod +x benchmarks/ejecutar_benchmarks.sh

# Run the complete process
./benchmarks/ejecutar_benchmarks.sh
```
### Manual Compilation

If you wish to compile the project manually, follow these steps:

```bash
# 1. Create a build directory
mkdir build
cd build

# 2. Configure with CMake (for MinGW on Windows)
cmake -G "MinGW Makefiles" ..

# 3. Compile the project
mingw32-make
```
## 💻 CLI Usage

To interact with the database, run the main `bonsaidb` program from the build directory, followed by the database file name.

```bash
# From the 'build' folder
./bonsaidb my_database.db
```

Once inside, you can use the following commands:

| Command  | Description                  | Example                          |
| :------- | :--------------------------- | :------------------------------- |
| `insert` | Inserts a new record.        | `insert 101 Juan 35 4500.75`     |
| `select` | Searches for a record by ID. | `select 101`                     |
| `delete` | Deletes a record by ID.      | `delete 101`                     |
| `dump`   | Displays all records.        | `dump`                           |
| `help`   | Shows command help.          | `help`                           |
| `exit`   | Closes the application.      | `exit`                           |



## 📊 Benchmarks and Performance

Tests have been conducted to measure the engine's efficiency as the database grows.

### Results

The following table and graph show the results obtained from running the benchmark script. The search time remains extremely low and stable thanks to the B+ Tree's efficiency, even with a large number of records.

| Operation       | 1,000 Records | 10,000 Records | 50,000 Records | 100,000 Records |
| :-------------- | :-----------: | :------------: | :------------: | :-------------: |
| **Insert Time** |    ~45 ms     |    ~180 ms     |    ~950 ms     |    ~2100 ms     |
| **Search Time** |   ~4,500 ns   |   ~5,200 ns    |   ~6,100 ns    |    ~6,800 ns    |

*(Note: These values are approximate. Run the benchmark on your own machine for precise figures.)*

### Performance Graph


## 📂 Project Structure
---
```
bonsaidb/
├── benchmarks/      # Benchmark scripts and programs
├── build/           # Build directory (ignored by git)
├── docs/            # Detailed documentation, diagrams (UML)
├── include/         # Header files (.hpp)
├── src/             # Source code (.cpp)
└── test/            # Unit tests

```

---

## 🤝 Contributing

Pull requests and bug reports are welcome! Feel free to clone, modify, and experiment with the code.

---

## 📜 License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).