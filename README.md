*This project has been created as part of the 42 curriculum by syanak.*

# Minitalk

## Description

Minitalk is a project that establishes communication between two processes using Unix signals (`SIGUSR1` and `SIGUSR2`). The project consists of two programs: **server** and **client**. The client sends a text message to the server, and the server receives it signal by signal, reconstructing and printing it to the terminal. All communication is carried out bit by bit using only two signals.

### Project Goals

- Understand how Unix signals (`SIGUSR1`, `SIGUSR2`) work
- Apply Inter-Process Communication (IPC) in practice
- Implement data transmission using bitwise operations

## How It Works

### Communication Flow Overview

```
CLIENT                                    SERVER
======                                    ======
  |                                         |
  |<-- 32 bit PID transmission (SIGUSR1/2) --->|  (Handshake)
  |                                         |
  |<-- 8 bit character transmission ------->|  (Message)
  |<-- SIGUSR1 (acknowledgment) ----------->|  (ACK per bit)
  |                                         |
  |<-- 8 bit NULL byte (0x00) ------------->|  (End of message)
  |<-- SIGUSR1 (final acknowledgment) ----->|
  |                                         |
EXIT                                CONTINUE WAITING
```

### 1. Signal-Bit Mapping

| Signal   | Meaning   |
|----------|-----------|
| SIGUSR1  | Bit `1`   |
| SIGUSR2  | Bit `0`   |

Each character is transmitted as 8 bits, with each bit sent as a single signal. For example, the letter `A` (ASCII 65, binary `01000001`) is sent as the sequence `SIGUSR2, SIGUSR1, SIGUSR2, SIGUSR2, SIGUSR2, SIGUSR2, SIGUSR2, SIGUSR1`.

### 2. Client PID Transmission (Handshake)

Before sending the message, the client transmits its own PID to the server. This allows the server to send acknowledgment signals back to the correct process.

- The client PID is sent as 32 bits from MSB (most significant bit) to LSB
- A 20-microsecond delay (`usleep(20)`) is used between each bit
- No acknowledgment is expected from the server during this phase (asynchronous)

```c
// client.c - PID transmission
void send_client_pid(int client_pid, int server_pid)
{
    int bit = 32;
    while (bit--)
    {
        if (client_pid & (1 << bit))
            kill(server_pid, SIGUSR1);   // bit 1
        else
            kill(server_pid, SIGUSR2);   // bit 0
        usleep(20);
    }
}
```

On the server side, `receive_pid()` accumulates each incoming signal and writes the PID to a global variable once all 32 bits are received:

```c
// server.c - PID reception
void receive_pid(int sig)
{
    static int bit = 32;
    static int pid = 0;

    bit--;
    if (sig == SIGUSR1)
        pid = pid | (1 << bit);    // set the corresponding bit to 1
    if (bit == 0)
    {
        g_client_pid = pid;        // PID complete
        pid = 0;
        bit = 32;
        kill(g_client_pid, SIGUSR1); // send acknowledgment
    }
}
```

### 3. Message Transmission (Bit by Bit)

After the PID handshake is complete, the client sends each character of the message as 8 bits:

- After each bit is sent, the client waits for an acknowledgment signal (`SIGUSR1`) from the server
- The client synchronizes via busy-wait using the `g_flag` variable
- The server responds with `SIGUSR1` after receiving each bit
- Once 8 bits are complete, the server prints the reconstructed character to the terminal
- When a null byte (`0x00`) is received, the message is considered complete and a newline is printed

```c
// client.c - Byte transmission
void send_byte(char byte, int server_pid)
{
    int i = 8;
    while (i--)
    {
        g_flag = 0;
        if (byte & (1 << i))
            kill(server_pid, SIGUSR1);
        else
            kill(server_pid, SIGUSR2);
        while (!g_flag)        // wait until server acknowledges
            ;
    }
}
```

### 4. Server Signal Routing

The server routes incoming signals to the appropriate handler based on the current state:

```c
void handle_signal(int sig)
{
    if (g_client_pid == 0)
        receive_pid(sig);     // PID not yet received -> handshake
    else
        receive_msg(sig);     // PID received -> receive message
}
```

## Instructions 

### Compilation

```bash
make        # compiles both server and client
make clean  # removes executables
make fclean # same as clean
make re     # clean rebuild
```

Compiled using the `cc` compiler with `-Wall -Wextra -Werror` flags.

### Running

**Step 1:** Start the server:

```bash
./server
```

The server's PID is displayed as output:

```
PID: 12345
```

**Step 2:** Open another terminal and send a message using the client:

```bash
./client 12345 "Hello World"
```

The message appears on the server terminal:

```
Hello World
```

### Usage

```
./server
./client <server_pid> <message>
```

| Argument      | Description                       |
|---------------|-----------------------------------|
| `server_pid`  | The PID printed by the server     |
| `message`     | The text to send                  |

## Technical Details

| Property                 | Detail                                          |
|--------------------------|-------------------------------------------------|
| Signals used             | `SIGUSR1` (bit 1), `SIGUSR2` (bit 0)           |
| PID transmission         | 32 bits, MSB-first, 20us interval, no ACK      |
| Message transmission     | 8 bits/char, MSB-first, ACK per bit            |
| End-of-message marker    | Null byte (`0x00`)                              |
| Synchronization          | Client: busy-wait, Server: signal-based         |
| Message length limit     | None (up to system resource limits)             |
| Compatibility            | POSIX-compliant (Linux/Unix)                    |

## Resources

- [signal(7) - Linux Man Page](https://man7.org/linux/man-pages/man7/signal.7.html) - Detailed documentation on Unix signals
- [kill(2) - Linux Man Page](https://man7.org/linux/man-pages/man2/kill.2.html) - Sending signals to processes
- [sigaction(2) - Linux Man Page](https://man7.org/linux/man-pages/man2/sigaction.2.html) - Defining signal handlers
- [Bitwise Operators in C](https://en.wikipedia.org/wiki/Bitwise_operations_in_C) - Bitwise operations in C
- [Inter-Process Communication](https://en.wikipedia.org/wiki/Inter-process_communication) - IPC methods overview

### AI Usage

AI tools were not used during the coding phase of this project. AI assistance was used for formatting the README documentation.
