# PRD: Internal IT Service Desk Engine

## 1. Problem Statement

Internal IT operations can be delayed when hardware, access, and incident requests are routed manually. Manual triage can lead to slow acknowledgements, missed service-level targets, and limited visibility into ticket progress.

The Internal IT Service Desk Engine is a small ticket-management system designed to demonstrate category-based ticket creation, severity-driven dispatch, controlled lifecycle transitions, and status notifications.

## 2. User Personas

- **Internal Employee:** Submits IT tickets and tracks resolution progress.
- **IT Support Agent:** Receives prioritized tickets based on severity and works on assigned requests.
- **IT Program Manager:** Monitors team performance, resolution bottlenecks, and SLA breach rates.

## 3. Goals and Non-Goals

### Goals

- Apply default priorities based on ticket category.
- Process the highest-priority ticket first.
- Restrict lifecycle changes to the supported state-transition flow.
- Notify subscribed observers when ticket status changes.

### Non-Goals for the Current Prototype

- Persistent ticket storage or a web/mobile interface.
- Authentication, authorization, or multi-team access control.
- Integration with a real email or messaging provider.
- Production-grade concurrent dispatch or SLA timers.

## 4. Functional Requirements

- **FR-1 — Ticket Creation:** The system must create predefined ticket categories and map each category to a default priority:
  - Hardware → High
  - Access → Medium
  - Incident → Critical
- **FR-2 — Lifecycle Governance:** Ticket states must transition only through the defined flow: `New → Assigned → Resolved → Closed`. Unsupported actions must be rejected.
- **FR-3 — Automated Dispatch:** The system must queue tickets and select them in descending priority order.
- **FR-4 — Real-Time Notifications:** Subscribed observers must receive an update when a ticket is assigned, resolved, or closed.

## 5. Ticket Lifecycle

```text
NEW --assign(agent)--> ASSIGNED
ASSIGNED --resolve(note)--> RESOLVED
RESOLVED --close()--> CLOSED
```

Any other lifecycle action is invalid in the current design.

## 6. Priority Rules

The prototype uses the following severity order:

1. Critical
2. High
3. Medium
4. Low

When two tickets have the same priority, their relative processing order is not specified as a product requirement.

## 7. Key Performance Indicators (KPIs)

These are proposed service targets for a future production implementation. The current in-memory C++ prototype does not record timestamps or calculate these metrics.

| KPI | Target |
| --- | --- |
| Mean Time to Acknowledge (MTTA) | Less than 15 minutes for Critical incidents |
| Mean Time to Resolve (MTTR) | Less than 4 hours for Critical incidents |
| SLA Breach Rate | Below 5% of tickets per month |

## 8. Acceptance Criteria

- Creating a Hardware ticket assigns High priority.
- Creating an Access ticket assigns Medium priority.
- Creating an Incident ticket assigns Critical priority.
- A newly created ticket starts in the New state.
- A ticket can be assigned only while it is New.
- A ticket can be resolved only while it is Assigned.
- A ticket can be closed only while it is Resolved.
- The priority queue returns Critical tickets before Medium or Low tickets.
- Registered observers receive notifications for each successful lifecycle transition.

## 9. Assumptions and Dependencies

- Ticket IDs are supplied by the caller and are unique within the running process.
- Priority values follow the enum ordering in the C++ implementation.
- Notifications are demonstrated through console output; no external email service is connected.
- SLA targets require timestamps, persistence, and reporting capabilities that are outside the current prototype.
