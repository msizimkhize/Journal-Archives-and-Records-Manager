# Journal Archives and Records Manager

A high-performance C++ desktop application built with the **Qt Framework** and orchestrated via **CMake**, designed for precise academic bookkeeping, data validation, and record archival management.

## Project Overview

This software provides a structured, administrative Graphical User Interface (GUI) to catalogue, track, and manage academic journal publications. Built with an emphasis on data integrity and user-centric state management, the application processes crucial publication metadata—such as publication year, authors, journal names, volumes, and page configurations—and commits them to a secure records architecture.

A key feature of the system is its real-time temporal processing engine, which automatically calculates the age of a publication from its metadata and conditionally highlights rows in **red or green** to immediately communicate archive status to the administrator.

## Key Features

*   **Multi-Layered Data Validation:** Implements a robust input verification pipeline that checks and sanitises all incoming metadata fields, ensuring structural uniformity and preventing runtime errors or database corruption.
*   **Dynamic Age Visualisation:** Features automated backend logic that runs temporal calculations on entries, updating the table-view UI state with conditional cell-colouring (red/green) based on record age.
*   **Advanced Data Manipulation:** Includes a flexible manipulation engine allowing users to search, apply filters, and dynamically sort records by specific criteria (e.g., Journal Name, Author, Pages).
*   **Secure Record Control:** Integrated with a dedicated deletion module to safely remove archival entries and update the underlying data structures in real-time.

## Technical Stack

*   **Core Logic:** C++ (Standard 17 or later)
*   **Framework:** Qt Creator 6.5+ (Qt Widget Architecture)
*   **Build System:** CMake
*   **UI Components:** QTableView & Custom Conditional Delegate Models

## Architecture & Implementation

The repository is structured around modern software engineering principles to maintain a clean separation of concerns:
1.  **Model-View Separation:** Utilises specialised data models to decouple the underlying journal records from the GUI presentation layer, ensuring high performance even with large datasets.
2.  **Input Sanitisation Pipeline:** Employs precise regular expressions and input validation rules directly within the entry components to catch formatting anomalies instantly.
3.  **Modern CMake Orchestration:** Developed completely using CMake to guarantee clean dependency tracking, maintainable modular file structure, and reliable cross-platform compilation.

---

## Getting Started

### Prerequisites
*   **Qt Creator 6.5** (or later) configured with a valid C++ compiler (GCC, Clang, or MSVC).
*   **CMake 3.16** or higher.

### Build Instructions
1.  Clone the repository to your local directory.
2.  Open the project root `CMakeLists.txt` file within **Qt Creator**.
3.  Select your desired build kit (Windows, macOS, or Linux).
4.  Run **Configure** followed by **Build** to compile the binaries.

#### Reach Out on LinkedIn

[![](https://raw.githubusercontent.com/msizimkhize/Vegetable-Prices-2022-MySQL-PowerBI/adc81f1d2ee7e3d74868bc8fdf859f13eba1fe5b/IMG/68747470733a2f2f696d672e736869656c64732e696f2f62616467652f4c696e6b6564496e2d436f6e6e6563742d626c75653f7374796c653d666f722d7468652d6261646765266c6f676f3d6c696e6b6564696e.svg)](https://www.linkedin.com/in/msizimkhize/)
