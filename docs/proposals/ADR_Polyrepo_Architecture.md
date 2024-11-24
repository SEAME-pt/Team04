
# ADR: Polyrepo Architecture using Git Submodules

## Status

**Refused** 
(Due to acceptance of [ADR: Monorepo Architecture](ADR_Monorepo_Architecture.md))

## Context

Considering the Distributed Embedded Systems project and related future work, we aim to decide between maintaining our project in a single repository or using submodules (see [ADR: Monorepo Architecture](ADR_Monorepo_Architecture.md))

A **polyrepo architecture** is a software development approach where each project or component is stored in a separate repository, and their integration is managed through tools like Git submodules. This allows for greater modularity and flexibility when working with multiple independent projects.

## Proposal

Adopt a polyrepo architecture using Git submodules. Git submodules allow embedding one repository within another, enabling teams to manage dependencies between separate projects while maintaining clear boundaries.

## Consequences

### Positive Consequences:
- **Improved modularity**: Each component can be developed, versioned, and deployed independently.  
- **Fine-grained access control**: Different repositories allow distinct permissions and access levels for each project.  
- **Smaller repository size**: Cloning individual repositories is faster and more efficient, especially for large projects.  
- **Easier collaboration on specific modules**: Teams can focus on individual repositories without being affected by changes in unrelated parts of the system.  
- **Clear separation of concerns**: Submodules help enforce boundaries between components, reducing potential cross-component interference.  

### Negative Consequences:
- **Complexity in managing submodules**: Submodules require careful management to avoid issues with synchronization, version mismatches, or broken dependencies.  
- **Challenging updates**: Updating submodules involves extra steps, which can lead to errors if not handled correctly.  
- **CI/CD setup complexity**: Configuring pipelines for multiple repositories and ensuring seamless integration can be more challenging compared to a monorepo.  
- **Difficult cross-module changes**: Changes that span multiple modules require coordination across repositories, increasing the effort needed for implementation and testing.  
