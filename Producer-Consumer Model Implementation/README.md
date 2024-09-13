# Producer-Consumer Communication Model Using Condition Variables

### Problem Statement

Implement a communication model between threads using a Producer/Consumer pattern. The communication is managed via an intermediate queue controlled by condition variables. One group of 3 threads will produce vector clocks that will be placed in the queue, while another group of 3 threads will consume these vector clocks from the queue and print them to the standard output. To ensure correct use of condition variables for synchronizing producers and consumers, implement the following two test scenarios:

1. **Scenario where the queue will become full**: Producer threads produce vector clocks at a faster rate than the consumer threads. For example, each producer thread generates a vector clock every second, while each consumer thread consumes one vector clock every two seconds.

2. **Scenario where the queue will become empty**: Producer threads produce vector clocks at a slower rate than the consumer threads. For example, each producer thread generates one vector clock every two seconds, while each consumer thread consumes one vector clock every second.

---

## How to Run the Program

1. **Compile the code**: Use the following command to compile the program:

    ```bash
    gcc -o producer_consumer producer_consumer.c -lpthread
    ```

2. **Run the program**: After compiling, you can run the executable with:

    ```bash
    ./producer_consumer
    ```

3. **Select the scenario**: Once you run the program, it will prompt you to choose between two scenarios:

    - Enter `1` for the scenario where the queue will become full.
    - Enter `2` for the scenario where the queue will become empty.

    The output will display the behavior of the producer and consumer threads for the chosen scenario.

---

## How the Code Works

### Thread Synchronization Using Condition Variables

- The program uses **mutexes** and **condition variables** to control access to the shared queue between producer and consumer threads. Mutexes are used to ensure that only one thread can modify the queue at a time, while condition variables manage the synchronization between producers and consumers.

- Two condition variables are used:
  - `condFull`: Signifies that the queue is full. When the queue is full, producer threads wait for the consumers to remove an item before they can add another one.
  - `condEmpty`: Signifies that the queue is empty. When the queue is empty, consumer threads wait for the producers to add an item before they can consume it.

### Data Structures

- The **`VectorClock`** struct represents the vector clock produced by the threads. It has:
  - An `id` to represent the unique identifier of the clock.
  - An array `vectorClock[VECTOR_CLOCK_SIZE]` to store the vector clock values.

- The **`Queue`** struct is used to represent the queue where vector clocks are stored:
  - An array `vectorClockQueue[QUEUE_SIZE]` stores the vector clocks.
  - An integer `arraySize` stores the current number of items in the queue.

### Producer Threads

Each producer thread generates a new vector clock and tries to insert it into the shared queue. The thread will wait if the queue is full. Once a spot opens in the queue, the producer adds the vector clock and notifies the consumer threads using the `condEmpty` condition variable.

- **Function**: `addToQueue()`
  - This function is responsible for adding a vector clock to the queue. It locks the mutex, checks if the queue is full, and waits if necessary. After adding a new item, it signals the consumer threads that the queue is no longer empty.
### Consumer Threads
Each consumer thread removes a vector clock from the queue. The thread will wait if the queue is empty. Once an item is available in the queue, the consumer removes it and notifies the producer threads using the `condFull` condition variable.
- **Function**: `removeFromQueue()`
  - This function is responsible for removing a vector clock from the queue. It locks the mutex, checks if the queue is empty, and waits if necessary. After removing an item, it signals the producer threads that the queue is no longer full.
### Program Logic
1. **Initialization**: The mutex and condition variables are initialized. Three producer threads and three consumer threads are created.
2. **Scenarios**:
   - In the **first scenario**, producer threads generate vector clocks at a faster rate than the consumer threads consume them, resulting in a full queue.
   - In the **second scenario**, consumer threads consume vector clocks at a faster rate than the producer threads can produce, resulting in an empty queue.
3. **Execution**: The producer threads generate vector clocks and place them in the queue, while the consumer threads consume the vector clocks and print them. The synchronization is handled using condition variables to prevent data races and ensure correct execution order.
4. **Termination**: The program runs indefinitely, simulating real-time production and consumption of vector clocks. To stop the program, terminate it manually (e.g., using `Ctrl + C`).
---
This model effectively demonstrates thread communication, synchronization using condition variables, and the Producer-Consumer problem in C.
