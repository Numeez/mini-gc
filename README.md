

# Mini GC

A lightweight **Garbage Collector implementation in C**, built as a learning project to explore and understand memory management techniques at a low level.  

This project starts with a **Reference Counting** garbage collection strategy, and later evolves into a more sophisticated **Mark-and-Sweep** collector.  

---

## 🚀 Overview

Memory management is one of the most critical parts of programming languages and runtime systems. While C provides manual memory management with `malloc` and `free`, this project demonstrates how higher-level memory management strategies (like those found in Python, Java, and Go) can be implemented from scratch.

The goal of **Mini GC** is to:

- Implement a simple runtime with objects allocated on a heap.
- Manage object lifetimes without requiring explicit `free` calls by the user.
- Experiment with different GC strategies (Reference Counting → Mark and Sweep).
- Provide a testable and extensible foundation for further experiments in memory management and language runtimes.

---

## ✨ Features

- **Reference Counting**
  - Each object tracks how many references point to it.
  - Objects are freed immediately when their reference count drops to zero.
  - Simple and efficient, but cannot handle cyclic references.

- **Mark and Sweep**
  - The runtime "marks" reachable objects by traversing from root references (stack, globals).
  - "Sweeps" away all unmarked (unreachable) objects from the heap.
  - Solves the cycle problem inherent in reference counting.

- **Minimal Virtual Machine (VM)**
  - Object stack for managing execution frames.
  - Support for allocating integers, strings, and composite objects.
  - Tracing and freeing logic integrated with the GC.

- **Unit Tests**
  - Thorough tests for allocation, deallocation, and garbage collection.
  - Ensures correctness of memory management and VM behavior.

---

## 🛠️ Technologies Used

- **C (C99)** – for low-level control over memory and data structures.
- **Munit** – minimal unit testing framework for C.
- **Make** – build system for compiling and running tests.

---

## ⚡ Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/your-username/mini-gc.git
cd mini-gc
```

### 2. Run Demo

```bash
make run
```
