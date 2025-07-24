# VarHandler

VarHandler is a C++ application for managing key-value variables stored in a PostgreSQL database. It provides both a graphical user interface (GUI) and a RESTful API for interacting with your variables.

## Features

- **Graphical User Interface (GUI):**
  - View all variables in the database.
  - Edit or delete existing variables.
  - Insert new variables.
  - Built with SDL2 and ImGui for a modern, responsive interface.

- **RESTful API:**
  - `GET /getVars` — Retrieve all variables.
  - `GET /getVar/<key>` — Retrieve a specific variable by key.
  - `POST /setVar` — Insert a new variable (`{ "key": "...", "value": "..." }`).
  - `PUT /updateVar` — Update an existing variable (`{ "key": "...", "value": "..." }`).
  - `DELETE /deleteVar/<key>` — Delete a variable by key.
  - `POST /shutdown` — Shut down the API server.

- **Database Integration:**
  - Uses PostgreSQL for persistent storage.
  - Connection details are loaded from a `.env` file.

- **Logging:**
  - All actions are logged to `logs/api.log` using spdlog.

## Getting Started

### Prerequisites

- C++17 or later
- PostgreSQL server
- CMake
- SDL2 development libraries
- OpenGL development libraries
- [libpqxx](https://github.com/jtv/libpqxx) (PostgreSQL C++ connector)
- [ImGui](https://github.com/ocornut/imgui) (included as a submodule or in `lib/`)
- [Crow](https://github.com/CrowCpp/crow) (included as a header in `lib/`)
- [spdlog](https://github.com/gabime/spdlog) (included in `lib/`)

### Configuration

Create a `.env` file in the project root with your database URL:

```
DB_URL=postgresql://user:password@localhost:5432/yourdb
```

### Building

```sh
mkdir build
cd build
cmake ..
make
```

### Running

```sh
./varHandler
```

- The GUI will open.
- The REST API will be available at `http://localhost:8080`.

## Usage

- Use the GUI to manage variables visually.
- Use the API to integrate with other tools or scripts.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
