# ADR: Naming Conventions

## Status

**Accepted** 

## Context

To maintain consistency, readability, and alignment with best practices, we need a unified naming convention for files, folders, and other code artifacts. This document outlines proposals based on team discussions and suggestions from training material.

## Proposal

Adopt the following naming conventions:

### 1. Folder Naming
- **Use lowercase for all folder names**, with words separated by hyphens (`-`) for readability.  
  Example:  
  - `project-components/`  
  - `distributed-systems/`  

### 2. File Naming
- **Class files:**  
  File names should match the class names written in UpperCamelCase.  
  Examples:  
  - `ClassName.hpp` or `ClassName.h` (declaration files)  
  - `ClassName.cpp` (implementation files)  
  - `ClassName.tpp` (template implementation files)  

- **Non-class files:**  
  Use lowercase with words separated by underscores (`_`) for utility or configuration files.  
  Examples:  
  - `constants.hpp`  
  - `build_config.yaml`  

### 3. Code Style
- Separate **definition** and **declaration** into different files:
  - `.hpp` or `.h` for declarations.  
  - `.cpp` for definitions.  
  - Exception: template classes may include both in `.tpp` files.  

- Names must be **meaningful** and self-explanatory. Avoid abbreviations unless widely accepted (e.g., `id`, `url`).  

- Add comments to clarify complex code. Code without comments will be considered poor quality.  

### 4. Git Commit Messages
- Commit messages must be **understandable to others** and describe the purpose of the change.  
