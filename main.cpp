#include <iostream> 
#include <string> 
#include <vector> 
#include <queue> 
#include <memory> 
 
using namespace std; 
 
enum class TicketType { HARDWARE, ACCESS, INCIDENT }; 
enum class Priority { LOW, MEDIUM, HIGH, CRITICAL }; 
 
string getPriorityStr(Priority p) { 
    switch(p) { 
        case Priority::LOW: return "LOW"; 
        case Priority::MEDIUM: return "MEDIUM"; 
        case Priority::HIGH: return "HIGH"; 
        case Priority::CRITICAL: return "CRITICAL"; 
    } 
    return "UNKNOWN"; 
} 
 
// Observer for simple email/status alerts 
class Observer { 
public: 
    virtual void update(int id, string msg) = 0; 
}; 
 
class EmailNotifier : public Observer { 
public: 
    void update(int id, string msg) override { 
        cout << "[Notification] Ticket #" << id << " -> " << msg << endl; 
    } 
}; 
 
class Ticket; 
 
// State pattern for ticket lifecycle 
class State { 
public: 
    virtual void assign(Ticket* t, string agent) { cout << "Invalid action for current state.\n"; } 
    virtual void resolve(Ticket* t, string note) { cout << "Invalid action for current state.\n"; } 
    virtual void close(Ticket* t) { cout << "Invalid action for current state.\n"; } 
    virtual string getName() = 0; 
}; 
 
class Ticket { 
private: 
    int id; 
    string title; 
    TicketType type; 
    Priority priority; 
    shared_ptr<State> currentState; 
    string assignedAgent; 
    vector<shared_ptr<Observer>> observers; 
 
public: 
    Ticket(int id, string title, TicketType type, Priority priority); 
 
    void setState(shared_ptr<State> s) { currentState = s; } 
    int getId() const { return id; } 
    Priority getPriority() const { return priority; } 
    string getTitle() const { return title; } 
 
    void addObserver(shared_ptr<Observer> o) { observers.push_back(o); } 
    void notify(string msg) { 
        for (auto& o : observers) o->update(id, msg); 
    } 
 
    void assign(string agent); 
    void resolve(string note); 
    void close(); 
     
    void setAgent(string agent) { assignedAgent = agent; } 
}; 
 
class NewState : public State { 
public: 
    string getName() override { return "NEW"; } 
    void assign(Ticket* t, string agent) override; 
}; 
 
class AssignedState : public State { 
public: 
    string getName() override { return "ASSIGNED"; } 
    void resolve(Ticket* t, string note) override; 
}; 
 
class ResolvedState : public State { 
public: 
    string getName() override { return "RESOLVED"; } 
    void close(Ticket* t) override; 
}; 
 
class ClosedState : public State { 
public: 
    string getName() override { return "CLOSED"; } 
}; 
 
// State transitions 
void NewState::assign(Ticket* t, string agent) { 
    t->setAgent(agent); 
    t->setState(make_shared<AssignedState>()); 
    t->notify("Assigned to agent " + agent); 
} 
 
void AssignedState::resolve(Ticket* t, string note) { 
    t->setState(make_shared<ResolvedState>()); 
    t->notify("Resolved with note: " + note); 
} 
 
void ResolvedState::close(Ticket* t) { 
    t->setState(make_shared<ClosedState>()); 
    t->notify("Ticket closed."); 
} 
 
Ticket::Ticket(int id, string title, TicketType type, Priority priority)  
    : id(id), title(title), type(type), priority(priority) { 
    currentState = make_shared<NewState>(); 
} 
 
// Factory pattern for creating tickets 
class TicketFactory { 
public: 
    static shared_ptr<Ticket> createTicket(int id, TicketType type, string desc) { 
        if (type == TicketType::HARDWARE) 
            return make_shared<Ticket>(id, "[Hardware] " + desc, type, Priority::HIGH); 
        else if (type == TicketType::ACCESS) 
            return make_shared<Ticket>(id, "[Access] " + desc, type, Priority::MEDIUM); 
        else 
            return make_shared<Ticket>(id, "[Incident] " + desc, type, Priority::CRITICAL); 
    } 
}; 
 
// Comparator for priority queue (highest priority first) 
struct ComparePriority { 
    bool operator()(shared_ptr<Ticket>& a, shared_ptr<Ticket>& b) { 
        return static_cast<int>(a->getPriority()) < static_cast<int>(b->getPriority()); 
    } 
}; 
 
int main() { 
    auto notifier = make_shared<EmailNotifier>(); 
    priority_queue<shared_ptr<Ticket>, vector<shared_ptr<Ticket>>, ComparePriority> pq; 
 
    // Create sample tickets 
    auto t1 = TicketFactory::createTicket(101, TicketType::ACCESS, "VPN access request"); 
    auto t2 = TicketFactory::createTicket(102, TicketType::INCIDENT, "Database server down"); 
 
    t1->addObserver(notifier); 
    t2->addObserver(notifier); 
 
    pq.push(t1); 
    pq.push(t2); 
 
    cout << "--- Processing Support Queue ---" << endl; 
    while (!pq.empty()) { 
        auto current = pq.top(); 
        pq.pop(); 
 
        cout << "\nHandling Ticket #" << current->getId() << " | " << current->getTitle()  
             << " | Priority: " << getPriorityStr(current->getPriority()) << endl; 
 
        current->assign("Alex"); 
        current->resolve("Issue fixed successfully."); 
        current->close(); 
    } 
 
    return 0; 
} 
