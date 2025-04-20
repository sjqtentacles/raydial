# Contributing to RayDial

Thank you for considering contributing to RayDial! This document outlines the process for contributing to the project.

## Code of Conduct

By participating in this project, you agree to abide by the [Code of Conduct](CODE_OF_CONDUCT.md). Please read it before contributing.

## How Can I Contribute?

### Reporting Bugs

This section guides you through submitting a bug report. Following these guidelines helps maintainers understand your report.

- **Use the GitHub issue tracker** — Check if the bug has already been reported in the [issue tracker](https://github.com/yourusername/raydial/issues).
- **Use a clear and descriptive title**.
- **Provide a step-by-step description** of how to reproduce the problem.
- **Describe the behavior you observed** and what you expected to see.
- **Include screenshots or animated GIFs** if possible.
- **Include details about your environment** (OS, compiler version, etc.).

### Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues:

- **Use a clear and descriptive title**.
- **Provide a detailed description of the suggested enhancement**.
- **Explain why this enhancement would be useful** to most RayDial users.
- **List some other libraries or applications where this enhancement exists**, if applicable.

### Pull Requests

- **Fill in the required template**.
- **Follow the C Programming Style Guide**.
- **Include appropriate test cases**.
- **End all files with a newline**.
- **Document new code** using the established documentation style.

## Development Workflow

1. Fork the repository.
2. Create a new branch for your feature or bugfix (`git checkout -b feature/your-feature-name`).
3. Make your changes.
4. Run the tests to ensure your changes didn't break anything.
5. Commit your changes (`git commit -am 'Add some feature'`).
6. Push to the branch (`git push origin feature/your-feature-name`).
7. Create a new Pull Request.

## Coding Standards

### C Style Guide

- Use 4 spaces for indentation, not tabs.
- Use lowercase for variable names with underscores separating words.
- Use uppercase for constants and macros.
- No trailing whitespace at the end of lines.
- Line width should be limited to 80 characters when possible.
- Always use braces for control structures (if, for, while, etc.), even for single-line blocks.
- Keep functions short and focused on a single task.
- Comment your code, explaining the "why" not the "what".

### Commit Message Guidelines

- Use the present tense ("Add feature" not "Added feature").
- Use the imperative mood ("Move cursor to..." not "Moves cursor to...").
- Limit the first line to 72 characters or less.
- Reference issues and pull requests after the first line.

## Testing

- All new features should include test cases.
- Run the existing tests before submitting your changes.
- Tests are run using `make test` in the build directory.

## Documentation

- Document all public functions, types, and constants.
- Update the README.md if necessary.
- Update the API.md with any new function or changes to existing functions.

Thank you for contributing to RayDial! 