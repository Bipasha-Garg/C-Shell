
# Description/Assumptions

## \_\_

    white space for printing in terminal or in history file not handled

## BG

    background process will be killed when user exit from shell

    Max arguments accepted is 1024

    A maximum limit on the number of background processes.

    Valid shell commands as input for background execution.

    Simple tokenization of commands by spaces.

    Management of background processes using an array.

    Error handling for system calls.

    Single-threaded execution.

    No user input validation.

    Potential limitations with very long commands.

## HISTORY

    History File: The code assumes it can read and write a history file named "history.txt."

    Fixed Sizes: It uses fixed sizes for data structures.

    Memory Allocation: Assumes successful memory allocation.

    Command Comparison: Compares commands based on tokenized strings.

    Line Endings: Assumes lines in the history file end with '\n.'

    History Index: Expects the user to provide a valid index for past commands (1 to 15).

    File Operations: Assumes file operations will succeed.

    User Input: Assumes user-provided commands are well-formed and not excessively long.

## input

    File Operations: The code assumes it can read and write files, particularly for history storage.

    Fixed Sizes: Fixed sizes are used for data structures and buffer allocations.

    Memory Allocation: It assumes successful memory allocation with malloc and strdup.

    Command Execution: Expects commands to be executed by calling specific functions based on their names.

    Background Processes: Handles background processes and allows checking and termination.

    Input Handling: Assumes command input is separated into background and foreground components based on '&'

    Command History: Manages a history of commands and provides options for displaying, purging, and executing past commands.

    Command Validation: The code verifies the validity of specific commands and parameters.

## PEEK

    File Access: Assumes it can read and access files and directories.

    Command Arguments: Expects command-line arguments to be passed in the "extract" parameter.

    Special Paths Handling: Handles special symbols and paths like ".", "..", "~", and "-".

    Memory Allocation: Assumes successful memory allocation with malloc.

    Directory Operations: Assumes it can open directories and read their contents.

    Sorting: Sorts directory entries alphabetically.

    File Details: Optionally displays file details like permissions, owner, group, size, and modification time.

    Color Printing: Prints file and directory names in color.

    Flag Handling: Handles flags like "-a" (show hidden files) and "-l" (show file details).

    Combined Flags: Handles combined flags like "-al" or "-la" appropriately.

## PROCLORE

    Header Files: The code includes header files for necessary declarations.

    Memory Allocation: Dynamic memory is allocated for column_2 and status.

    proc_details Function: Retrieves and displays process details using "/proc" file system. Handles errors but does not terminate.

    Error Handling: Prints error messages for file opening and reading errors.

    Executable Path Handling: Tries to abbreviate the executable path with "~" if it matches the user's home directory.

    proclore_func Function: Wrapper for proc_details, determines whether to get info for a specific PID or the current process.

    Input Parameters: Relies on extract for PID, home for home directory, and home_size (unused).

## PROMPT

    Header Files: The code includes header files "headers.h" and "prompt.h" for necessary declarations.

    Environment Variables: It assumes that the environment variable USER contains the current username.

    Current Directory: The code retrieves the current directory using getcwd and assumes it will fit in the curr_dir buffer of size MAX_ARR_SIZE.

    Error Handling: If there's an error while retrieving the current directory, it prints an error message but does not terminate.

    Prompt Format: The code generates a command prompt in the format <username@hostname:~> or <username@hostname:~path> based on comparisons between the current directory (curr_dir) and the user's home directory (home) and its size (home_size).

    Assumption for Doubt: In the case where the length of the current directory (curr_dir) is less than the specified home directory size (home_size), it assumes that the prompt should still be <username@hostname:~>.

## SEEK

    Header Files: The code includes header files "headers.h" and "seek.h" for necessary declarations.

    Color Printing: It defines a function print_color to print text with specified color codes.

    Directory Search: The particular function searches for files and directories within a given target_dir based on provided flags and a name to match.

    Directory Traversal: The particular function recursively traverses subdirectories while searching for matching files and directories.

    Command Parsing: The call_seek function parses a command line input, allowing for various flags (-d, -f, -e) and a target directory and name to search for.

    Usage Message: If the command line input is not properly formatted, it prints a usage message.

    Memory Management: It dynamically allocates memory for name and target_dir and frees them when they are no longer needed.

    Input Format: The code assumes that the input follows the format: seek [-d] [-f] [-e] <name> [<target_dir>], where -d, -f, and -e are optional flags, name is the target name to search for, and target_dir is the directory to start the search (defaults to the current directory if not provided).

    Error Handling: The code handles errors related to directory opening and file stat operations, printing error messages but continuing execution.

    Color Printing: The print_color function is assumed to work as intended, applying color codes to text for terminal output.

## WARP

    all the commands are working(i hope)

    If the user is already on home directory then by performing "warp ~" the user will remain
    on the same home directory only.

    there is no restriction on the number of arguments(path/directory)
    
    Header Files: The code includes header files "warp.h" and "headers.h" for necessary declarations.

    Buffer Size: A character buffer buffer of size 10,000 is used to store directory paths and filenames.

    Changing Directories: The code changes the current working directory based on different input cases, including moving to the previous directory, staying in the current directory, going up one directory, going to the home directory, and navigating to specified paths.

    Error Handling: The code includes error handling for various operations like getting the current working directory, changing directories, and accessing directories. Error messages are printed when errors occur.

    Tokenization: The input is tokenized based on spaces, and different cases are handled for each token.

    Home Directory: The code assumes that the user's home directory is provided as home_directory and is a valid path.

    User Home Directory: It retrieves the user's home directory using getpwnam based on the current user's login.

    Path Concatenation: Path concatenation is performed using strcat to create full directory paths.

    Directory Existence: It checks for the existence of directories using access with the F_OK flag.

    Output: After changing directories, the current working directory (cwd) is printed as output.
