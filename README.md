# IPC Application Suite: Data Creator, Data Reader, and Data Corruptor

Welcome to the IPC Application Suite, a system comprising three distinct processing components: Data Creator (DC), Data Reader (DR), and Data Corruptor (DX) applications. This project challenges you to design and implement these components, showcasing how different Inter-Process Communication (IPC) mechanisms are employed within UNIX/Linux System Application programming.

## Team Collaboration

Given the scope of this assignment, teamwork is encouraged. We recommend partnering up as follows:

- One member handles both the DC (client) and DX (corruptor) applications.
- Another member focuses on the DR (server) application.

Please ensure both members sign up and enroll in the same group using the A-03 Group sign-up in Course Tools.

## System Overview

The IPC Application Suite entails the following considerations:

- Modular design and adherence to SET Coding Standards for all applications.
- File-header and function comment blocks for enhanced readability.
- Collaboration on the shared codebase using C programming language.
- Proper directory structure and makefiles for each component.

### Data Creator (DC)

#### Purpose

The DC application generates simulated status conditions for the Hoochamacallit machine. It sends messages via a message queue to the Data Reader application.

- DC verifies message queue existence before sending the first message.
- The application enters a loop, attempting to establish the queue if it doesn't exist.
- After queue creation, the DC sends a series of messages, including machine ID and status.
- Status values are randomly selected from predefined conditions.
- Messages are sent at random intervals (10 to 30 seconds).
- The DC continues sending messages until a "Machine is Off-line" status.

### Data Reader (DR)

#### Purpose

The DR application monitors incoming messages, tracks machine statuses, and logs findings to a data monitoring log file.

- DR creates and maintains a master list of communicating DCs.
- Communication involves updating time stamps and responding to machine off-line events.
- DR detects non-responsive DCs and logs removal events.
- The application logs various events with timestamps to an ASCII log file.
- DR exits gracefully when all DCs have terminated.

### Data Corruptor (DX)

#### Purpose

DX gains insight into system resources, randomly choosing actions to disrupt normal operations.

- DX attaches to shared memory created by the DR application.
- Shared memory holds a master list of communicating DCs.
- DX executes random actions from the "Wheel of Destruction."
- Actions include killing DC processes and deleting message queues.
- DX logs actions with timestamps to an ASCII log file.
- DX exits if the message queue is not detected or all DCs have terminated.

## Execution and Development

- During development, debug output may be printed to the screen.
- For final submission, ensure no output is printed from applications.
- All code must adhere to C programming standards.
- Collaboratively develop and structure your solution using modular design and proper documentation.

Unlock the realm of Inter-Process Communication in UNIX/Linux with the IPC Application Suite and gain a deeper understanding of communication mechanisms and exception handling.

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
