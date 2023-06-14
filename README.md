# Singly Linked List

This repository contains a C program that implements a singly linked list with various operations. The program is designed as a part of the "Programmieren 1" assignment and serves as a practical example of data structures, pointers, and memory management in C.

## Project Structure

The program is organized into several files:

- `element.h`: Declares the list elements (nodes).
- `list.h`: Declares the list itself and the functions for list manipulation and output.
- `test.h`: Declares the functions for testing the list functions.
- `main.c`: Includes the header files and contains the main function.
- `utilities.h` and `utilities.c`: These files contain utility functions for the project, including functions for reading from the console with additional control over input. The utilities also include a menu for task selection, implemented using the `ncurses` library.

## Code Improvements and User Interface Enhancement

This project began as a straightforward assignment with simple solutions. However, with the passage of time and increased experience in C programming, I decided to revisit and refine the code. These enhancements, though not part of the original assignment, were introduced as a personal initiative to improve code quality, learn new concepts, and make the program more user-friendly.

A significant upgrade has been the integration of the `ncurses` library, which has transformed the program's user interface. The terminal menu is now more interactive and user-friendly, providing a more engaging experience when selecting tasks.

## Compilation and Execution

To compile and run the program, follow these steps:

1. Ensure you have a C compiler installed (e.g., gcc).
2. Ensure the `ncurses` library is installed on your system.
3. Open a terminal and navigate to the directory containing the source files.
4. Compile the program using the following command:

   ``` bash
   gcc -o Singly-Linked-List main.c utilities.c -lncurses
   ```

5. Run the compiled program with:

   ``` bash
   ./Singly-Linked-List
   ```

   Or, to run the program with tests, use the following command:

   ``` bash
   ./Singly-Linked-List test
   ```

## Contributing

Contributions to this project are welcome. If you have suggestions for improvements or additional features, please feel free to open an issue or submit a pull request on the project's GitHub repository.

## License

This project is licensed under the Apache 2.0 License. See the LICENSE file for more information.

## Acknowledgments

We would like to thank the instructors and teaching assistants of the "Programmieren 1" course for their guidance and support throughout the development of this project.

Additionally, we appreciate the contributions of fellow students and open source community members who have provided feedback and suggestions for improvement.

## Disclaimer

This project is intended for educational purposes only. The creators of this project are not responsible for any misuse or consequences resulting from the use of the program.
