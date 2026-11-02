# Internal IT Service Desk Engine

A C++ low-level design (LLD) project that models an internal IT service desk. It creates categorized support tickets, processes them in severity order, enforces a ticket lifecycle through the State pattern, and sends status notifications through the Observer pattern.

## Features

- **Ticket creation:** A factory assigns a default priority based on ticket category.
- **Lifecycle management:** Tickets follow the defined flow `NEW → ASSIGNED → RESOLVED → CLOSED`.
- **Priority-based dispatch:** A priority queue processes Critical tickets before High, Medium, and Low tickets.
- **Notifications:** Observers receive updates when a ticket is assigned, resolved, or closed.
- **Shared ownership:** `shared_ptr` is used for tickets, states, and observers in this example.

## Design Patterns and Data Structures

| Pattern / Structure | Responsibility |
| --- | --- |
| Factory | Creates tickets and maps ticket categories to default priorities. |
| State | Keeps lifecycle behavior in state-specific classes and controls valid transitions. |
| Observer | Decouples ticket status changes from notification delivery. |
| Priority queue (heap) | Selects the highest-priority ticket for processing. |

### Ticket priority mapping

| Ticket category | Default priority |
| --- | --- |
| Hardware | High |
| Access | Medium |
| Incident | Critical |

The `Priority` enum is ordered from `LOW` to `CRITICAL`. The priority-queue comparator uses that ordering so the highest numeric priority is returned first.

## Project Structure

```text
Internal-IT-Service-Desk-Incident-Management-Engine/
├── docs/
│   └── PRD.md
├── main.cpp
└── README.md
```

## Requirements

- A C++14-compatible compiler (for example, GCC or Clang).
- Standard C++ library.

## Build and Run

From the repository root:

```bash
g++ -std=c++14 -Wall -Wextra -pedantic main.cpp -o service-desk
./service-desk
```

On Windows PowerShell, run the generated executable with:

```powershell
.\service-desk.exe
```

## Example Workflow

The demo creates two tickets:

1. An Access ticket with Medium priority.
2. An Incident ticket with Critical priority.

Both tickets are added to the priority queue. The Incident ticket is processed first, followed by the Access ticket. For each ticket, the demo assigns an agent, resolves the issue, closes the ticket, and prints observer notifications for the state changes.

## Sample Output

```text
--- Processing Support Queue ---

Handling Ticket #102 | [Incident] Database server down | Priority: CRITICAL
[Notification] Ticket #102 -> Assigned to agent Alex
[Notification] Ticket #102 -> Resolved with note: Issue fixed successfully.
[Notification] Ticket #102 -> Ticket closed.

Handling Ticket #101 | [Access] VPN access request | Priority: MEDIUM
[Notification] Ticket #101 -> Assigned to agent Alex
[Notification] Ticket #101 -> Resolved with note: Issue fixed successfully.
[Notification] Ticket #101 -> Ticket closed.
```

## Scope and Current Limitations

This repository is an in-memory LLD demonstration, not a production ticketing service. It currently does not include persistent storage, an API or user interface, authentication, concurrent dispatch, real email delivery, timestamps, or automatic SLA measurement. The SLA targets are documented in the PRD as product goals; they are not enforced or measured by the current C++ implementation.

## Product Documentation

See [docs/PRD.md](docs/PRD.md) for the problem statement, user personas, functional requirements, and service-level KPIs.
