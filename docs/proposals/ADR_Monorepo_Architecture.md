# ADR: Monorepo Architecture

## Status

**Accepted** 

## Context

Considering the Distributed Embedded Systems project and related future work, we aim to decide between maintaining our project in a single repository or adopting a submodule-based approach (see [ADR:  Polyrepo Architecture using Git Submodules](ADR_Polyrepo_Architecture.md))

A **monorepo** is a single repository that contains multiple distinct projects with well-defined relationships. 

It is important to emphasize that a monorepo is not the same as a monolithic repository—this is a common misconception that often leads to confusion. Having all components of a project in the same repository does not imply maintaining a single artifact for deployment.

## Proposal

We propose adopting a monorepo architecture, complemented by a well-defined CI/CD pipeline to efficiently manage releases, deployments, and the integration of project components.

## Consequences

### Positive Consequences
- **Unified access to code**: With a monorepo, all code is accessible for every component within the repository.  
- **Simplified code-sharing**: Sharing code becomes easier, as all components are readily accessible.  
- **Compatibility with Bazel**: The use of Bazel, which we are considering, is particularly effective in a monorepo setup.  
- **Centralized configuration**: Configuration related to CI/CD, testing, and other aspects is centralized, ensuring consistency across the project.  

### Negative Consequences
- **Increased cloning time**: Monorepo projects can grow significantly in size, potentially leading to long wait times for git operations like cloning or checking the repository status.  
- **Complex CI configuration requirements**: Poor CI/CD configuration can cause significant issues due to the interdependencies among project components.  
- **Challenges with mixed technologies**: Supporting multiple technologies in the same repository can complicate the setup and require additional attention.  
