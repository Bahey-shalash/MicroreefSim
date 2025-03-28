# MicroreefSim
This repository contains the code for “Projet Microrécif,” a simulation project that models the evolution of micro-organisms in a 2D environment. The project adheres to object-oriented programming principles and utilizes the GTKMM 4 library for graphical interface development.
[Specifications_2023_24_V1.01.pdf](https://github.com/user-attachments/files/16339018/Specifications_2023_24_V1.01.pdf)

## Project Overview

The project is divided into three main phases, each building upon the previous one:

1. **Phase 1**:
   - Implementation of the `shape` module for geometric calculations.
   - Reading and error detection of configuration files.
   - Command-line execution: `./projet txx.txt`.

2. **Phase 2**:
   - Development of the GTKMM graphical interface.
   - Initial graphical display and minimal interface functionality.
   - Simulation of algae creation and disappearance.
   - Command-line execution: `./projet test1.txt`.

3. **Phase 3**:
   - Complete simulation with full GTKMM interface.
   - Comprehensive functionality as described in the project specifications.

## Key Features

- **Geometric Calculations**: Implementation of fundamental geometric structures and functions to handle the spatial aspects of the simulation.
- **Error Handling**: Robust error detection and reporting for configuration files.
- **Graphical Interface**: Utilization of GTKMM 4 to create an intuitive and interactive user interface.
- **Simulation Logic**: Management of the life cycles and interactions of algae, corals, and scavengers within a confined 2D space.

- ## Building and Running

To build the project, navigate to the root directory and run:

```sh
make
```

To run the project with a test configuration file:


```sh
./projet txx.txt
```
## Test Files

The test files for the project are located in the public folder.

## Author

This project is developed by Bahey Shalash. Contributions are managed via GitLab, and we adhere to high coding standards and best practices.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
## Examples 

<img width="645" alt="Screenshot 2024-07-30 at 1 02 47 AM" src="https://github.com/user-attachments/assets/2885c739-7c44-4e81-a2bb-1e5316dcf715">

<img width="647" alt="Screenshot 2024-07-30 at 1 06 14 AM" src="https://github.com/user-attachments/assets/64152fc7-efc9-4049-8632-b6804235e28c">




